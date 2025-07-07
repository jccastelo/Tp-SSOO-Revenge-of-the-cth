#ifndef SWAP_FUNTIONS_H_
#define SWAP_FUNTIONS_H_ 

#include <commons/collections/list.h>
#include <commons/collections/dictionary.h>
#include <stdbool.h>


#include "memoria-pages-tables.h" 

typedef struct {
    int nro_pagina;     
    int offset_swap;    
} swap_entry_t;
extern t_dictionary* diccionario_swap_metadata; 
extern FILE* archivo_swap ;; 


#endif // SWAP_FUNTIONS_H_

/**
 * @brief Inicializa el archivo de swap y crea el diccionario de metadata para gestionar el contenido del swap.
 * 
 * Crea un archivo llamado "swap.bin" en modo escritura binaria para inicializarlo vacío, 
 * luego lo vuelve a abrir en modo lectura/escritura binario ("rb+").
 * También se crea un diccionario global para almacenar metadata del swap por proceso.
 */
void init_swap();

/**
 * @brief Obtiene la lista de marcos asociada a un proceso específico desde la tabla de páginas.
 * 
 * @param pid_key Clave que representa el PID del proceso (se libera dentro de la función).
 * @param tabla_de_paginas Diccionario que contiene listas de marcos por proceso.
 * @return Lista de marcos del proceso si existe, o NULL si no se encontraron entradas asociadas.
 */
t_list* get_marcos_list_of_proc(char* pid_key, t_dictionary *tabla_de_paginas);

/**
 * @brief Elimina y retorna la lista de metadata de swap asociada a un proceso específico.
 * 
 * @param pid_key Clave que representa el PID del proceso (se libera dentro de la función).
 * @param diccionario_swap Diccionario que contiene la metadata de swap de todos los procesos.
 * @return Lista de metadata eliminada del proceso, o NULL si no existía metadata asociada.
 */
t_list* remove_marcos_list_of_proc(char* pid_key , t_dictionary *diccionario_swap);

/**
 * @brief Libera los recursos utilizados por el sistema de swap, cerrando el archivo y destruyendo la metadata.
 * 
 * Cierra el archivo "swap.bin" si está abierto y libera la memoria del diccionario global de metadata.
 */
void finalizar_swap();