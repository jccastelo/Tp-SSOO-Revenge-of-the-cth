#ifndef CPU_CICLO_INSTRUCCION_H
#define CPU_CICLO_INSTRUCCION_H

#include <utils/logger.h>
#include <utils/protocolo.h>
#include <commons/string.h>

#include "cpu-types.h"
#include "cpu-state.h"
#include "cpu-comunicacion-memoria.h"
#include "cpu-comunicacion-kernel.h"

#include "../include/cpu-ciclo-instruccion.h"

/**
 * @brief Realiza el ciclo de instrucción de un proceso.
 * 
 * Ejecuta instrucciones sucesivas hasta que una syscall o una interrupción indique que debe finalizar.
 */
void realizar_ciclo_de_instruccion();

/**
 * @brief Obtiene la siguiente instrucción desde el módulo correspondiente.
 * 
 * Incrementa el Program Counter (PC) y registra el acceso en logs.
 * 
 * @return char* Cadena que representa la instrucción a ejecutar.
 */
char* fetch_instruction();

/**
 * @brief Decodifica una instrucción en su estructura interna.
 * 
 * Convierte la cadena en un array de argumentos y determina el tipo de instrucción.
 * 
 * @param instruccion Cadena con la instrucción completa.
 * @return t_instruccion* Puntero a la estructura que contiene el tipo y los argumentos de la instrucción.
 */
t_instruccion* decode(char* instruccion);

/**
 * @brief Ejecuta una instrucción previamente decodificada.
 * 
 * Realiza la acción correspondiente al tipo de instrucción y avanza el PC si aplica.
 * 
 * @param instruccion Puntero a la estructura de instrucción.
 */
void excecute(t_instruccion* instruccion);

/**
 * @brief Determina si una instrucción es una syscall que implica frenar la ejecución del proceso.
 * 
 * Las instrucciones IO, DUMP_MEMORY y EXIT provocan la suspensión o finalización del proceso.
 * 
 * @param tipo Tipo de instrucción.
 * @return true Si la instrucción debe frenar la ejecución.
 * @return false En caso contrario.
 */
bool es_syscall_que_frena(t_tipo_instruccion tipo);

/**
 * @brief Verifica si se ha recibido una interrupción y actúa en consecuencia.
 * 
 * En caso de interrupción, se registra el evento y se envía el contexto para su desalojo.
 * 
 * @return true Si hubo una interrupción y debe finalizar el ciclo.
 * @return false Si no hubo interrupción.
 */
bool check_interrupt();

/**
 * @brief Mapea una cadena a un tipo de instrucción.
 * 
 * Convierte una palabra clave como "NOOP" o "IO" al valor correspondiente del enum t_tipo_instruccion.
 * 
 * @param tipo_instruccion Cadena con el nombre de la instrucción.
 * @return t_tipo_instruccion Tipo de instrucción correspondiente.
 */
t_tipo_instruccion mapeo_string_tipo(char* tipo_instruccion);

#endif // CPU_COMUNICACION_MEMORIA_H_