#ifndef KERNEL_CPUS_CONNECTIONS_H
#define KERNEL_CPUS_CONNECTIONS_H

#include "kernel-planner.h"
#include "kernel-state.h"
#include "kernel-escucha.h"

t_cpu *cpu_init();
void iniciar_cpu(t_buffer *buffer,int socket_cpu);

#endif //KERNEL_CPUS_CONNECTIONS_H