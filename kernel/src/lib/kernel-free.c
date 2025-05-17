
#include "../include/kernel-free.h"

void carnicero(t_pcb* process){

    pthread_mutex_lock(&process->queue_ESTADO_ACTUAL->mutex);
    list_remove_element(process->queue_ESTADO_ACTUAL->queue_ESTADO, process);
    pthread_mutex_unlock(&process->queue_ESTADO_ACTUAL->mutex);
    
    // aca van los logs
    log_metricas(process);

    free(process->archivo);    
    free(process->queue_ESTADO_ACTUAL);
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
    log_info(logger,"Fin de proceso (PID): %d",process->pid);
    log_info(logger, "Métricas por estado:");

    log_info(logger, "NEW: Ocurrencias %d  Tiempo: %" PRId64,
            process->metricas_de_estado->new,
            process->metricas_de_tiempo->NEW ? temporal_gettime(process->metricas_de_tiempo->NEW) : 0);

    log_info(logger, "READY: Ocurrencias %d  Tiempo: %" PRId64,
            process->metricas_de_estado->ready,
            process->metricas_de_tiempo->READY ? temporal_gettime(process->metricas_de_tiempo->READY) : 0);

    log_info(logger, "EXECUTE: Ocurrencias %d  Tiempo: %" PRId64,
            process->metricas_de_estado->execute,
            process->metricas_de_tiempo->EXECUTE ? temporal_gettime(process->metricas_de_tiempo->EXECUTE) : 0);

    log_info(logger, "BLOCKED: Ocurrencias %d  Tiempo: %" PRId64,
            process->metricas_de_estado->blocked,
            process->metricas_de_tiempo->BLOCKED ? temporal_gettime(process->metricas_de_tiempo->BLOCKED) : 0);

    log_info(logger, "READY SUSPENDED: Ocurrencias %d  Tiempo: %" PRId64,
            process->metricas_de_estado->ready_suspended,
            process->metricas_de_tiempo->READY_SUSPENDED ? temporal_gettime(process->metricas_de_tiempo->READY_SUSPENDED) : 0);

    log_info(logger, "BLOCKED SUSPENDED: Ocurrencias %d  Tiempo: %" PRId64,
            process->metricas_de_estado->blocked_suspended,
            process->metricas_de_tiempo->BLOCKED_SUSPENDED ? temporal_gettime(process->metricas_de_tiempo->BLOCKED_SUSPENDED) : 0);
}