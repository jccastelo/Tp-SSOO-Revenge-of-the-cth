#ifndef KERNEL_SERVERS_H
#define KERNEL_SERVERS_H

// Incluyo las bibliotecas externas necesarias:
#include <utils/logger.h>
#include <utils/socket.h>
#include <utils/protocolo.h>
#include <utils/connection-strategy.h>
#include <utils/server.h>

// Incluyo las bibliotecas internas necesarias:
#include "kernel-state.h"
#include "kernel-escucha.h"

/**
 * @brief Inicia la escucha de los servidores principales del kernel.
 * 
 * Esta función lanza todos los servidores que forman parte del núcleo del sistema, 
 * incluyendo aquellos responsables de manejar interrupciones, operaciones de entrada/salida, 
 * y otras funcionalidades centrales. Es utilizada durante el proceso de inicialización 
 * del sistema para que todos los componentes del kernel estén preparados para aceptar 
 * conexiones entrantes y procesar solicitudes.
 */
void kernel_servers_listening();

/**
 * @brief Inicia la escucha del servidor encargado de manejar operaciones de entrada/salida (IO).
 * 
 * Esta función inicia el servidor específico del kernel que se encarga de procesar 
 * solicitudes relacionadas con dispositivos de entrada/salida. Es responsable de 
 * atender los pedidos de los procesos que requieren acceso a recursos como discos, 
 * terminales u otros periféricos, y puede utilizar mecanismos de planificación o espera 
 * según la lógica definida en el sistema.
 */
void* kernel_server_io_listening(void *_);

/**
 * @brief Inicia la escucha del servidor encargado de manejar las operaciones de planificación (dispatch).
 * 
 * Esta función lanza el servidor que recibe y procesa las solicitudes de dispatch enviadas al kernel.
 * Su principal responsabilidad es gestionar la asignación de procesos o hilos a la CPU, respetando 
 * las políticas de planificación definidas por el sistema operativo. 
 * 
 * Es fundamental para el correcto funcionamiento del planificador del sistema, ya que coordina la 
 * ejecución de los procesos en función de su prioridad, estado y otros criterios.
 * 
 * @param _ Argumento genérico requerido por la firma de pthread (no utilizado).
 * @return Siempre retorna NULL al finalizar.
 */
void* kernel_server_dispatcher_listening(void *_);

/**
 * @brief Inicia la escucha del servidor encargado de manejar interrupciones en el kernel.
 * 
 * Esta función inicia el servidor dedicado a atender eventos de interrupción dentro del sistema. 
 * Las interrupciones pueden provenir de distintas fuentes, como señales externas, finalización de 
 * procesos, o necesidades de replanificación inmediata.
 * 
 * Su propósito es asegurar que el kernel pueda responder en tiempo y forma a eventos que requieren 
 * atención urgente, manteniendo la coherencia y estabilidad del sistema.
 * 
 * @param _ Argumento genérico requerido por la firma de pthread (no utilizado).
 * @return Siempre retorna NULL al finalizar.
 */
void* kernel_server_interrupt_listening(void *_);

#endif // KERNEL_SERVERS_H