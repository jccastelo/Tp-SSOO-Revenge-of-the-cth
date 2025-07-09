#ifndef KERNEL_FREE_H
#define KERNEL_FREE_H

#include "kernel.h"
#include "kernel-types.h"
#include "kernel-syscalls.h"
#include "inttypes.h"

void carnicero(t_pcb* process);
void log_metricas(t_pcb* process);

void carnicero_de_instancias_io(t_IO_instancia* instancia_io);
void carnicero_de_io(t_IO* IO);
void terminar_kernel();
void liberar_monitor(t_monitor* monitor);
#endif //KERNEL_FREE_H