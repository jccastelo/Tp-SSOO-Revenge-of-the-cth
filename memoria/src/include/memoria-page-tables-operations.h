#ifndef MEMORIA_PAGE_TABLES_OPERATIONS_H_
#define MEMORIA_PAGE_TABLES_OPERATIONS_H__ 

#include <commons/collections/list.h>
#include <commons/collections/dictionary.h>
#include <stdbool.h>



#include "memoria-pages-tables.h"

typedef void (*accion_sobre_nodo_t)(t_list* nodo);


/**
 * @brief Aplica recursivamente una acción sobre cada nodo del árbol de páginas.
 *
 * Esta función recorre el árbol de manera postorden, es decir, primero recorre
 * los hijos y luego aplica la acción sobre el nodo actual.
 * 
 * @param nodo Nodo actual (lista de hijos o marcos).
 * @param accion Función a aplicar sobre cada nodo.
 */
void recorrer_arbol_rec(t_list* nodo, accion_sobre_nodo_t accion);
/**
 * @brief Recorre el árbol de páginas de un proceso aplicando una acción sobre cada nodo.
 *
 * Función de alto nivel que inicia el recorrido recursivo desde la raíz del árbol.
 * 
 * @param raiz Nodo raíz del árbol de páginas del proceso.
 * @param accion Función a aplicar sobre cada nodo.
 */
void recorrer_arbol_paginas(t_list* raiz, accion_sobre_nodo_t accion);
/**
 * @brief Elimina una entrada de marco específica del árbol de páginas de un proceso.
 *
 * Recorre el árbol de páginas multinivel hasta llegar a la hoja correspondiente y 
 * elimina el marco si coincide con el `frame_id` dado.
 * 
 * @param frame_id Identificador del marco a eliminar.
 * @param pid Identificador del proceso cuya tabla de páginas se va a modificar.
 */
void eliminar_marco(int frame_id, char* pid_key);

/**
 * @brief Destruye recursivamente un nodo del árbol de páginas liberando su memoria.
 *
 * Recorre cada subnodo del nodo dado y libera sus elementos antes de destruir la lista actual.
 * 
 * @param nodo Nodo del árbol (lista de subnodos o marcos) a destruir.
 */
void destruir_nodo(t_list* nodo);
/**
 * @brief Elimina completamente la tabla de páginas de un proceso, liberando todos los nodos.
 *
 * Extrae la tabla de páginas del proceso del diccionario global y libera toda su memoria asociada.
 * 
 * @param pid Identificador del proceso cuya tabla de páginas se quiere eliminar.
 */
void destruir_tabla_de_paginas(char* pid_key);
#endif // MEMORIA_PAGE_TABLES_OPERATIONS_H_