#include "../include/kernel-process-create.h"


void recibir_proceso(t_buffer *buffer ,int client_socket){

    t_pcb *process =  process_init();

    cargar_proceso(process, buffer, client_socket);

    queue_process(process, NEW);
}

t_pcb *process_init(){

    t_pcb *new_process = malloc(sizeof(t_pcb));
    new_process->metricas_de_estado = malloc(sizeof(t_metricas_de_estados));
    new_process->metricas_de_tiempo = malloc(sizeof(t_metricas_de_tiempo));

    new_process->process_name =NULL;
    new_process->tamanio_proceso = 0;
    new_process->pid = 0;
    new_process->pc = 0;
    
    new_process->metricas_de_estado->new= 0;
    new_process->metricas_de_estado->ready= 0;
    new_process->metricas_de_estado->execute= 0;
    new_process->metricas_de_estado->blocked= 0;
    new_process->metricas_de_estado->blocked_suspended= 0;
    new_process->metricas_de_estado->ready_suspended= 0; 

    //Las metricas de tiempo se inicializan cuando el proceso entra por primera vez en cada estado
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

void queue_process(t_pcb* process, int estado){

    switch(estado)
    {
    case NEW:
        process->metricas_de_estado->new += 1;
        actualizarTiempo(process->metricas_de_tiempo->new);
        planner->long_term->algoritmo_planificador(process, planner->long_term->queue_NEW);
        // Gestion de memoria
        break;

    case READY:
        process->metricas_de_estado->ready += 1;
        planner->short_term->algoritmo_planificador(process, planner->short_term->queue_READY);
        break;

    case EXECUTE:
        process->metricas_de_estado->execute += 1;

        // Aca entra el dispatcher 
        break;

    case BLOCKED:
        process->metricas_de_estado->blocked += 1;

        planner->short_term->algoritmo_planificador(process, planner->short_term->queue_READY);
        break;

    case EXIT:
        //process->metricas_de_estado->exit += 1;
        // carnuicero();
        break;

    case BLOCKED_SUSPENDED:
        //process->metricas_de_estado->blocked_suspended += 1;
        break;

    case READY_SUSPENDED:
        //process->metricas_de_estado->ready_suspended += 1;
        break;
    }
}

void actualizarTiempo(t_temporal *metricas_de_tiempo_estado){
    
    if( metricas_de_tiempo_estado == 0)
    {
        metricas_de_tiempo_estado= temporal_create();
    }else{

        temporal_resume(metricas_de_tiempo_estado); 
    }
}


void queue_FIFO(t_pcb *process, t_queue *queue)
{
    queue_push(queue,process);
}