#ifndef KERNEL_CONFIG_H
#define KERNEL_CONFIG_H

// Incluye las bibliotecas necesarias:
#include <utils/logger.h>
#include <commons/config.h>
#include <stdlib.h>
#include <string.h>

// Incluye las estructuras necesarias:
#include "kernel-types.h"
#include "kernel-state.h"

/**
 * @brief Inicializa la estructura de configuración del módulo Kernel.
 * 
 * Reserva memoria e inicializa los campos de la estructura `t_config_kernel`.
 * 
 * @return Puntero a una estructura `t_config_kernel` inicializada.
 */
t_config_kernel* inicializar_config_kernel();

/**
 * @brief Carga y configura los parámetros del módulo Kernel desde el archivo de configuración.
 * 
 * Inicializa las estructuras necesarias y completa sus campos a partir de la configuración general.
 * 
 * @return No devuelve ningún valor.
 */
void kernel_config_init(char *config_route);

/**
 * @brief Configura los valores relacionados con la memoria en la estructura de configuración del Kernel.
 * 
 * Completa los campos correspondientes a la configuración de memoria utilizando los datos del archivo de configuración.
 * 
 * @param config_io Puntero a la estructura `t_config_kernel` a completar.
 * @param config Puntero a la configuración general desde donde se obtendrán los valores.
 * @return No devuelve ningún valor.
 */
void configurar_valores_de_memoria(t_config_kernel* config_io, t_config* config);

/**
 * @brief Configura los valores relacionados con los servidores en la estructura de configuración del Kernel.
 * 
 * Establece la configuración de los servidores (puertos, direcciones, etc.) en base a la configuración general.
 * 
 * @param config_io Puntero a la estructura `t_config_kernel` a completar.
 * @param config Puntero a la configuración general desde donde se obtendrán los valores.
 * @return No devuelve ningún valor.
 */
void configurar_valores_de_servidores(t_config_kernel* config_io, t_config* config);

/**
 * @brief Configura los parámetros de planificación en la estructura de configuración del Kernel.
 * 
 * Define el algoritmo de planificación y sus parámetros (quantum, estimaciones, etc.) a partir de la configuración general.
 * 
 * @param config_io Puntero a la estructura `t_config_kernel` a completar.
 * @param config Puntero a la configuración general desde donde se obtendrán los valores.
 * @return No devuelve ningún valor.
 */
void configurar_valores_de_planificacion(t_config_kernel* config_io, t_config* config);

/**
 * @brief Configura los valores relacionados con el sistema de logs en la estructura de configuración del Kernel.
 * 
 * Establece el nivel y la ruta del log a partir de la configuración general.
 * 
 * @param config_io Puntero a la estructura `t_config_kernel` a completar.
 * @param config Puntero a la configuración general desde donde se obtendrán los valores.
 * @return No devuelve ningún valor.
 */
void configurar_valores_de_log(t_config_kernel* config_io, t_config* config);


#endif // IO_CONFIG_H