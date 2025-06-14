#ifndef KERNEL_BLOCKED_H
#define KERNEL_BLOCKED_H

#include "kernel.h"
#include "kernel-types.h"
#include "kernel-syscalls.h"
#include "kernel-free.h"
#include "kernel-planner.h"

void bloquearProceso(t_pcb *process);
void* timer_de_blockeo(void* arg);
bool condicionProcesoEnLista(void* elemento);
#endif // KERNEL_BLOCKED_H