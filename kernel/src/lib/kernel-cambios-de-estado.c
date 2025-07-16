#include "../include/kernel-cambios-de-estado.h"

pthread_mutex_t mutex_desalojo = PTHREAD_MUTEX_INITIALIZER;

void queue_process(t_pcb* process, int estado){

     char *estadoActual= get_NombreDeEstado(process->queue_ESTADO_ACTUAL);

    switch(estado)
    {
    case NEW:

        log_info(logger,"## PID: %d,Tamnio %d - Se crea el proceso - Estado: NEW", process->pid,process->tamanio_proceso);
        
        process->metricas_de_estado->new += 1;
        temporal_resume(process->metricas_de_tiempo->NEW);
        process->metricas_de_tiempo->metrica_actual = process->metricas_de_tiempo->NEW;

        cambiar_estado(planner->long_term->algoritmo_planificador, process, planner->long_term->queue_NEW); 

        pthread_mutex_lock(&planner->medium_term->queue_READY_SUSPENDED->mutex);
        if(!list_is_empty(planner->medium_term->queue_READY_SUSPENDED->cola)){
            pthread_mutex_unlock(&planner->medium_term->queue_READY_SUSPENDED->mutex);
            break;
        }
        pthread_mutex_unlock(&planner->medium_term->queue_READY_SUSPENDED->mutex);
        
        pthread_mutex_lock(&planner->long_term->queue_NEW->mutex);
        if(list_size(planner->long_term->queue_NEW->cola) == 1) {
            
            pthread_mutex_unlock(&planner->long_term->queue_NEW->mutex);           
            
            if(solicitar_a_memoria(memoria_init_proc, process)) {
                queue_process(process, READY);
            }
            break;
        }
        pthread_mutex_unlock(&planner->long_term->queue_NEW->mutex);

        pthread_mutex_lock(&planner->long_term->queue_NEW->mutex);
        if(get_algoritm(config_kernel->ALGORITMO_INGRESO_A_READY) == PMCP && list_get(planner->long_term->queue_NEW->cola,0) == process){

            pthread_mutex_unlock(&planner->long_term->queue_NEW->mutex);

            if(solicitar_a_memoria(memoria_init_proc, process)) {
                queue_process(process, READY);
            }
            break;
        }
        pthread_mutex_unlock(&planner->long_term->queue_NEW->mutex);

        break;

    case READY:

        log_info(logger,"## PID: %d Pasa del estado %s al estado READY",process->pid,estadoActual);
        
        process->metricas_de_estado->ready += 1;
        actualizarTiempo(&(process->metricas_de_tiempo->metrica_actual),&(process->metricas_de_tiempo->READY));
        
        cambiar_estado(planner->short_term->algoritmo_planificador, process, planner->short_term->queue_READY);

        //pthread_mutex_lock(&mutex_cpu); 

        t_cpu* cpu_disponible = buscar_cpu_disponible();
        if(cpu_disponible != NULL)
        {
            pthread_mutex_lock(&cpu_disponible->mutex);
            //set_cpu(cpu_disponible->socket_dispatch,EJECUTANDO,process->pid); al re pedo, para que voy a buscarla si ya ltenfo
            cpu_disponible->estado = EJECUTANDO;
            cpu_disponible->pid = process->pid;
            pthread_mutex_unlock(&cpu_disponible->mutex);

            queue_process(process, EXECUTE);
  
        } else { // Si el proceso que entro esta primero ahi se fija si desaloja
            pthread_mutex_lock(&planner->short_term->queue_READY->mutex);            
            t_pcb* primero = list_get(planner->short_term->queue_READY->cola,0);
            pthread_mutex_unlock(&planner->short_term->queue_READY->mutex);
            
            if(primero == process && planner->short_term->algoritmo_desalojo == desalojo_SJF) {
                
                log_debug(logger, "INTENO DESALOJO");

                pthread_mutex_lock(&mutex_desalojo);
                planner->short_term->algoritmo_desalojo(process);
                pthread_mutex_unlock(&mutex_desalojo); // Si tiene desalojo ejecuta, sino null pattern 
                
                log_debug(logger, "INTENO DESALOJO TERMINADO");
            }
        }
        
        break;

    case EXECUTE:

        log_info(logger,"## PID: %d Pasa del estado %s al estado EXECUTE",process->pid,estadoActual);

        process->metricas_de_estado->execute += 1;
        actualizarTiempo(&(process->metricas_de_tiempo->metrica_actual),&(process->metricas_de_tiempo->EXECUTE));
        
        cambiar_estado(queue_FIFO, process, planner->queue_EXECUTE);
        t_cpu* cpu_a_ocupar = buscar_mi_cpu(process->pid);

        if(cpu_a_ocupar == NULL) // busca la CPU disponible y envia el proceso
        {
            log_debug(logger, "PARA WACHO NO HAY CPU DISPONIBLE"); 
            queue_process(process,READY); 
            break;
        } //NO puede haber procesos en EXECUTE que no estan ejecutando    

        pthread_mutex_lock(&cpu_a_ocupar->mutex);
        enviar_proceso_cpu(cpu_a_ocupar->socket_dispatch, process);
        pthread_mutex_unlock(&cpu_a_ocupar->mutex);

        if(planner->short_term->algoritmo_planificador == queue_SJF){
            
            process->estimaciones_SJF->rafagaReal = temporal_create();
            temporal_resume(process->estimaciones_SJF->rafagaReal);
        }
        break;

    case BLOCKED:
    
        log_info(logger,"## PID: %d Pasa del estado %s al estado BLOCKED",process->pid,estadoActual);

        process->metricas_de_estado->blocked += 1;
        actualizarTiempo(&(process->metricas_de_tiempo->metrica_actual),&(process->metricas_de_tiempo->BLOCKED));
        
        cambiar_estado(planner->long_term->algoritmo_planificador, process, planner->long_term->queue_BLOCKED);

        bloquearProceso(process);

        break;

    case BLOCKED_SUSPENDED:

        if(strcmp(get_NombreDeEstado(process->queue_ESTADO_ACTUAL),"BLOCKED"))
        {
            if(process->hilo_activo){
                {
                    pthread_cancel(process->hilo_block);
                }
                    pthread_join(process->hilo_block, NULL);
                }
            return;
        }

        log_info(logger,"## PID: %d Pasa del estado %s al estado BLOCKED_SUSPENDED",process->pid,estadoActual);

        process->metricas_de_estado->blocked_suspended += 1;
        actualizarTiempo(&(process->metricas_de_tiempo->metrica_actual),&(process->metricas_de_tiempo->BLOCKED_SUSPENDED));
        
        cambiar_estado(planner->medium_term->algoritmo_planificador, process, planner->medium_term->queue_BLOCKED_SUSPENDED);

        log_debug(logger,"SOLICITANDO SUSP A MEMORIA");

        if(solicitar_a_memoria(suspender_proceso, process))
        {
            log_debug(logger,"SUSP A MEMORIA ACEPTADO");
            traer_proceso_a_MP();

            if(process->hilo_activo) {
            {
                pthread_cancel(process->hilo_block);
            }
                pthread_join(process->hilo_block, NULL);
            }
            
        }
        else{ log_debug(logger,"Error al suspender proceso"); }

        
        break;

    case READY_SUSPENDED:
        log_info(logger,"## PID: %d Pasa del estado %s al estado READY_SUSPENDED",process->pid,estadoActual);

        process->metricas_de_estado->ready_suspended += 1;
        actualizarTiempo(&(process->metricas_de_tiempo->metrica_actual),&(process->metricas_de_tiempo->READY_SUSPENDED));
        
        cambiar_estado(planner->medium_term->algoritmo_planificador, process, planner->medium_term->queue_READY_SUSPENDED);
        
        pthread_mutex_lock(&planner->medium_term->queue_READY_SUSPENDED->mutex);        
        int cant_procesos = list_size(planner->medium_term->queue_READY_SUSPENDED->cola);
        pthread_mutex_unlock(&planner->medium_term->queue_READY_SUSPENDED->mutex);
        
        if(cant_procesos == 1){
            traer_proceso_a_MP();
        }

        break;

    case EXIT:
        
        log_info(logger,"## PID: %d Pasa del estado %s al estado EXIT",process->pid,estadoActual);

        log_info(logger,"## PID: %d - Finaliza el proceso",process->pid);
        
        process->metricas_de_estado->exit += 1;

        cambiar_estado(planner->long_term->algoritmo_planificador, process, planner->long_term->queue_EXIT);
        
        if(solicitar_a_memoria(memoria_delete_process, process))
        {
            carnicero(process);
        }
        else{log_debug(logger, "Error al pasar proceso a exit");}
        
        traer_proceso_a_MP();
        log_debug(logger , "NO mas procesos por traer a mp desde exit");
        break;
    }
}

