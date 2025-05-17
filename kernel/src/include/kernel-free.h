#ifndef KERNEL_FREE_H
#define KERNEL_FREE_H

#include "kernel.h"
#include "kernel-types.h"
#include "kernel-syscalls.h"
#include "inttypes.h"

void carnicero(t_pcb* process);
void log_metricas(t_pcb* process);
#endif //KERNEL_FREE_H