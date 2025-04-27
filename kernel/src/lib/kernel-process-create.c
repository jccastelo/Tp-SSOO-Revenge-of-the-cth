#include "../include/kernel-process-create.h"

void recibir_proceso(t_buffer *buffer ,int client_socket ){

    t_pcb *process =  process_init();

}

t_pcb *process_init(){

    t_pcb *new_process = malloc(sizeof(t_pcb));
    new_process->metricas_de_estado = malloc(sizeof(t_metricas_de_estados));
    new_process->metricas_de_tiempo= malloc(sizeof(t_metricas_de_tiempo));

    new_process->process_name =NULL;
    new_process->tamano_proceso = 0;
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


