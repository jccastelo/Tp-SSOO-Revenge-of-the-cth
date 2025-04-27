#ifndef  MEMORIA_SERVER_STARTUP_H
#define  MEMORIA_SERVER_STARTUP_H

// Incluyo las bibliotecas externas necesarias:
#include <utils/logger.h>
#include <utils/socket.h>
#include <utils/protocolo.h>
#include <utils/connection-strategy.h>
#include <utils/server.h>

// Incluyo las bibliotecas internas necesarias:
#include "memoria-state.h"
#include "memoria-handlers.h"

/**
 * @brief Inicia el proceso de escucha de conexiones entrantes en el servidor de Memoria.
 * 
 * Esta función se encarga de poner en marcha el servidor de Memoria para que comience a escuchar las 
 * solicitudes entrantes de los clientes. Mantiene el servidor en un estado de espera activa, en el cual 
 * puede aceptar conexiones de clientes que deseen interactuar con el servidor de Memoria.
 * 
 * El servidor permanecerá escuchando hasta que se reciba una señal de interrupción o se cierre el 
 * servicio.
 */
void memoria_servers_listening(void);

#endif // MEMORIA_SERVER_STARTUP_H