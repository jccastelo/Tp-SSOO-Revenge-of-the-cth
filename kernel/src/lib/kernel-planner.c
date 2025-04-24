#include "../include/kernel-planner.h"

void planner_init(){

    t_planner *planner = malloc(sizeof(t_planner));

    planner->short_term = malloc(sizeof(t_short_term));
    planner->long_term= malloc(sizeof(t_long_term));

    planner->short_term->queue_READY = queue_create();
    planner->short_term->algoritmoPlanificador_short = config_kernel->ALGORITMO_CORTO_PLAZO;

    planner->long_term->queue_NEW= queue_create();
    planner->long_term->queue_BLOCKED = queue_create();
    planner->long_term->algoritmoPlanificador_long = config_kernel->ALGORITMO_INGRESO_A_READY;

    log_info(logger,"Planificador listo. Esperando para iniciar...");

    getchar();
    
    log_info(logger,"PLANIFICADOR INICIADO");
}   