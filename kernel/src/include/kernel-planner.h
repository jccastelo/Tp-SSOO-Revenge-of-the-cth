#ifndef KERNEL_PLANNER_H
#define KERNEL_PLANNER_H

#include "kernel.h"
#include "kernel-types.h"
#include "kernel-syscalls.h"

void planner_init();
int get_algoritm(char* algoritmo);
void queue_process(t_pcb* process, int estado);
void cambiar_estado(void (*algoritmo_planificador)(t_pcb* process, t_list* estado), t_pcb* process, t_monitor* sgteEstado);
void actualizarTiempo(t_temporal **metrica_actual,t_temporal **metricas_de_tiempo_estado);
void queue_FIFO(t_pcb *process, t_list *queue);
void traer_proceso_a_MP();
void init_fist_process(char *archivo_pseudocodigo,int Tamanio_proc);
void queue_PMCP(t_pcb *process, t_list *lista);
#endif //KERNEL_PLANNER_H