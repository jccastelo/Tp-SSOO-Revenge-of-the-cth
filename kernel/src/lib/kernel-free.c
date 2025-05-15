
#include "../include/kernel-free.h"

void carnicero(t_pcb* process){

    pthread_mutex_lock(&process->queue_ESTADO_ACTUAL->mutex);
    list_remove_element(process->queue_ESTADO_ACTUAL->queue_ESTADO, process);
    pthread_mutex_unlock(&process->queue_ESTADO_ACTUAL->mutex);
    
    // aca van los logs
    free(process->archivo);    
    free(process->queue_ESTADO_ACTUAL);
    free(process->metricas_de_estado);

    free(process->metricas_de_tiempo->NEW);
    free(process->metricas_de_tiempo->READY);
    free(process->metricas_de_tiempo->EXECUTE);
    free(process->metricas_de_tiempo->BLOCKED);
    free(process->metricas_de_tiempo->BLOCKED_SUSPENDED);
    free(process->metricas_de_tiempo->READY_SUSPENDED);
    free(process->metricas_de_tiempo->metrica_actual);

    free(process->metricas_de_tiempo);    

    free(process);
}