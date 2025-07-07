#ifndef MEMORIA_REQUESTS_H
#define MEMORIA_REQUESTS_H

// Incluimos las biblotecas internas:
#include "memoria-protocols.h"
#include "memoria-instructions.h"
#include "memoria-user-spaces.h"
#include "memoria-pages-tables.h"
#include "memoria-processes.h"
#include "swap-funtions.h"
#include "memoria-processes.h"
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

/**
 * @brief Envía una instrucción de un proceso a solicitud del cliente (CPU).
 * 
 * Esta función coordina la recepción de los parámetros necesarios (ID del proceso y program counter)
 * y la posterior obtención y envío de la instrucción correspondiente desde la memoria. 
 * Se utiliza como parte del protocolo de comunicación entre el módulo de memoria y otros componentes, 
 * permitiendo que el cliente obtenga la instrucción que necesita ejecutar.
 * 
 * @param cliente_socket El socket a través del cual se recibe la solicitud y se envía la instrucción.
 */
void send_process_instruction(int cliente_socket);

#endif // MEMORIA_REQUESTS_H
