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
/**
 * @brief Parsea un entero desde un buffer en una posición determinada.
 *
 * Esta función copia un valor entero (`int`) desde el buffer en la posición indicada
 * por el desplazamiento, lo almacena en la variable de destino y actualiza el desplazamiento.
 *
 * @param buffer Puntero al buffer que contiene los datos.
 * @param desplazamiento Puntero al desplazamiento actual dentro del buffer (en bytes). Se actualiza tras la lectura.
 * @param destino Puntero a la variable donde se almacenará el valor entero parseado.
 */
void parsear_int(void* buffer, int* desplazamiento, int* destino);

/**
 * @brief Parsea una cadena de caracteres desde un buffer.
 *
 * Esta función lee un entero inicial que representa la longitud de la cadena,
 * luego copia esa cantidad de caracteres desde el buffer, reserva memoria para el string
 * e incluye el caracter nulo al final (`'\0'`). También actualiza el desplazamiento.
 *
 * @param buffer Puntero al buffer que contiene los datos.
 * @param desplazamiento Puntero al desplazamiento actual dentro del buffer (en bytes). Se actualiza tras la lectura.
 * @param destino Puntero donde se almacenará el puntero a la cadena parseada (se reserva memoria internamente).
 */
void parsear_string(void* buffer, int* desplazamiento, char** destino);

#endif // MEMORIA_PROTOCOLS_H