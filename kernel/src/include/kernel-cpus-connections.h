#ifndef KERNEL_CPUS_CONNECTIONS_H
#define KERNEL_CPUS_CONNECTIONS_H

#include "kernel-planner.h"
#include "kernel-state.h"
#include "kernel-types.h"
#include "kernel-escucha.h"

t_cpu *cpu_init();
void iniciar_cpu(t_buffer *buffer,int socket_cpu, int dispatch_o_interrupt);
void set_cpu(int cpu_socket_buscado,int estado_nuevo);
t_pcb* recibir_proceso(t_buffer* buffer);
t_cpu* buscar_cpu_disponible();
void enviar_proceso_cpu(int cpu_socket, t_pcb* process);
void desalojar_proceso(t_cpu* cpu);

#endif //KERNEL_CPUS_CONNECTIONS_Hs