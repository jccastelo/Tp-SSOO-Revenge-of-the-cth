#ifndef MEMORIA_INSTRUCTIONS_H_
#define MEMORIA_INSTRUCTIONS_H_

// Incluyo las bibliotecas externas necesarias:
#include <stdio.h>
#include <string.h>
#include <utils/logger.h>
#include <commons/string.h>
#include <commons/collections/dictionary.h>
#include <commons/collections/list.h>

// Incluyo las bibliotecas internas necesarias:
#include "memoria-state.h"

/**
 * @brief Carga las instrucciones pseudocódigo de un archivo y las asocia al proceso identificado por id_process.
 *
 * Esta función completa la ruta del archivo, lo abre, lee las instrucciones línea por línea,
 * y las almacena en la estructura global instrucciones_por_procesos usando como clave el ID del proceso.
 *
 * @param id_process Identificador del proceso al que se asociarán las instrucciones.
 * @param file_procces Ruta relativa al archivo de pseudocódigo. Se completará internamente con la carpeta correspondiente.
 */
void loading_process_instructions(int id_process, char *file_procces);

/**
 * @brief Asocia la ruta del archivo con la carpeta de instrucciones.
 *
 * Modifica la cadena file_procces agregando el path del directorio correspondiente al archivo de pseudocódigo.
 * 
 * @param file_procces Puntero al nombre del archivo, que se transformará en la ruta completa.
 */
void associate_instructions_folder_path(char **file_procces);

/**
 * @brief Lee un archivo de pseudocódigo y devuelve una lista con las instrucciones.
 *
 * Cada línea del archivo se interpreta como una instrucción y se almacena como un elemento en la lista.
 *
 * @param pseudocode_file Puntero al archivo abierto en modo lectura.
 * @return Lista de instrucciones leídas desde el archivo.
 */
t_list *read_pseudocode_file(FILE *pseudocode_file);

/**
 * @brief Asocia una lista de instrucciones a un ID de proceso en el diccionario global.
 *
 * Almacena la lista de instrucciones utilizando como clave el ID del proceso en forma de string.
 *
 * @param id_process Identificador del proceso.
 * @param process_instructions Lista de instrucciones asociadas al proceso.
 */
void load_process_instructions_in_instrucciones_por_procesos(int id_process, t_list *process_instructions);

#endif // MEMORIA_INSTRUCTIONS_H