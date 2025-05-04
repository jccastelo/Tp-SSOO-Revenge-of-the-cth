#ifndef MEMORIA_SERVER_H
#define MEMORIA_SERVER_H

// Incluye las bibliotecas necesarias externas:
#include <utils/logger.h>
#include <utils/socket.h>
#include <utils/connection.h>
#include <commons/string.h>

// Incluyo las bibliotecas necesarias internas:
#include "memoria-state.h"

/**
 * @brief Inicializa la estructura de servidores para el módulo Memoria.
 *
 * Esta función reserva memoria para la estructura `memoria_servers` y configura el servidor
 * de escucha principal utilizando los datos de configuración cargados previamente.
 * El servidor se inicia con la IP `127.0.0.1` y el puerto definido en `config_memoria->PUERTO_ESCUCHA`.
 * Además, se pasa una función de callback (`set_escucha_id`) para asignar el identificador
 * de servidor correspondiente una vez que la conexión está lista.
 */
void memoria_servers_init(void);

/**
 * @brief Establece el ID del servidor de escucha.
 * 
 * Esta función se utiliza para configurar el identificador único del servidor encargado de escuchar las 
 * conexiones entrantes, permitiendo que el servidor identifique y maneje sus conexiones de manera 
 * adecuada según su ID asignado.
 *
 * @param id_server El identificador único del servidor que se usará para la escucha.
 */
void set_escucha_id(int id_server);

/**
 * @brief Establece el ID del servidor de escucha.
 * 
 * Esta función se utiliza para configurar el identificador único del servidor encargado de escuchar las 
 * conexiones entrantes, permitiendo que el servidor identifique y maneje sus conexiones de manera 
 * adecuada según su ID asignado.
 *
 * @param id_server El identificador único del servidor que se usará para la escucha.
 */
void set_escucha_id(int id_server);

#endif // MEMORIA_SERVER_H