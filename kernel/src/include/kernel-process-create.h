#ifndef KERNEL_PROCESS_CREATE_H
#define KERNEL_PROCESS_CREATE_H

// Incluyo las bibliotecas externas necesarias:
#include <utils/logger.h>
#include <utils/socket.h>
#include <utils/protocolo.h>
#include <utils/connection-strategy.h>
#include <utils/server.h>

// Incluyo las bibliotecas internas necesarias:
#include "kernel-state.h"
#include "kernel-handlers.h"

void recibir_proceso(t_buffer *buffer ,int client_socket );
t_pcb *procces_init();

#endif //KERNEL_PROCESS_CREATE_H