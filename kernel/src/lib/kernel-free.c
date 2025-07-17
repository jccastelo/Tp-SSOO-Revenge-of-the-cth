
#include "../include/kernel-free.h"

void carnicero(t_pcb* process){

    pthread_mutex_lock(&process->queue_ESTADO_ACTUAL->mutex);
    list_remove_element(process->queue_ESTADO_ACTUAL->cola, process);
    pthread_mutex_unlock(&process->queue_ESTADO_ACTUAL->mutex);
    
    //SI hago un remove, los procesos se moverian de posicion, por lo tanto reemplazamos el proceso por un pid negativo para marcar esa posicion como vacia y poder seguir buscando los procesos por pid
    int PROCESOS_ELIMINADO =-99;
    pthread_mutex_lock(&list_procesos->mutex);
    list_replace(list_procesos->cola,process->pid, &PROCESOS_ELIMINADO);
    pthread_mutex_unlock(&list_procesos->mutex);

    // aca van los logs
    log_metricas(process);

    free(process->archivo);    
    free(process->metricas_de_estado);

    free(process->metricas_de_tiempo->NEW);
    free(process->metricas_de_tiempo->READY);
    free(process->metricas_de_tiempo->EXECUTE);
    free(process->metricas_de_tiempo->BLOCKED);
    free(process->metricas_de_tiempo->BLOCKED_SUSPENDED);
    free(process->metricas_de_tiempo->READY_SUSPENDED);

    free(process->metricas_de_tiempo);    

    free(process);
}

void log_metricas(t_pcb* process){
char buffer[1024];

    snprintf(buffer, 1024,
        "\nFin de proceso (PID): %d\n"
        "Métricas por estado:\n"
        "NEW: Ocurrencias %d  Tiempo: %" PRId64 "\n"
        "READY: Ocurrencias %d  Tiempo: %" PRId64 "\n"
        "EXECUTE: Ocurrencias %d  Tiempo: %" PRId64 "\n"
        "BLOCKED: Ocurrencias %d  Tiempo: %" PRId64 "\n"
        "READY SUSPENDED: Ocurrencias %d  Tiempo: %" PRId64 "\n"
        "BLOCKED SUSPENDED: Ocurrencias %d  Tiempo: %" PRId64 "\n"
        "------------------------------------------- \n",
        process->pid,
        process->metricas_de_estado->new,
        process->metricas_de_tiempo->NEW ? temporal_gettime(process->metricas_de_tiempo->NEW) : 0,
        process->metricas_de_estado->ready,
        process->metricas_de_tiempo->READY ? temporal_gettime(process->metricas_de_tiempo->READY) : 0,
        process->metricas_de_estado->execute,
        process->metricas_de_tiempo->EXECUTE ? temporal_gettime(process->metricas_de_tiempo->EXECUTE) : 0,
        process->metricas_de_estado->blocked,
        process->metricas_de_tiempo->BLOCKED ? temporal_gettime(process->metricas_de_tiempo->BLOCKED) : 0,
        process->metricas_de_estado->ready_suspended,
        process->metricas_de_tiempo->READY_SUSPENDED ? temporal_gettime(process->metricas_de_tiempo->READY_SUSPENDED) : 0,
        process->metricas_de_estado->blocked_suspended,
        process->metricas_de_tiempo->BLOCKED_SUSPENDED ? temporal_gettime(process->metricas_de_tiempo->BLOCKED_SUSPENDED) : 0
    );

    log_info(logger, "%s", buffer);
}

void carnicero_de_instancias_io(t_IO_instancia* instancia_io){
    free(instancia_io);
}

void carnicero_de_io(t_IO* io){
    free(io->nombre);
    pthread_mutex_destroy(&io->instancias_IO->mutex);
    free(io->instancias_IO->cola);
    free(io->instancias_IO);
    pthread_mutex_destroy(&io->procesos_esperando->mutex);    
    free(io->procesos_esperando->cola);
    free(io->procesos_esperando);
    free(io);
}

