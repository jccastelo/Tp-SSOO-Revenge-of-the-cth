#ifndef MEMORIA_PROCESS_H
#define MEMORIA_PROCESS_H

// Incluyo las bibliotecas externas necesarias:
#include <stdlib.h>
#include <commons/bitarray.h>
#include <commons/collections/list.h>

// Incluyo las bibliotecas internas necesarias:
#include "memoria-state.h"

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


#endif