#ifndef KERNEL_CAMBIOS_DE_ESTADO_H
#define KERNEL_CAMBIOS_DE_ESTADO_H

#include <pthread.h>

#include "kernel.h"
#include "kernel-types.h"
#include "kernel-syscalls.h"
#include "kernel-free.h"
#include "kernel-planner.h"
#include "kernel-blocked.h"

void queue_process(t_pcb* process, int estado);
void cambiar_estado(void (*algoritmo_planificador)(t_pcb* process, t_list* estado), t_pcb* process, t_monitor* sgteEstado);
void actualizarTiempo(t_temporal **metrica_actual,t_temporal **metricas_de_tiempo_estado);
void bloquearProceso(t_pcb* process);
char *get_NombreDeEstado(t_monitor* queue_ESTADO);
#endif //KERNEL_CAMBIOS_DE_ESTADO_H