#ifndef KERNEL_CPUS_CONNECTIONS_H
#define KERNEL_CPUS_CONNECTIONS_H

#include "kernel-planner.h"
#include "kernel-state.h"
#include "kernel-types.h"
#include "kernel-escucha.h"

t_cpu *cpu_init();
void iniciar_cpu(t_buffer *buffer,int socket_cpu);
void set_cpu(int cpu_socket_buscado,int estado_nuevo);

#endif //KERNEL_CPUS_CONNECTIONS_Hs