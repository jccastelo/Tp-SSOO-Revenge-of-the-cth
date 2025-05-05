#include "../include/kernel-planner.h"

void planner_init(){

    l_procesos = list_create();

    t_planner *planner = malloc(sizeof(t_planner));

    planner->short_term = malloc(sizeof(t_short_term));
    planner->medium_term = malloc(sizeof(t_medium_term));
    planner->long_term = malloc(sizeof(t_long_term));
    planner->queue_EXECUTE = malloc(sizeof(t_mutex_queue));
    
    pthread_mutex_init(planner->queue_EXECUTE->mutex, NULL);
    planner->queue_EXECUTE->queue_ESTADO = queue_create();

    switch(get_algoritm(config_kernel->ALGORITMO_INGRESO_A_READY))
    {
    case FIFO:
        planner->long_term->algoritmo_planificador = queue_FIFO;
        break;
    
    case PMCP:
        //planner->long_term->algoritmoPlanificador_long = queue_PMCP();
        break;
    
    default:
        log_info(logger,"Mal ingresado el nombre del algoritmo planificador");
        abort();
    }

    planner->medium_term->algoritmo_planificador = queue_FIFO;

    switch(get_algoritm(config_kernel->ALGORITMO_CORTO_PLAZO))
    {
    case FIFO:
        planner->short_term->algoritmo_planificador = queue_FIFO;
        break;
    
    case SJFsD:
        //planner->long_term->algoritmo_planificador = queue_SJFsD;
        break;

    case SJFcD:
        //planner->long_term->algoritmo_planificador = queue_SJFcD;
        break;
        
    default:
        log_info(logger,"Mal ingresado el nombre del algoritmo planificador");
        abort();
    }

    planner->short_term->queue_READY->queue_ESTADO = queue_create();

    planner->medium_term->queue_BLOCKED_SUSPENDED->queue_ESTADO = queue_create();
    planner->medium_term->queue_READY_SUSPENDED->queue_ESTADO = queue_create();

    planner->long_term->queue_NEW->queue_ESTADO = queue_create();
    planner->long_term->queue_EXIT->queue_ESTADO = queue_create();
    planner->long_term->queue_BLOCKED->queue_ESTADO = queue_create();

    pthread_mutex_init(planner->short_term->queue_READY->mutex, NULL);

    pthread_mutex_init(planner->medium_term->queue_BLOCKED_SUSPENDED->mutex, NULL);
    pthread_mutex_init(planner->medium_term->queue_READY_SUSPENDED->mutex, NULL);

    pthread_mutex_init(planner->long_term->queue_NEW->mutex, NULL);
    pthread_mutex_init(planner->long_term->queue_BLOCKED->mutex, NULL);
    pthread_mutex_init(planner->long_term->queue_EXIT->mutex, NULL);

    log_info(logger,"Planificador listo. Esperando para iniciar...");

    getchar();
    
    log_info(logger,"PLANIFICADOR INICIADO");
}   

int get_algoritm(char *algoritm_name)
{
    if(strcmp("FIFO",algoritm_name))
    {return FIFO;}

    else if(strcmp("PMCP",algoritm_name))
    {return PMCP;}

    else if(strcmp("SJFsD",algoritm_name))
    {return SJFsD;}

    else if(strcmp("SJFcD",algoritm_name))
    {return SJFcD;}

    else{log_info(logger,"BOOOM");
    return -1;}

}

void queue_process(t_pcb* process, int estado){

    switch(estado)
    {
    case NEW:
        process->metricas_de_estado->new += 1;
        actualizarTiempo(&(process->metricas_de_tiempo->metrica_actual),&(process->metricas_de_tiempo->NEW));
        cambiar_estado(planner->long_term->algoritmo_planificador, process, planner->long_term->queue_NEW);
        memoria_init_proc(process);
        break;

    case READY:
        process->metricas_de_estado->ready += 1;
        actualizarTiempo(&(process->metricas_de_tiempo->metrica_actual),&(process->metricas_de_tiempo->READY));
        cambiar_estado(planner->long_term->algoritmo_planificador, process, planner->short_term->queue_READY);
        break;

    case EXECUTE:
        process->metricas_de_estado->execute += 1;
        actualizarTiempo(&(process->metricas_de_tiempo->metrica_actual),&(process->metricas_de_tiempo->EXECUTE));
        cambiar_estado(queue_FIFO, process, planner->queue_EXECUTE);
        // Aca entra el dispatcher => mandamos el proceso a cpu
        break;

    case BLOCKED:
        process->metricas_de_estado->blocked += 1;
        actualizarTiempo(&(process->metricas_de_tiempo->metrica_actual),&(process->metricas_de_tiempo->BLOCKED));
        cambiar_estado(planner->long_term->algoritmo_planificador, process, planner->long_term->queue_BLOCKED);
        break;

    case BLOCKED_SUSPENDED:
        process->metricas_de_estado->blocked_suspended += 1;
        actualizarTiempo(&(process->metricas_de_tiempo->metrica_actual),&(process->metricas_de_tiempo->BLOCKED_SUSPENDED));
        cambiar_estado(planner->medium_term->algoritmo_planificador, process, planner->medium_term->queue_BLOCKED_SUSPENDED);
        //a
        break;

    case READY_SUSPENDED:
        process->metricas_de_estado->ready_suspended += 1;
        actualizarTiempo(&(process->metricas_de_tiempo->metrica_actual),&(process->metricas_de_tiempo->READY_SUSPENDED));
        cambiar_estado(planner->medium_term->algoritmo_planificador, process, planner->medium_term->queue_READY_SUSPENDED);
        //a
        break;

    case EXIT:
        process->metricas_de_estado->exit += 1;
        temporal_stop(process->metricas_de_tiempo->metrica_actual);
        cambiar_estado(planner->long_term->algoritmo_planificador, process, planner->long_term->queue_EXIT);
        // carnicero(process); deberiamos crear una cola de procesos que hicieron exit y eliminar de a uno o hacer hilos con la funcion carnicero
        break;
    }
}

void cambiar_estado(void (*algoritmo_planificador)(t_pcb* process, t_queue* estado), t_pcb* process, t_mutex_queue* sgteEstado){
    
    if(process->queue_ESTADO_ACTUAL != NULL){
        // Cerramos el mutex y sacamos el pcb de la cola del estado en el que estaba el proceso (que esta primero)
        pthread_mutex_lock(process->queue_ESTADO_ACTUAL->mutex);
        queue_pop(process->queue_ESTADO_ACTUAL->queue_ESTADO);
        pthread_mutex_unlock(process->queue_ESTADO_ACTUAL->mutex);
    }

    // Cerramos el mutex y replanificamos la cola del estado al que pasamos agregando el pcb
    pthread_mutex_lock(sgteEstado->mutex);
    algoritmo_planificador(process, sgteEstado->queue_ESTADO);
    pthread_mutex_unlock(sgteEstado->mutex);

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


void queue_FIFO(t_pcb *process, t_queue *queue)
{
    queue_push(queue,process);
}