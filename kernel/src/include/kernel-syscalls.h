#ifndef KERNEL_SYSCALLS_H
#define KERNEL_SYSCALLS_H

// Incluyo las bibliotecas externas necesarias:
#include <utils/logger.h>
#include <utils/socket.h>
#include <utils/protocolo.h>
#include <utils/connection-strategy.h>
#include <utils/server.h>

// Incluyo las bibliotecas internas necesarias:
#include "kernel-planner.h"
#include "kernel-state.h"
#include "kernel-handlers.h"

void recibir_proceso(t_buffer *buffer ,int client_socket);
t_pcb *process_init();
void cargar_proceso(t_pcb* procces, t_buffer* buffer, int client_socket);

#endif //KERNEL_SYSCALLS_H