void cambiar_estado(void (*algoritmo_planificador)(t_pcb* process, t_list* estado), t_pcb* process, t_monitor* sgteEstado){
    
    // Verificación de parámetros
    if (process == NULL || sgteEstado == NULL ) {
        return; // O manejar error adecuadamente
    }

        // if viene de execute => frenamos el timer sjf
    if(!strcmp(get_NombreDeEstado(process->queue_ESTADO_ACTUAL),"EXECUTE") && planner->short_term->algoritmo_planificador == queue_SJF && process->estimaciones_SJF->rafagaReal != NULL){
        
        temporal_stop(process->estimaciones_SJF->rafagaReal);
        actualizar_rafagas_sjf(process);
        temporal_destroy(process->estimaciones_SJF->rafagaReal);
    } 

    if(process->queue_ESTADO_ACTUAL != NULL){
        // Cerramos el mutex y sacamos el pcb de la cola del estado en el que estaba el proceso (que esta primero)
        pthread_mutex_lock(&process->queue_ESTADO_ACTUAL->mutex);
        bool hallado = list_remove_element(process->queue_ESTADO_ACTUAL->cola, process);
        pthread_mutex_unlock(&process->queue_ESTADO_ACTUAL->mutex);        
        
        if(!hallado) {
            log_debug(logger,"NO hallado PID %d",process->pid);
        }
    }

    // Cerramos el mutex y replanificamos la cola del estado al que pasamos agregando el pcb
    pthread_mutex_lock(&sgteEstado->mutex);
    algoritmo_planificador(process, sgteEstado->cola);
    pthread_mutex_unlock(&sgteEstado->mutex);

    // Cambiamos el estado del pcb
    process->queue_ESTADO_ACTUAL = sgteEstado;

    return;
}

void actualizarTiempo(t_temporal **metrica_actual,t_temporal **metricas_de_tiempo_estado) 
{
                                                    
    temporal_stop(*metrica_actual); //Detengo la metrica actual                                                                                
 
    temporal_resume(*metricas_de_tiempo_estado); //Si ya existe entonces lo reanuda

    *metrica_actual = *metricas_de_tiempo_estado;

}

char *get_NombreDeEstado(t_monitor* queue_ESTADO)
{
    if(queue_ESTADO == planner->long_term->queue_NEW)
    {return "NEW";}

    else if(queue_ESTADO == planner->long_term->queue_BLOCKED)
    {return "BLOCKED";}

    else if(queue_ESTADO == planner->long_term->queue_EXIT)
    {return "EXIT";}

    else if(queue_ESTADO == planner->short_term->queue_READY)
    {return "READY";}

    else if(queue_ESTADO == planner->queue_EXECUTE)
    {return "EXECUTE";}

    else if(queue_ESTADO == planner->medium_term->queue_BLOCKED_SUSPENDED)
    {return "BLOCKED_SUSPENDED";}

    else if(queue_ESTADO == planner->medium_term->queue_READY_SUSPENDED)
    {return "READY_SUSPENDED";}
    else{return "NULL";}

}