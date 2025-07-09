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
void write_memory(int client_socket, int id_process, char *content_to_write, int physical_address);

/**
 * @brief Lee datos desde el espacio de memoria de usuario en una dirección física específica.
 *
 * Esta función copia un bloque de datos desde la memoria de usuario, comenzando en la dirección
 * física especificada, y envía el contenido leído al cliente a través del socket indicado.
 *
 * @param client_socket Descriptor del socket del cliente al que se enviará la respuesta con los datos leídos.
 * @param id_process Identificador del proceso que solicita la operación de lectura.
 * @param quantity_bytes Cantidad de bytes a leer desde la memoria de usuario.
 * @param physical_address Dirección física absoluta desde donde se inicia la lectura.
 *
 * @details En caso de error (por ejemplo, si la dirección solicitada excede los límites de la memoria),
 * se responde con ERROR. Si la operación se realiza exitosamente, se copia el contenido solicitado y se responde con OK.
 */
void read_memory(int client_socket, int id_process, int quantity_bytes, int physical_address);

/**
 * @brief Marca como libres los marcos indicados en una lista.
 *
 * Esta función recorre una lista de marcos (frames) y, para cada uno,
 * limpia el bit correspondiente en el bitmap global `frames_bitmap`,
 * indicando que ese marco queda disponible para futuros usos.
 *
 * @param free_frames Lista (`t_list *`) que contiene punteros a marcos (frames) que se deben marcar como libres.
 *
 * @details Internamente, se define una función auxiliar que se encarga de convertir cada puntero
 * de la lista a un entero (número de marco) y limpiar su bit asociado en el bitmap.
 * 
 * Esta función es útil, por ejemplo, al finalizar un proceso o al liberar páginas
 * que ya no se necesitan en memoria, permitiendo que los marcos se puedan reutilizar.
 */
void mark_frames_as_free(t_list *free_frames);

/**
 * @brief Finaliza un proceso en memoria liberando todos sus recursos asociados.
 *
 * Esta función recibe el identificador de un proceso, lo elimina del diccionario
 * de métricas, obtiene y libera los frames ocupados en memoria, y finalmente
 * marca dichos frames como libres. Una vez completado, devuelve OK para indicar
 * que el proceso fue finalizado correctamente.
 *
 * @param id_process Identificador único del proceso a finalizar.
 * @return OK si el proceso fue finalizado correctamente.
 */
int is_process_end(int id_process);

/**
 * @brief Inicializa un bitmap de frames marcando todos los bits como libres.
 *
 * Esta función recorre todos los bits del bitmap recibido y los limpia (pone en 0),
 * indicando que todos los frames están disponibles (libres) para ser asignados.
 *
 * @param frames_bitmap Puntero al bitmap de frames a inicializar.
 */
void inicializar_bitmap(t_bitarray *frames_bitmap);

#endif