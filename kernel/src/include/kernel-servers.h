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
#include "kernel-handlers.h"

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
void kernel_server_io_listening();


#endif // KERNEL_SERVERS_H