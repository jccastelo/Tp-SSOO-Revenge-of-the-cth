#ifndef MEMORIA_PROCESS_H
#define MEMORIA_PROCESS_H

// Incluyo las bibliotecas externas necesarias:
#include <stdlib.h>
#include <commons/bitarray.h>
#include <commons/collections/list.h>
#include <utils/protocolo.h>

// Incluyo las bibliotecas internas necesarias:
#include "memoria-state.h"
#include "memoria-processes.h"
#include "memoria-protocols.h"

/**
 * @brief Inicializa el bitmap que representa la disponibilidad de frames en memoria.
 *
 * Calcula la cantidad de bytes necesarios para almacenar un bit por cada frame del sistema,
 * reserva la memoria para el bitmap e inicializa la estructura bitarray que administrará
 * qué frames están libres (0) u ocupados (1), usando el modo LSB-first.
 */
void init_free_frames_bitmap();

/**
 * @brief Busca frames libres suficientes para alojar un proceso.
 *
 * Calcula la cantidad de frames que el proceso necesita y recorre el bitmap
 * global de frames en memoria para recopilar sus índices. Si la cantidad de
 * frames libres es suficiente, retorna una lista con los índices de los frames;
 * de lo contrario, libera la lista y retorna NULL.
 *
 * @param size_process Tamaño del proceso en bytes.
 * @return Una lista (`t_list*`) con los índices (como punteros a entero) de
 *         los frames libres si hay suficientes frames disponibles;
 *         NULL si no hay suficientes frames libres.
 */
t_list *is_memory_sufficient(int size_process);

/**
 * @brief Marca como ocupados los marcos indicados en una lista.
 *
 * Recorre una lista de marcos libres e indica en el bitmap global que 
 * esos marcos ya no están disponibles. Se asume que los elementos de la 
 * lista son punteros que representan índices de marcos (convertibles a int).
 *
 * Esta función actualiza el estado de ocupación de cada marco restando 1 
 * a su índice, ya que el bitmap es de base cero.
 *
 * @param free_frames Lista (`t_list*`) de marcos a marcar como ocupados.
 *                    Cada elemento debe ser un puntero que representa un número de marco.
 */
void mark_frames_as_busy(t_list *free_frames);

/**
 * @brief Escribe datos en el espacio de memoria de usuario en una posición física específica.
 *
 * Esta función copia el contenido de un buffer de datos proporcionado por el proceso
 * en la memoria de usuario, comenzando desde la posición calculada a partir de la 
 * dirección física indicada.
 *
 * @param client_socket Descriptor del socket del cliente al cual se enviará la respuesta de estado.
 * @param id_process Identificador del proceso que solicita la escritura.
 * @param extra_data Puntero al buffer con los datos a escribir (interpretado como char *).
 * @param physical_address Dirección física absoluta donde se debe comenzar a escribir.
 *
 * @details En caso de error (por ejemplo, overflow de memoria), se envía una respuesta con ERROR.
 * Si la operación se completa exitosamente, se envía una respuesta con OK.
 */
void write_memory(int client_socket, int id_process, void *extra_data, int physical_address);

/**
 * @brief Lee datos desde el espacio de memoria de usuario en una posición física específica.
 *
 * Esta función copia un bloque de datos desde la memoria de usuario, comenzando en la dirección
 * física indicada, y luego envía el contenido leído al cliente a través del socket especificado.
 *
 * @param client_socket Descriptor del socket del cliente al cual se enviará la respuesta con los datos leídos.
 * @param id_process Identificador del proceso que solicita la lectura.
 * @param extra_data Puntero a un entero que indica el tamaño (en bytes) de los datos a leer.
 * @param physical_address Dirección física absoluta desde donde se debe comenzar la lectura.
 *
 * @details En caso de error (por ejemplo, si la dirección excede los límites de la memoria),
 * se responde con ERROR. Si la operación es exitosa, se copia el contenido y se responde con OK.
 */
void read_memory(int client_socket, int id_process, void *extra_data, int physical_address);

#endif