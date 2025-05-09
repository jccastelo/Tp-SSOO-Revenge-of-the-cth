#include "../include/kernel-planner.h"

void planner_init(){

    list_procesos = list_create();

    list_cpus = list_create();

    t_planner *planner = malloc(sizeof(t_planner));

    planner->short_term = malloc(sizeof(t_short_term));
    planner->medium_term = malloc(sizeof(t_medium_term));
    planner->long_term = malloc(sizeof(t_long_term));
    planner->queue_EXECUTE = malloc(sizeof(t_monitor));
    
    pthread_mutex_init(planner->queue_EXECUTE->mutex, NULL);
    planner->queue_EXECUTE->queue_ESTADO = list_create();

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

    planner->short_term->queue_READY->queue_ESTADO = list_create();

    planner->medium_term->queue_BLOCKED_SUSPENDED->queue_ESTADO = list_create();
    planner->medium_term->queue_READY_SUSPENDED->queue_ESTADO = list_create();

    planner->long_term->queue_NEW->queue_ESTADO = list_create();
    planner->long_term->queue_EXIT->queue_ESTADO = list_create();
    planner->long_term->queue_BLOCKED->queue_ESTADO = list_create();

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

void traer_proceso_a_MP(){
            
    char* respuestaMemoria;

    while(list_size(planner->medium_term->queue_READY_SUSPENDED->queue_ESTADO) > 0){

        respuestaMemoria = memoria_init_proc(list_get(planner->medium_term->queue_READY_SUSPENDED->queue_ESTADO,0));
        // SE USA LA MISMA FUNCION PARA INCIAR PROCESOS EN MP QUE PARA TRAERLOS DE SWAP???
        
        if(strcmp(respuestaMemoria, "OK"))
        {
            queue_process(list_get(planner->medium_term->queue_READY_SUSPENDED->queue_ESTADO,0), READY);
        } else { break; }
    }

    while(list_size(planner->long_term->queue_NEW->queue_ESTADO) > 0){ 

        respuestaMemoria = memoria_init_proc(list_get(planner->long_term->queue_NEW->queue_ESTADO,0));

        if(!strcmp(respuestaMemoria, "OK"))
        {
            queue_process(list_get(planner->long_term->queue_NEW->queue_ESTADO,0), READY);
        } else { break; }
    }
}

void queue_FIFO(t_pcb *process, t_list *lista)
{
    list_add(lista, process);
}