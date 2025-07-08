#include "../include/kernel-cambios-de-estado.h"


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

        if(list_is_empty(planner->medium_term->queue_READY_SUSPENDED->cola) && // READY_SUSP vacia => pueden entrar los de NEW
           (list_size(planner->long_term->queue_NEW->cola) == 1 || // Si es el unico en la cola => se le pregunta a memoria si puede entrar
           (get_algoritm(config_kernel->ALGORITMO_INGRESO_A_READY) == PMCP && list_get(planner->long_term->queue_NEW->cola,0) == process))) // Sino, en el caso de PMCP => si esta primero le pregunta a memoria
            {
                if(solicitar_a_memoria(memoria_init_proc, process))
                {
                    queue_process(process, READY);
                }
                
            }

        break;

    case READY:
        
        log_info(logger,"## PID: %d Pasa del estado %s al estado READY",process->pid,estadoActual);
        
        process->metricas_de_estado->ready += 1;
        actualizarTiempo(&(process->metricas_de_tiempo->metrica_actual),&(process->metricas_de_tiempo->READY));
        
        cambiar_estado(planner->short_term->algoritmo_planificador, process, planner->short_term->queue_READY);

        if(buscar_cpu_disponible() != NULL) // si llega a READY y hay una CPU disponible va a EXECUTE
        { 
            queue_process(process, EXECUTE);
            
        } else if(list_get(planner->short_term->queue_READY->cola,0) == process) { // Si el proceso que entro esta primero ahi se fija si desaloja
            planner->short_term->algoritmo_desalojo(process); // Si tiene desalojo ejecuta, sino null pattern 
        }
        
        break;

    case EXECUTE:
       
        log_info(logger,"## PID: %d Pasa del estado %s al estado EXECUTE",process->pid,estadoActual);

        process->metricas_de_estado->execute += 1;
        actualizarTiempo(&(process->metricas_de_tiempo->metrica_actual),&(process->metricas_de_tiempo->EXECUTE));
        
        cambiar_estado(queue_FIFO, process, planner->queue_EXECUTE);

        temporal_resume(process->estimaciones_SJF->rafagaReal);

        t_cpu* cpu_a_ocupar = buscar_cpu_disponible();
        if(cpu_a_ocupar != NULL) // busca la CPU disponible y envia el proceso
        {
            enviar_proceso_cpu(cpu_a_ocupar->socket_dispatch, process);
            
        } else {
             queue_process(process,READY);//NO puede haber procesos en EXECUTE que no estan ejecutando
             log_warning(logger, "PARA WACHO NO HAY CPU DISPONIBLE"); }
        break;

    case BLOCKED:
        log_info(logger,"## PID: %d Pasa del estado %s al estado BLOCKED",process->pid,estadoActual);

        process->metricas_de_estado->blocked += 1;
        actualizarTiempo(&(process->metricas_de_tiempo->metrica_actual),&(process->metricas_de_tiempo->BLOCKED));
        
        cambiar_estado(planner->long_term->algoritmo_planificador, process, planner->long_term->queue_BLOCKED);
        
        bloquearProceso(process);

        break;

    case BLOCKED_SUSPENDED:
        
        log_warning(logger,"## PID: %d Pasa del estado %s al estado BLOCKED_SUSPENDED",process->pid,estadoActual);

        process->metricas_de_estado->blocked_suspended += 1;
        actualizarTiempo(&(process->metricas_de_tiempo->metrica_actual),&(process->metricas_de_tiempo->BLOCKED_SUSPENDED));
        
        cambiar_estado(planner->medium_term->algoritmo_planificador, process, planner->medium_term->queue_BLOCKED_SUSPENDED);

        if(solicitar_a_memoria(suspender_proceso, process))
        {traer_proceso_a_MP();}
        else{ log_error(logger,"Error al suspender proceso");}
        
        break;

    case READY_SUSPENDED:
        log_info(logger,"## PID: %d Pasa del estado %s al estado READY_SUSPENDED",process->pid,estadoActual);

        process->metricas_de_estado->ready_suspended += 1;
        actualizarTiempo(&(process->metricas_de_tiempo->metrica_actual),&(process->metricas_de_tiempo->READY_SUSPENDED));
        
        cambiar_estado(planner->medium_term->algoritmo_planificador, process, planner->medium_term->queue_READY_SUSPENDED);
        break;

    case EXIT:

        log_info(logger,"## PID: %d Pasa del estado %s al estado EXIT",process->pid,estadoActual);

        log_info(logger,"## PID: %d - Finaliza el proceso",process->pid);
        
        process->metricas_de_estado->exit += 1;
        temporal_stop(process->metricas_de_tiempo->metrica_actual);
        
        cambiar_estado(planner->long_term->algoritmo_planificador, process, planner->long_term->queue_EXIT);

        if(solicitar_a_memoria(memoria_delete_process, process))
        {
            carnicero(process);
        }
        else{log_info(logger, "Error al pasar proceso a exit");}
        
        traer_proceso_a_MP();
        log_info(logger , "NO mas procesos por traer a mp desde exit");
        break;
    }
}

void cambiar_estado(void (*algoritmo_planificador)(t_pcb* process, t_list* estado), t_pcb* process, t_monitor* sgteEstado){
    
    // Verificación de parámetros
    if (process == NULL || sgteEstado == NULL ) {
        return; // O manejar error adecuadamente
    }

        // if viene de execute => frenamos el timer sjf
    if(!strcmp(get_NombreDeEstado(process->queue_ESTADO_ACTUAL),"EXECUTE") && planner->short_term->algoritmo_desalojo == desalojo_SJF){
        temporal_stop(process->estimaciones_SJF->rafagaReal);
    } 
    // if viene de blocked => matamos el hilo
    if(!strcmp(get_NombreDeEstado(process->queue_ESTADO_ACTUAL),"BLOCKED")){
        pthread_cancel(process->hilo_block);
        pthread_join(process->hilo_block, NULL);
    }

    if(process->queue_ESTADO_ACTUAL != NULL){
        // Cerramos el mutex y sacamos el pcb de la cola del estado en el que estaba el proceso (que esta primero)
        pthread_mutex_lock(&process->queue_ESTADO_ACTUAL->mutex);
        list_remove_element(process->queue_ESTADO_ACTUAL->cola, process);
        pthread_mutex_unlock(&process->queue_ESTADO_ACTUAL->mutex);
    }

    // Cerramos el mutex y replanificamos la cola del estado al que pasamos agregando el pcb
    pthread_mutex_lock(&sgteEstado->mutex);
    algoritmo_planificador(process, sgteEstado->cola);
    pthread_mutex_unlock(&sgteEstado->mutex);

    // Cambiamos el estado del pcb
    process->queue_ESTADO_ACTUAL = sgteEstado;
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