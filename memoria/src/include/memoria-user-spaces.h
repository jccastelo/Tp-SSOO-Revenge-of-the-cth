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
 * Escribe datos en el espacio de memoria de usuario en una posición específica.
 *
 * Esta función copia un buffer de datos proporcionado por el proceso en la memoria de usuario,
 * comenzando desde la posición calculada a partir del frame físico y el offset indicados.
 *
 * Parámetros:
 * - client_socket: Socket del cliente al que se enviará la respuesta de estado.
 * - id_process: Identificador del proceso que solicita la escritura.
 * - searched_frame: Número de frame físico donde se debe realizar la escritura.
 * - extra_data: Puntero al buffer con los datos a escribir (se interpreta como char *).
 * - offset: Desplazamiento dentro del frame donde comienza la escritura.
 *
 * En caso de error (overflow de memoria o frame), se responde con ERROR.
 * Si la operación es exitosa, se responde con OK.
 */
void write_memory(int client_socket, int id_process, int searched_frame, void *extra_data, int offset);

/**
 * Lee datos del espacio de memoria de usuario en una posición específica y los devuelve al cliente.
 *
 * Esta función copia un bloque de memoria desde la memoria de usuario a un buffer temporal,
 * a partir de la posición calculada con el frame físico y el offset indicados. Luego,
 * envía el estado de la operación y el contenido leído al cliente.
 *
 * Parámetros:
 * - client_socket: Socket del cliente al que se enviará la respuesta y los datos leídos.
 * - id_process: Identificador del proceso que solicita la lectura.
 * - searched_frame: Número de frame físico desde donde se debe leer.
 * - extra_data: Puntero a un entero que indica la cantidad de bytes a leer.
 * - offset: Desplazamiento dentro del frame donde comienza la lectura.
 *
 * En caso de error (overflow de memoria o frame), se responde con ERROR y no se envía contenido.
 * Si la operación es exitosa, se responde con OK seguido del contenido leído.
 */
void read_memory(int client_socket, int id_process, int searched_frame, void *extra_data, int offset);

#endif