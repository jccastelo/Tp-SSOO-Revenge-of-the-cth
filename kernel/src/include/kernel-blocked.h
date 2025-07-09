#ifndef KERNEL_BLOCKED_H
#define KERNEL_BLOCKED_H

#include "kernel.h"
#include "kernel-types.h"
#include "kernel-syscalls.h"
#include "kernel-free.h"
#include "kernel-planner.h"

void bloquearProceso(t_pcb *process);
void* timer_de_blockeo(void* arg);
bool proceso_esta_en_lista(t_list* lista, t_pcb* proceso);
void limpiar_hilo_activo(void* arg);
void liberar_mutex(void* arg) ;
#endif // KERNEL_BLOCKED_H