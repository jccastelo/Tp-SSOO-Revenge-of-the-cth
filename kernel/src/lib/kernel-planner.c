#include "../include/kernel-planner.h"

void planner_init(){

    t_planner *planner = malloc(sizeof(t_planner));

    planner->short_term = malloc(sizeof(t_short_term));
    planner->long_term = malloc(sizeof(t_long_term));

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

     switch(get_algoritm(config_kernel->ALGORITMO_CORTO_PLAZO))
    {
    case FIFO:
        planner->short_term->algoritmo_planificador = queue_FIFO;
        break;
    
    case SJFsD:
        //planner->long_term->algoritmo_planificador = queue_SJFsD();
        break;

    case SJFcD:
        //planner->long_term->algoritmo_planificador = queue_SJFcD();
        break;
        
    default:
        log_info(logger,"Mal ingresado el nombre del algoritmo planificador");
        abort();
    }

    planner->short_term->queue_READY = queue_create();
    planner->long_term->queue_NEW= queue_create();
    planner->long_term->queue_BLOCKED = queue_create();

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