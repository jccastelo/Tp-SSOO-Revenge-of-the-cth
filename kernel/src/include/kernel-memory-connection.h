#ifndef KERNEL_MEMORY_CONNECTION_H
#define KERNEL_MEMORY_CONNECTION_H

// Incluye las bibliotecas necesarias externas:
#include <utils/logger.h>
#include <utils/socket.h>
#include <utils/connection.h>
#include <commons/string.h>
#include <utils/protocolo.h>
#include <stdio.h>


// Incluyo las bibliotecas necesarias internas:
#include "kernel.h"
#include "kernel-state.h"

void kernel_memory_connection(void);
void set_socket_memoria(int);

int solicitar_a_memoria(int (*operacion)(t_pcb* proceso), t_pcb* proceso_a_enviar);

int memoria_init_proc(t_pcb* proceso);
int memoria_delete_process(t_pcb* proceso);
int avisar_dump_memory(t_pcb* proceso);
int suspender_proceso(t_pcb* proceso);
int desuspender_proceso(t_pcb* proceso);
#endif // KERNEL_MEMORY_CONNECTION_H