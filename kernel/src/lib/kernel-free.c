
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
    free(io->instancias_IO);
    free(io->procesos_esperando);
    free(io);
}

void terminar_kernel(){
    
    //liberar planner
    if (!planner) return;

    if (planner->short_term) {
        liberar_monitor(planner->short_term->queue_READY);
        free(planner->short_term);
    }

    if (planner->medium_term) {
        liberar_monitor(planner->medium_term->queue_BLOCKED_SUSPENDED);
        liberar_monitor(planner->medium_term->queue_READY_SUSPENDED);
        free(planner->medium_term);
    }

    if (planner->long_term) {
        liberar_monitor(planner->long_term->queue_NEW);
        liberar_monitor(planner->long_term->queue_BLOCKED);
        liberar_monitor(planner->long_term->queue_EXIT);
        free(planner->long_term);
    }

    liberar_monitor(planner->queue_EXECUTE);

    free(planner);

    //liberar listas cpus, procesos e io
    liberar_monitor(list_cpus);    
    liberar_monitor(list_procesos);
    liberar_monitor(list_ios);    

    //liberar conexiones
    close(socket_memoria);
    //liberar lo que haya quedado

}   

void liberar_monitor(t_monitor* monitor) {
    if (!monitor) return;

    if (monitor->cola) {
        list_destroy_and_destroy_elements(monitor->cola, free);
    }

    pthread_mutex_destroy(&monitor->mutex);
    free(monitor);
}