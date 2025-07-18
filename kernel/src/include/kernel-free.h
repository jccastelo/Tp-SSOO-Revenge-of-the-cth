#ifndef KERNEL_FREE_H
#define KERNEL_FREE_H

#include "kernel.h"
#include "kernel-types.h"
#include "kernel-syscalls.h"
#include "inttypes.h"
#include "kernel-cambios-de-estado.h"

void carnicero(t_pcb* process);
void log_metricas(t_pcb* process);

void carnicero_de_instancias_io(t_IO_instancia* instancia_io);
void carnicero_de_io(t_IO* IO);
void terminar_kernel();
void carnicero_final(void * proceso);

extern pthread_mutex_t mutex_eliminando_proceso;
#endif //KERNEL_FREE_H