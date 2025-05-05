#ifndef KERNEL_MEMORY_CONNECTION_H
#define KERNEL_MEMORY_CONNECTION_H

// Incluye las bibliotecas necesarias externas:
#include <utils/logger.h>
#include <utils/socket.h>
#include <utils/connection.h>
#include <commons/string.h>
#include <stdio.h>


// Incluyo las bibliotecas necesarias internas:
#include "kernel.h"
#include "kernel-state.h"

void kernel_memory_connection(void);
void set_socket_memoria(int);
char* memoria_init_proc(t_pcb* process);
void* kernel_wait_init_proc(t_pcb* process);

#endif // KERNEL_MEMORY_CONNECTION_H