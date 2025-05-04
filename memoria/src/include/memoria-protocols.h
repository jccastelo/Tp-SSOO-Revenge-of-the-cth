#ifndef MEMORIA_PROTOCOLS_H
#define MEMORIA_PROTOCOLS_H

// Incluyo las bibliotecas externas necesarias:
#include <utils/protocolo.h>
#include <utils/logger.h>

/**
 * @brief Recibe y deserializa los datos necesarios para iniciar el procesamiento de un archivo enviado por un cliente.
 *
 * Esta función se encarga de recibir un buffer desde el socket del cliente, interpretar su contenido y extraer
 * tres elementos clave: el identificador del proceso, el tamaño del proceso y el archivo a procesar.
 * Los datos extraídos se almacenan en las variables provistas por el llamador.
 *
 * @param client_socket     Socket desde el cual se recibe el buffer con la información.
 * @param id_process        Puntero a un entero donde se almacenará el ID del proceso recibido.
 * @param tam_process       Puntero a un entero donde se almacenará el tamaño del proceso.
 * @param file_procces      Puntero a una cadena (char**) donde se almacenará el archivo recibido.
 *                          La memoria para la cadena se asigna dinámicamente dentro de la función.
 */
void rcv_setup_to_process(int client_socket, int *id_process, int *tam_process, char **file_procces);

#endif // MEMORIA_PROTOCOLS_H