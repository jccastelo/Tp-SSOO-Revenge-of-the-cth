#include "../include/kernel-syscalls.h"

t_pcb *process_init(){

    t_pcb *new_process = malloc(sizeof(t_pcb));
    new_process->metricas_de_estado = malloc(sizeof(t_metricas_de_estados));
    new_process->metricas_de_tiempo = malloc(sizeof(t_metricas_de_tiempo));
    new_process->estimaciones_SJF = malloc(sizeof(t_SJF));

    new_process->estimaciones_SJF->rafagaEstimada = (int64_t)config_kernel->ESTIMACION_INICIAL;
    new_process->estimaciones_SJF->ultimaEstimacion = (int64_t)config_kernel->ESTIMACION_INICIAL;
    new_process->estimaciones_SJF->rafagaReal = NULL;
    new_process->estimaciones_SJF->rafagaTotalReal = 0;
    
    new_process->archivo = NULL;
    new_process->tamanio_proceso = 0;
    new_process->pid = 0;
    new_process->pc = 0;
    new_process->queue_ESTADO_ACTUAL = NULL;
    
    new_process->metricas_de_estado->new = 0;
    new_process->metricas_de_estado->ready = 0;
    new_process->metricas_de_estado->execute = 0;
    new_process->metricas_de_estado->blocked = 0;
    new_process->metricas_de_estado->blocked_suspended = 0;
    new_process->metricas_de_estado->ready_suspended = 0;
    new_process->metricas_de_estado->exit = 0;

    new_process->metricas_de_tiempo->NEW = NULL;
    new_process->metricas_de_tiempo->READY = NULL;
    new_process->metricas_de_tiempo->EXECUTE = NULL;
    new_process->metricas_de_tiempo->BLOCKED = NULL;
    new_process->metricas_de_tiempo->BLOCKED_SUSPENDED = NULL;
    new_process->metricas_de_tiempo->READY_SUSPENDED = NULL;
    new_process->metricas_de_tiempo->metrica_actual = NULL;

    new_process->metricas_de_tiempo->NEW = temporal_create(); 
    temporal_stop(new_process->metricas_de_tiempo->NEW);
    new_process->metricas_de_tiempo->READY = temporal_create();
    temporal_stop(new_process->metricas_de_tiempo->READY);
    new_process->metricas_de_tiempo->EXECUTE = temporal_create();
    temporal_stop(new_process->metricas_de_tiempo->EXECUTE); 
    new_process->metricas_de_tiempo->BLOCKED = temporal_create();
    temporal_stop(new_process->metricas_de_tiempo->BLOCKED);
    new_process->metricas_de_tiempo->BLOCKED_SUSPENDED = temporal_create();
    temporal_stop(new_process->metricas_de_tiempo->BLOCKED_SUSPENDED); 
    new_process->metricas_de_tiempo->READY_SUSPENDED = temporal_create(); 
    temporal_stop(new_process->metricas_de_tiempo->READY_SUSPENDED);
    new_process->metricas_de_tiempo->metrica_actual = temporal_create(); 
    temporal_stop(new_process->metricas_de_tiempo->metrica_actual);

    new_process->estimaciones_SJF->rafagaReal = temporal_create(); 
    temporal_stop(new_process->estimaciones_SJF->rafagaReal);
    
    return new_process;
}

void recibir_y_crear_proceso(t_buffer *buffer){

    t_pcb *process =  process_init();

    cargar_proceso(process, buffer);

    queue_process(process, NEW);
}

void cargar_proceso(t_pcb* process, t_buffer* buffer){
    
    log_warning(logger, "CARGANDO PROCESO");
	int desplazamiento = 0;
    // Copiamos el tamanio del nombre
    int tamanio_nombre=0;
    memcpy(&tamanio_nombre, buffer->stream + desplazamiento, sizeof(int));
    desplazamiento += sizeof(int);

    // Copiamos el nombre del archivo
    process->archivo = malloc(tamanio_nombre + 1);
    memcpy(process->archivo, buffer->stream+  desplazamiento, tamanio_nombre);
    process->archivo[tamanio_nombre] = '\0'; 
    desplazamiento+=tamanio_nombre;
    
    // Copiamos el tamanio de proceso
    memcpy(&process->tamanio_proceso, buffer->stream+desplazamiento, sizeof(int));
    desplazamiento += sizeof(int);

    pthread_mutex_lock(&list_procesos->mutex);
    process->pid = list_add(list_procesos->cola,process);
    pthread_mutex_unlock(&list_procesos->mutex);

    if(desplazamiento < buffer->size) 
    {log_error(logger,"Hay informacion sin deserializar en INIC_PROC"); }
    else{ log_info(logger,"Se inicializo proceso PID: %d ",process->pid ); }
    sleep(1);

}

void delate_process(t_buffer *buffer){


    int pid_delate; //Solo necesito el pid del proceso

    memcpy(&pid_delate, buffer->stream, sizeof(int)); 

    t_pcb *process = list_get(list_procesos->cola, pid_delate); //Obtengo el proceso a eliminar de la lista global

    temporal_stop(process->estimaciones_SJF->rafagaReal);

    queue_process(process,EXIT); 
}