#include "../include/kernel-cambios-de-estado.h"

void queue_process(t_pcb* process, int estado){

    switch(estado)
    {
    case NEW:
        log_info(logger, "Proceso en NEW");
        process->metricas_de_estado->new += 1;
        actualizarTiempo(&(process->metricas_de_tiempo->metrica_actual),&(process->metricas_de_tiempo->NEW));
        cambiar_estado(planner->long_term->algoritmo_planificador, process, planner->long_term->queue_NEW); 

        if(list_size(planner->long_term->queue_NEW->queue_ESTADO) == 1 ){ // Si la cola estaba vacia manda la solicitud a memoria (size retornaria 1 que es igual a true)
            
            
            if(strcmp(memoria_init_proc(process), "OK")==0){

                queue_process(process, READY);
            } 
        }

        break;

    case READY:
        log_info(logger, "Proceso en READY");
        process->metricas_de_estado->ready += 1;
        actualizarTiempo(&(process->metricas_de_tiempo->metrica_actual),&(process->metricas_de_tiempo->READY));
        cambiar_estado(planner->long_term->algoritmo_planificador, process, planner->short_term->queue_READY);
        
        if(buscar_cpu_disponible() != NULL) // si llega a READY y hay una CPU disponible va a EXECUTE
        {
            queue_process(process, EXECUTE);
            log_info(logger, "Proceso en EJECUTANDO EN cpu...");
                return; //test
        }else { log_error(logger, "PARA WACHO NO HAY CPU DISPONIBLE"); }

        break;

    case EXECUTE:
        process->metricas_de_estado->execute += 1;
        actualizarTiempo(&(process->metricas_de_tiempo->metrica_actual),&(process->metricas_de_tiempo->EXECUTE));
        cambiar_estado(queue_FIFO, process, planner->queue_EXECUTE);

        t_cpu* cpu_a_ocupar = buscar_cpu_disponible();
        if(cpu_a_ocupar != NULL) // busca la CPU disponible y envia el proceso
        {
            enviar_proceso_cpu(cpu_a_ocupar->socket_cpu, process);
        } else { log_error(logger, "PARA WACHO NO HAY CPU DISPONIBLE"); }
        break;

    case BLOCKED:
        process->metricas_de_estado->blocked += 1;
        actualizarTiempo(&(process->metricas_de_tiempo->metrica_actual),&(process->metricas_de_tiempo->BLOCKED));
        cambiar_estado(planner->long_term->algoritmo_planificador, process, planner->long_term->queue_BLOCKED);
        
        if(list_size(planner->short_term->queue_READY->queue_ESTADO) > 0)
        {
            t_pcb *sgte_proceso = list_get(planner->short_term->queue_READY->queue_ESTADO,0);
            queue_process(sgte_proceso, EXECUTE);
        }

        break;

    case BLOCKED_SUSPENDED:
        process->metricas_de_estado->blocked_suspended += 1;
        actualizarTiempo(&(process->metricas_de_tiempo->metrica_actual),&(process->metricas_de_tiempo->BLOCKED_SUSPENDED));
        cambiar_estado(planner->medium_term->algoritmo_planificador, process, planner->medium_term->queue_BLOCKED_SUSPENDED);
        traer_proceso_a_MP();
        break;

    case READY_SUSPENDED:
        process->metricas_de_estado->ready_suspended += 1;
        actualizarTiempo(&(process->metricas_de_tiempo->metrica_actual),&(process->metricas_de_tiempo->READY_SUSPENDED));
        cambiar_estado(planner->medium_term->algoritmo_planificador, process, planner->medium_term->queue_READY_SUSPENDED);
        break;

    case EXIT:
        log_info(logger, "Proceso en EXIT");
        process->metricas_de_estado->exit += 1;
        temporal_stop(process->metricas_de_tiempo->metrica_actual);
        cambiar_estado(planner->long_term->algoritmo_planificador, process, planner->long_term->queue_EXIT);


        if(memory_delete_process(process) == 51)
        {
            carnicero(process);
        }
        else{log_info(logger, "Error al pasar proceso a exit");}
        
        traer_proceso_a_MP();
        log_info(logger , "NO mas porcesos por traer a mp desde exit");
        break;
    }
}

void cambiar_estado(void (*algoritmo_planificador)(t_pcb* process, t_list* estado), t_pcb* process, t_monitor* sgteEstado){
    
    // Verificación de parámetros
    if (process == NULL || sgteEstado == NULL ) {
        return; // O manejar error adecuadamente
    }

    if(process->queue_ESTADO_ACTUAL != NULL){
        // Cerramos el mutex y sacamos el pcb de la cola del estado en el que estaba el proceso (que esta primero)
        pthread_mutex_lock(&process->queue_ESTADO_ACTUAL->mutex);
        list_remove_element(process->queue_ESTADO_ACTUAL->queue_ESTADO, process);
        pthread_mutex_unlock(&process->queue_ESTADO_ACTUAL->mutex);
    }

    // Cerramos el mutex y replanificamos la cola del estado al que pasamos agregando el pcb
    pthread_mutex_lock(&sgteEstado->mutex);
    algoritmo_planificador(process, sgteEstado->queue_ESTADO);
    pthread_mutex_unlock(&sgteEstado->mutex);

    // Cambiamos el estado del pcb
    process->queue_ESTADO_ACTUAL = sgteEstado;
}

void actualizarTiempo(t_temporal **metrica_actual,t_temporal **metricas_de_tiempo_estado) 
{ //La funcion recibe la metrica que se esta ejecutando y la nueva
    

    if (*metrica_actual != NULL) 
    {  
        temporal_stop(*metrica_actual); //Detengo la metrica actual
    }

    if( *metricas_de_tiempo_estado == NULL)
    {
        *metricas_de_tiempo_estado= temporal_create(); //Si es la primera vez que entra en el estado se crea e inicializa
    }else{

        temporal_resume(*metricas_de_tiempo_estado); //Si ya existe entonces lo reanuda
    }

    *metrica_actual = *metricas_de_tiempo_estado;

}