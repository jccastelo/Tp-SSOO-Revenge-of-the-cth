#ifndef KERNEL_ESCUCHA_H
#define KERNEL_ESCUCHA_H

// Incluyo las bibliotecas externas necesarias:
#include <utils/logger.h>
#include <utils/protocolo.h>

// Incluyo las bibliotecas internas necesarias:
#include "kernel-escucha.h"
#include "kernel-cpus-connections.h"
#include "kernel-syscalls.h"
#include "kernel-io-connections.h"


/**
 * @brief Maneja una operación de entrada/salida solicitada al servidor de I/O del kernel.
 * 
 * Esta función es responsable de atender las solicitudes de entrada/salida enviadas por
 * un cliente al servidor de I/O del kernel. Recibe un código de operación, ejecuta la 
 * lógica correspondiente y puede utilizar el nombre lógico del servidor para fines de 
 * trazabilidad, monitoreo o logging.
 * 
 * Generalmente, esta función se utiliza como `client_handler` dentro de una estrategia 
 * de conexión (persistente o por única vez), y permite centralizar la lógica de atención 
 * de operaciones I/O dentro del núcleo del sistema.
 * 
 * @param client_socket Descriptor del socket del cliente que envió la solicitud.
 * @param operation Código numérico que representa la operación de I/O a ejecutar.
 * @param server_name Nombre lógico del servidor, útil para logs o identificación en el sistema.
 */

/**
 * @brief Maneja una operación de entrada/salida solicitada al servidor de I/O del kernel.
 * 
 * Esta función es responsable de atender las solicitudes de entrada/salida enviadas por
 * un cliente al servidor de I/O del kernel. Recibe un código de operación, ejecuta la 
 * lógica correspondiente y puede utilizar el nombre lógico del servidor para fines de 
 * trazabilidad, monitoreo o logging.
 * 
 * Generalmente, esta función se utiliza como `client_handler` dentro de una estrategia 
 * de conexión (persistente o por única vez), y permite centralizar la lógica de atención 
 * de operaciones I/O dentro del núcleo del sistema.
 * 
 * @param client_socket Descriptor del socket del cliente que envió la solicitud.
 * @param operation Código numérico que representa la operación de I/O a ejecutar.
 * @param server_name Nombre lógico del servidor, útil para logs o identificación en el sistema.
 */
void kernel_server_io_handler(int client_socket, int operation, const char *server_name);

/**
 * @brief Maneja una operación enviada al servidor de dispatch del kernel.
 * 
 * Esta función es responsable de procesar solicitudes relacionadas con la planificación
 * y asignación de procesos o hilos dentro del núcleo del sistema. A partir del código 
 * de operación recibido, ejecuta la lógica de dispatch correspondiente.
 * 
 * Se utiliza típicamente como `client_handler` en una estrategia de servidor, y permite
 * desacoplar la lógica de planificación del resto de las operaciones del kernel.
 * 
 * @param client_socket Descriptor del socket del cliente que envió la solicitud.
 * @param operation Código numérico que representa la operación de dispatch a ejecutar.
 * @param server_name Nombre lógico del servidor, útil para logs o trazabilidad.
 */
void kernel_server_dispatch_handler(int client_socket, int operation, const char *server_name);

/**
 * @brief Maneja una operación enviada al servidor de interrupciones del kernel.
 * 
 * Esta función atiende solicitudes relacionadas con interrupciones, como cambios de
 * contexto, señales de finalización o interrupciones externas que requieren atención 
 * inmediata del núcleo del sistema.
 * 
 * Se utiliza como `client_handler` dentro del servidor de interrupciones y permite
 * centralizar la gestión de eventos asincrónicos o urgentes dentro del kernel.
 * 
 * @param client_socket Descriptor del socket del cliente que envió la solicitud.
 * @param operation Código numérico que representa el tipo de interrupción o evento.
 * @param server_name Nombre lógico del servidor, útil para logs o identificación del contexto.
 */
void kernel_server_interrupt_handler(int client_socket, int operation, const char *server_name);

#endif // KERNEL_ESCUCHA_H