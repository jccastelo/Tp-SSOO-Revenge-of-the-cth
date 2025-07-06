#ifndef MEMORIA_PAGES_TABLES_H
#define MEMORIA_PAGES_TABLES_H

// Incluyo las bibliotecas externas necesarias:
#include <stdlib.h>
#include <commons/collections/dictionary.h>
#include <commons/collections/list.h>

// Incluyo las bibliotecas internas necesarias:
#include "memoria-state.h"
#include "memoria-user-spaces.h"

/**
 * @brief Inicializa la estructura de tablas de páginas para un proceso.
 *
 * Esta función crea la estructura raíz de la tabla multinivel que representará
 * la memoria virtual del proceso y la registra en la estructura global que mantiene
 * todas las tablas de páginas por proceso. Una vez creada la raíz, delega la
 * asignación bajo demanda de los frames libres disponibles.
 *
 * @param id_process Identificador del proceso que recibirá la tabla de páginas.
 * @param free_frames Lista de frames libres que se usarán para mapear páginas del proceso.
 */
void setup_page_tables(int id_process, t_list *free_frames);

/**
 * @brief Construye la tabla de páginas multinivel asignando los frames libres.
 *
 * Esta función recorre la lista de frames libres e inserta cada uno en la estructura
 * multinivel que representa la tabla de páginas del proceso. La función utiliza
 * una función auxiliar para insertar cada frame en el nivel correspondiente
 * y crea nodos intermedios cuando es necesario.
 *
 * @param free_frames Lista de frames libres que se asignarán al proceso.
 * @param page_table_root Nodo raíz de la estructura multinivel de páginas del proceso.
 */
void populate_page_table(t_list *free_frames, t_list *page_table_root);

/**
 * @brief Inserta cada frame libre en la tabla de páginas multinivel del proceso.
 *
 * Esta función itera sobre la lista de frames libres y para cada frame
 * llama a la función recursiva que inserta el frame en el nivel y posición
 * adecuados dentro de la estructura multinivel de la tabla de páginas.
 *
 * @param free_frames Lista de frames libres disponibles para asignación.
 * @param page_table_root Nodo raíz de la tabla de páginas donde se insertan los frames.
 * @param divisors Arreglo con divisores precomputados para cálculo de índices por nivel.
 * @param total_levels Número total de niveles en la tabla multinivel.
 * @param entries_per_level Cantidad de entradas permitidas por cada nivel.
 */
void insert_each_frame(t_list *free_frames, t_list *page_table_root, int *divisors, int total_levels, int entries_per_level);

/**
 * @brief Inserta un frame en la tabla de páginas multinivel en la posición correspondiente.
 *
 * Esta función inserta recursivamente un frame en el nivel adecuado dentro
 * de la estructura multinivel que representa la tabla de páginas. Si está
 * en el último nivel, inserta directamente el frame. En niveles intermedios,
 * calcula el índice adecuado, crea o reutiliza el nodo hijo correspondiente
 * y continúa la inserción en el siguiente nivel.
 *
 * @param current_node Nodo actual en la tabla de páginas donde se intenta insertar.
 * @param level Nivel actual dentro de la tabla multinivel.
 * @param total_levels Número total de niveles en la tabla multinivel.
 * @param entries_per_level Cantidad máxima de entradas permitidas en cada nivel.
 * @param divisors Arreglo con valores precomputados para cálculo de índices por nivel.
 * @param frame Identificador del frame a insertar (valor real).
 * @param frame_id Índice o posición del frame dentro de la lista de frames libres (usado para cálculo del índice).
 */
void insert_frame_in_table(t_list *current_node, int level, int total_levels, int entries_per_level, int *divisors, int frame, int frame_id);

/**
 * @brief Calcula el índice de la entrada en un nivel dado para un frame.
 *
 * Utiliza los divisores precomputados para determinar la posición correcta
 * en el nivel multinivel correspondiente al frame_id.
 *
 * @param frame_id Identificador del frame.
 * @param level Nivel actual en la tabla multinivel.
 * @param entries_per_level Cantidad de entradas por nivel.
 * @param divisors Arreglo con los divisores precomputados.
 * @return Índice calculado para el frame en el nivel indicado.
 */
int calculate_index(int frame_id, int level, int entries_per_level, int *divisors);

/**
 * @brief Obtiene el nodo hijo en la posición especificada o crea uno nuevo.
 *
 * Si el hijo en el índice dado existe, lo retorna.  
 * Si no existe, crea un nuevo nodo hijo, lo agrega a la lista y lo retorna.
 *
 * @param parent Nodo padre donde se buscan o crean los hijos.
 * @param index Índice del hijo en la lista del nodo padre.
 * @return Puntero al nodo hijo obtenido o creado.
 */
t_list* get_or_create_child(t_list *parent, int index);

/**
 * @brief Precomputa los divisores usados para calcular índices en cada nivel.
 *
 * Los divisores son potencias de la cantidad de entradas por nivel,
 * usados para calcular rápidamente la posición en cada nivel multinivel.
 *
 * @param total_levels Cantidad total de niveles en la tabla.
 * @param entries_per_level Cantidad de entradas por nivel.
 * @param divisors Arreglo donde se almacenan los divisores precomputados.
 */
void precompute_divisors(int total_levels, int entries_per_level, int *divisors);

/**
 * @brief Busca el frame correspondiente dado un proceso y una lista de índices por nivel.
 *
 * Recorre la tabla multinivel desde la raíz, navegando por las entradas indicadas en 'entries_per_level'.
 * Al llegar al último nivel, devuelve el frame asociado.
 *
 * @param id_process Identificador del proceso.
 * @param entries_per_level Lista de índices para cada nivel de la tabla de páginas.
 * @return El número de frame encontrado, o -1 si no se encuentra.
 */
int find_frame_from_entries(int id_process, t_list *entries_per_level);

/**
 * @brief Obtiene la tabla raíz de páginas de un proceso dado su ID.
 *
 * Convierte el ID del proceso a cadena para usarlo como clave en el diccionario
 * global de tablas de páginas y devuelve la tabla raíz asociada.
 *
 * @param id_process Identificador del proceso.
 * @return Puntero a la lista que representa la tabla raíz de páginas, o NULL si no existe.
 */
t_list *get_root_table(int id_process);


/**
 * @brief Resuelve el número de frame físico a partir de una dirección física.
 *
 * Determina el frame físico correspondiente a una dirección dada, considerando si la
 * dirección incluye un offset (cuando hay N+1 niveles). En ese caso, el offset se
 * extrae y se retorna por parámetro, y el frame se calcula a partir de las entradas.
 * Si no hay offset, se asume que el primer elemento de la lista ya es el frame.
 *
 * @param id_process Identificador del proceso que accede a la memoria.
 * @param physical_address Lista de entradas que representan la dirección física.
 *                         Puede contener N entradas o N+1 si incluye offset.
 * @param offeset Puntero a entero donde se almacenará el offset, si aplica.
 * @return Número de frame físico correspondiente a la dirección.
 */
int resolve_physical_frame(int id_process, t_list *physical_address, int *offeset);

#endif