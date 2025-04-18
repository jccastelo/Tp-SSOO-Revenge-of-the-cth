#ifndef KERNEL_CONNECTION_H
#define KERNEL_CONNECTION_H

// Incluye las bibliotecas necesarias externas:
#include <utils/logger.h>
#include <utils/socket.h>
#include <utils/connection.h>
#include <commons/string.h>

// Incluyo las bibliotecas necesarias internas:
#include "kernel-state.h"

/**
 * @brief Inicia un servidor en la IP y puerto especificados.
 * 
 * Crea un socket, lo enlaza a la IP y puerto dados, y lo deja escuchando conexiones entrantes.
 * 
 * @param name Nombre lógico del servidor (solo para logs o mensajes).
 * @param ip IP en la cual el servidor escuchará conexiones (puede ser "0.0.0.0" para todas).
 * @param puerto Puerto en el cual el servidor escuchará conexiones.
 * @return Devuelve el descriptor del socket del servidor, o -1 en caso de error.
 */
void kernel_servers_init(void);

/**
 * @brief Asigna el ID del servidor al campo DISPATCH de la estructura kernel_servers.
 * 
 * Esta función se utiliza como callback para guardar el ID del servidor responsable 
 * del despacho de procesos.
 * 
 * @param id_server ID del servidor asignado.
*/
void set_dispatcher_id (int id_server);
 
/**
 * @brief Asigna el ID del servidor al campo IO de la estructura kernel_servers.
 * 
 * Esta función se utiliza como callback para guardar el ID del servidor responsable 
 * de operaciones de entrada/salida.
 * 
 * @param id_server ID del servidor asignado.
*/
void set_io_id (int id_server);

/**
 * @brief Asigna el ID del servidor al campo INTERRUPT de la estructura kernel_servers.
 * 
 * Esta función se utiliza como callback para guardar el ID del servidor encargado 
 * de manejar interrupciones.
 * 
 * @param id_server ID del servidor asignado.
*/
void set_interrupt_id(int id_server);

#endif /* KERNEL_CONNECTION_H */
