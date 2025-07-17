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
#include "kernel-escucha.h"

void recibir_y_crear_proceso(t_buffer *buffer);
t_pcb *process_init();
void cargar_proceso(t_pcb* procces, t_buffer* buffer);

void delate_process(t_buffer *buffer);

extern pthread_mutex_t creando_proceso;
#endif //KERNEL_SYSCALLS_H