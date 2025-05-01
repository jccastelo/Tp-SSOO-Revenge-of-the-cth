#ifndef KERNEL_PLANNER_H
#define KERNEL_PLANNER_H

#include "kernel.h"
#include "kernel-syscalls.h"

void planner_init();
int get_algoritm(char* algoritmo);
void queue_process(t_pcb* process, int estado);
void cambiar_estado(void (*algoritmo_planificador)(t_pcb* process, t_queue* estado), t_pcb* process, t_mutex_queue* sgteEstado);
void actualizarTiempo(t_temporal **metrica_actual,t_temporal **metricas_de_tiempo_estado);
void queue_FIFO(t_pcb *process, t_queue *queue);

#endif //KERNEL_PLANNER_H