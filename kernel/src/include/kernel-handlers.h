#ifndef KERNEL_HANDLERS_H
#define KERNEL_HANDLERS_H

// Incluyo las bibliotecas externas necesarias:
#include <utils/logger.h>
#include <utils/protocolo.h>

// Incluyo las bibliotecas internas necesarias:

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
void kernel_server_io_handler(int client_socket, int operation, char *server_name);

#endif // KERNEL_HANDLERS_H