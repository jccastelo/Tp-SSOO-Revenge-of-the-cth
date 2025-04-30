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

void recibir_proceso(t_buffer *buffer ,int client_socket);
t_pcb *process_init();
void cargar_proceso(t_pcb* procces, t_buffer* buffer, int client_socket);
void queue_process(t_pcb* process, int estado);
void actualizarTiempo(t_temporal **metrica_actual,t_temporal **metricas_de_tiempo_estado);
void queue_FIFO(t_pcb *process, t_queue *queue);

#endif //KERNEL_PROCESS_CREATE_H