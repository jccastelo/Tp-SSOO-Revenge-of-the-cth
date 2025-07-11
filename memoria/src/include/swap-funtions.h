#ifndef SWAP_FUNTIONS_H_
#define SWAP_FUNTIONS_H_ 

// Incluimos librerias externas:
#include <commons/memory.h>
#include <commons/collections/list.h>
#include <commons/collections/dictionary.h>
#include <stdbool.h>

// Incluimos librerias internas
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

/**
 * @brief Realiza el swap in de todas las páginas de un proceso desde el archivo de swap
 *        a la memoria principal. Reconstituye la tabla de páginas del proceso con los marcos
 *        recién asignados y actualiza las métricas correspondientes.
 *
 * @param pid_key Clave (como string) que identifica al proceso cuyo contenido debe traerse
 *                desde swap a memoria. Debe coincidir con la clave usada en las estructuras
 *                del sistema como el diccionario de swap o de tablas de páginas.
 *
 * Este procedimiento implica:
 *  - Incrementar los contadores de métricas relacionadas a swap in, swap out y escritura.
 *  - Obtener y remover la metadata del proceso desde el diccionario de swap.
 *  - Verificar si hay suficiente memoria para cargar todas las páginas del proceso.
 *  - Leer cada página desde el archivo de swap y escribirla en un marco libre.
 *  - Crear una nueva tabla de páginas para el proceso con los marcos asignados.
 *  - Registrar esta tabla en el diccionario global de procesos en memoria.
 *  - Liberar los recursos auxiliares utilizados (listas de metadata y marcos).
 *
 * @note En caso de no haber suficiente memoria disponible, se aborta el swap in
 *       y se libera la metadata asociada al intento.
 */
void swap_in(char* pid_key, int pid, int socket_client); 

/**
 * @brief Vacía las entradas de swap de un proceso en el diccionario de metadatos.
 * 
 * Esta función libera toda la memoria asociada a las entradas de swap de un proceso
 * identificado por `pid_key` dentro del diccionario `diccionario_swap_metadata`.
 * No modifica el archivo de swap, por lo que puede generar fragmentación lógica.
 * 
 * @param pid Identificador numérico del proceso (usado solo para logging).
 * @param pid_key Clave string del proceso usada para acceder y eliminar sus metadatos.
 *                Esta cadena se libera dentro de la función.
 */
void vaciar_swap_del_proceso(int pid, char* pid_key);