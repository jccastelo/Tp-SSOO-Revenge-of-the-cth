#ifndef MEMORIA_PROCESSES_H
#define MEMORIA_PROCESSES_H

// Incluyo las bibliotecas externas necesarias:
#include <stdlib.h>
#include <commons/collections/dictionary.h>
#include <commons/string.h>

// Incluyo las bibliotecas externas necesarias:
#include "memoria-state.h"

typedef struct {
    int tablas_requests;         
    int instrs_requests;        
    int swap_out_requests;       
    int swap_in_requests;        
    int mem_read_requests;       
    int mem_write_requests;      
} t_process_in_memory;  

typedef enum {
    TABLAS_REQUESTS,
    INSTRS_REQUESTS,
    SWAP_OUT_REQUESTS,
    SWAP_IN_REQUESTS,
    MEM_READ_REQUESTS,
    MEM_WRITE_REQUESTS,
} t_campo;

/**
 * @brief Inicializa un nuevo proceso en memoria solicitando espacio dinámico e inicializando sus contadores.
 *
 * Esta función reserva memoria dinámica para una estructura `t_process_in_memory` y pone a cero
 * todos los contadores relacionados con la gestión de memoria y procesos, tales como accesos a tablas de páginas,
 * instrucciones solicitadas, operaciones de swap y accesos de lectura/escritura en memoria.
 *
 * @return Puntero a la nueva estructura inicializada en memoria dinámica.
 */
t_process_in_memory *initialization_process();

/**
 * @brief Agrega un nuevo proceso al diccionario global de procesos activos.
 *
 * Recibe el ID de un proceso entero, lo convierte a cadena para usarlo como clave en el diccionario,
 * inicializa la estructura que representa al proceso en memoria y lo almacena en el diccionario global.
 *
 * @param id_process Identificador único del proceso a agregar.
 */
void add_process_to_memory(int id_process); 

/**
 * @brief Se fija si ese proceso está dentro del diccionario, 
 * si la longitud de marcos que contiene su subtabla es NULL retorna falso
 * 
 * @param key_pid es el char del proceso 
 * @param diccionario es la fuente donde busca el pid
 */
bool estaEn(t_list* diccionario, char* key_pid);

/**
 * @brief Calcula la cantidad de frames necesarios para alojar un proceso en memoria.
 *
 * Dado el tamaño del proceso en bytes, calcula cuántos frames completos (páginas)
 * se requieren para alojar dicho proceso, considerando la fragmentación interna
 * que ocurre al reservar frames completos.
 *
 * @param size_process Tamaño del proceso en bytes.
 * @return Número entero de frames necesarios para almacenar el proceso.
 */
int required_frames_for_process(int size_process); 

void aumentar_contador(t_dictionary* , t_campo , char*);

#endif