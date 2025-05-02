#include "../include/kernel-syscalls.h"


void recibir_proceso(t_buffer *buffer ,int client_socket){

    t_pcb *process =  process_init();

    cargar_proceso(process, buffer, client_socket);

    queue_process(process, NEW);
}

t_pcb *process_init(){

    t_pcb *new_process = malloc(sizeof(t_pcb));
    new_process->metricas_de_estado = malloc(sizeof(t_metricas_de_estados));
    new_process->metricas_de_tiempo = malloc(sizeof(t_metricas_de_tiempo));

    new_process->process_name = NULL;
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

    return new_process;
}

void cargar_proceso(t_pcb* process, t_buffer* buffer, int client_socket){
    
    static int pid_unico = 0;

	int desplazamiento = 0;
    
    // Copiamos el tamanio del nombre
    int tamanio_nombre;
    memcpy(&tamanio_nombre, buffer + desplazamiento, sizeof(int));

    // Copiamos el nombre del archivo
    memcpy(process->process_name, buffer + desplazamiento, tamanio_nombre);
    desplazamiento+=tamanio_nombre;
    
    // Copiamos el tamanio de arcivo
    memcpy(&process->tamanio_proceso, buffer+desplazamiento, sizeof(int));
    desplazamiento += sizeof(int);

    process->pid = pid_unico;
    pid_unico += 1;

    if(desplazamiento < buffer->size)
    { 
        log_info(logger,"Hay informacion sin deserializar en INIC_PROC"); 
    }
    else{ log_info(logger,"Se inicializo "); }

}