void terminar_kernel(){
    
    log_debug(logger,"Liberando kernel...");

    // Destruir y liberar list_cpus
    pthread_mutex_destroy(&(list_cpus->mutex));
    list_destroy_and_destroy_elements(list_cpus->cola, free);
    free(list_cpus);

    // Destruir y liberar list_ios
    log_debug(logger,"Liberando IOS...");
    
    pthread_mutex_destroy(&(list_ios->mutex));
    int cant_ios = list_size(list_ios->cola);
    for(int i = 0; i < cant_ios; i++){
        log_debug(logger,"liberando IOS");
        t_IO* io = list_get(list_ios->cola, i);
        
        pthread_mutex_destroy(&io->instancias_IO->mutex);
        list_destroy_and_destroy_elements(io->instancias_IO->cola, free);

        pthread_mutex_destroy(&io->procesos_esperando->mutex);
        list_destroy_and_destroy_elements(io->procesos_esperando->cola, free);       
    }

    free(list_ios);

    log_debug(logger,"Liberando planificador...");
    // Destruir planner->queue_EXECUTE
    pthread_mutex_destroy(&(planner->queue_EXECUTE->mutex));
    list_destroy_and_destroy_elements(planner->queue_EXECUTE->cola, carnicero_final);
    free(planner->queue_EXECUTE);

    // SHORT TERM
    pthread_mutex_destroy(&(planner->short_term->queue_READY->mutex));
    list_destroy_and_destroy_elements(planner->short_term->queue_READY->cola, carnicero_final);
    free(planner->short_term->queue_READY);
    free(planner->short_term);

    // MEDIUM TERM
    pthread_mutex_destroy(&(planner->medium_term->queue_BLOCKED_SUSPENDED->mutex));
    list_destroy_and_destroy_elements(planner->medium_term->queue_BLOCKED_SUSPENDED->cola, carnicero_final);
    free(planner->medium_term->queue_BLOCKED_SUSPENDED);

    pthread_mutex_destroy(&(planner->medium_term->queue_READY_SUSPENDED->mutex));
    list_destroy_and_destroy_elements(planner->medium_term->queue_READY_SUSPENDED->cola, carnicero_final);
    free(planner->medium_term->queue_READY_SUSPENDED);

    free(planner->medium_term);

    // LONG TERM
    pthread_mutex_destroy(&(planner->long_term->queue_NEW->mutex));
    list_destroy_and_destroy_elements(planner->long_term->queue_NEW->cola, carnicero_final);
    free(planner->long_term->queue_NEW);

    pthread_mutex_destroy(&(planner->long_term->queue_EXIT->mutex));
    list_destroy_and_destroy_elements(planner->long_term->queue_EXIT->cola, carnicero_final);
    free(planner->long_term->queue_EXIT);

    pthread_mutex_destroy(&(planner->long_term->queue_BLOCKED->mutex));
    list_destroy_and_destroy_elements(planner->long_term->queue_BLOCKED->cola, carnicero_final);
    free(planner->long_term->queue_BLOCKED);

    free(planner->long_term);

    // Finalmente, liberar planner
    free(planner);

    // Destruir y liberar list_procesos
    pthread_mutex_destroy(&(list_procesos->mutex));
    free(list_procesos->cola);
    free(list_procesos);

    // Config  
    free(config_kernel->IP_MEMORIA);
    free(config_kernel->IP_SERVER);
    free(config_kernel->ALGORITMO_CORTO_PLAZO);
    free(config_kernel->ALGORITMO_INGRESO_A_READY);
    free(config_kernel->LOG_LEVEL);

    // Kernel-servers
    free(kernel_servers);

    // Finalmente liberar la estructura
    free(config_kernel);

    //HIlos globales
    pthread_mutex_destroy(&mutex_memoria);
    pthread_mutex_destroy(&mutex_desalojo);
    pthread_mutex_destroy(&mutex_io);
    pthread_mutex_destroy(&mutex_memoria);
    pthread_mutex_destroy(&mutex_memoria_envio_pid);
    pthread_mutex_destroy(&mutex_traer);
    pthread_mutex_destroy(&mutex_creacion_de_proceso);

    log_debug(logger,"Kernel liberado");
    pthread_mutex_unlock(&mutex_control_kernel);
    exit(EXIT_SUCCESS);
}   

void carnicero_final(void* proceso){
    log_debug(logger,"carnicero final INICIANDO");
    t_pcb* otro_proceso = (t_pcb*) proceso;
    carnicero(otro_proceso);
    log_debug(logger,"carnicero final TERMINADO");
}