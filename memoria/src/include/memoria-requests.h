#ifndef MEMORIA_REQUESTS_H
#define MEMORIA_REQUESTS_H

// Incluimos las biblotecas internas:
#include "memoria-protocols.h"
#include "memoria-instructions.h"

/**
 * @brief Inicializa un proceso en el servidor en respuesta a una solicitud del kernel.
 * 
 * Esta función se utiliza para recibir una solicitud del kernel para la creación y inicialización 
 * de un nuevo proceso. Se encarga de configurar los parámetros necesarios, como la asignación 
 * de recursos y la preparación del entorno del proceso, para que este pueda comenzar a ejecutar 
 * tareas específicas según la solicitud recibida.
 * 
 * @param client_socket El socket a través del cual el servidor interactúa con el cliente,
 *                      permitiendo la transmisión de datos necesarios para la inicialización.
 */
void init_process(int client_socket);

#endif // MEMORIA_REQUESTS_H
