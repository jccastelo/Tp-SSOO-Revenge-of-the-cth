#ifndef MEMORIA_HANDLERS_H
#define MEMORIA_HANDLERS_H

// Incluyo las bibliotecas externas necesarias:
#include <utils/logger.h>
#include <utils/protocolo.h>

/**
 * @brief Maneja las conexiones entrantes al servidor de Memoria.
 * 
 * Esta función es responsable de gestionar las solicitudes recibidas a través del socket de cliente, 
 * permitiendo la interacción entre los módulos que se conectan y el servidor de Memoria. 
 * Según la operación solicitada, el servidor procesará instrucciones, responderá consultas o 
 * administrará eventos relevantes en la memoria.
 * 
 * Generalmente, esta función se invoca cada vez que un nuevo cliente establece conexión 
 * con el servidor de Memoria.
 *
 * @param client_socket Socket asociado al cliente conectado.
 * @param operation Código que identifica la operación solicitada por el cliente.
 * @param server_name Nombre del servidor que maneja la conexión (en este caso, "Memoria").
 */
void memoria_server_escucha_handler(int client_socket, int operation, const char *server_name);

#endif //MEMORIA_HANDLERS_H