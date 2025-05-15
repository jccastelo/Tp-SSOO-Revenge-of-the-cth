#ifndef KERNEL_PLANNER_H
#define KERNEL_PLANNER_H

#include "kernel.h"
#include "kernel-types.h"
#include "kernel-syscalls.h"

void planner_init();
int get_algoritm(char* algoritmo);
void queue_FIFO(t_pcb *process, t_list *queue);
void queue_process(t_pcb* process, int estado);
void traer_proceso_a_MP();
void init_fist_process(char *archivo_pseudocodigo,int Tamanio_proc);
#endif //KERNEL_PLANNER_H