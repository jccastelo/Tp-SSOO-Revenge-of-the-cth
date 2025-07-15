#ifndef KERNEL_PLANNER_H
#define KERNEL_PLANNER_H

#include "kernel.h"
#include "kernel-types.h"
#include "kernel-syscalls.h"
#include "kernel-cpus-connections.h"

void planner_init();
int get_algoritm(char* algoritmo);
void queue_process(t_pcb* process, int estado);
void cambiar_estado(void (*algoritmo_planificador)(t_pcb* process, t_list* estado), t_pcb* process, t_monitor* sgteEstado);
void actualizarTiempo(t_temporal **metrica_actual,t_temporal **metricas_de_tiempo_estado);
void queue_FIFO(t_pcb *process, t_list *queue);
void traer_proceso_a_MP();
void queue_PMCP(t_pcb *process, t_list *lista);
void queue_SJF(t_pcb *process, t_list *lista);
void sin_desalojo(t_pcb *process);
void desalojo_SJF(t_pcb *process);
void init_fist_process(char *archivo_pseudocodigo,int Tamanio_proc);
t_cpu* cpu_mayor_rafaga();
void mandar_procesos_a_execute();
int64_t max(int64_t a, int64_t b);
void actualizar_rafagas_sjf(t_pcb* proceso);

extern pthread_mutex_t mutex_traer;
#endif //KERNEL_PLANNER_H