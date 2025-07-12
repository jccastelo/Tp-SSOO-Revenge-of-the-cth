#ifndef CPU_CONFIG_H
#define CPU_CONFIG_H

// Incluye las bibliotecas necesarias:
#include <stdlib.h>
#include <utils/logger.h>
#include <commons/config.h>
#include <commons/string.h>
#include <string.h>

// Incluye las estructuras necesarias:
#include "cpu-types.h"
#include "cpu-state.h"

/**
 * @brief Inicializa la estructura de configuración del módulo cpu.
 * 
 * Reserva memoria e inicializa los campos de la estructura `t_config_cpu`.
 * 
 * @return Puntero a una estructura `t_config_cpu` inicializada.
 */
t_config_cpu* inicializar_config_cpu();

/**
 * @brief Carga y configura los parámetros del módulo cpu desde el archivo de configuración.
 * 
 * Inicializa las estructuras necesarias y completa sus campos a partir de la configuración general.
 * 
 * @return No devuelve ningún valor.
 */
void cpu_config_init(char *config_path);

/**
 * @brief Configura los valores relacionados con la memoria en la estructura de configuración del cpu.
 * 
 * Completa los campos correspondientes a la configuración de memoria utilizando los datos del archivo de configuración.
 * 
 * @param config_io Puntero a la estructura `t_config_cpu` a completar.
 * @param config Puntero a la configuración general desde donde se obtendrán los valores.
 * @return No devuelve ningún valor.
 */
void configurar_valores_de_memoria(t_config_cpu* config_io, t_config* config);

/**
 * @brief Configura los valores relacionados con el módulo kernel en la estructura de configuración del cpu.
 * 
 * Asigna los parámetros provenientes del archivo de configuración relacionados con la interacción entre el cpu y el kernel.
 * 
 * @param config_cpu Puntero a la estructura `t_config_cpu` a completar.
 * @param config Puntero a la configuración general desde donde se obtendrán los valores.
 * @return No devuelve ningún valor.
 */
void configurar_valores_de_kernel(t_config_cpu* config_cpu, t_config* config);

/**
 * @brief Configura los valores relacionados con la TLB (Translation Lookaside Buffer) en la estructura de configuración del cpu.
 * 
 * Carga los parámetros que determinan el comportamiento de la TLB desde el archivo de configuración.
 * 
 * @param config_cpu Puntero a la estructura `t_config_cpu` a completar.
 * @param config Puntero a la configuración general desde donde se obtendrán los valores.
 * @return No devuelve ningún valor.
 */
void configurar_valores_de_tlb(t_config_cpu* config_cpu, t_config* config);

/**
 * @brief Configura los valores relacionados con la caché de instrucciones en la estructura de configuración del cpu.
 * 
 * Establece los parámetros necesarios para el funcionamiento de la caché utilizando la información del archivo de configuración.
 * 
 * @param config_cpu Puntero a la estructura `t_config_cpu` a completar.
 * @param config Puntero a la configuración general desde donde se obtendrán los valores.
 * @return No devuelve ningún valor.
 */
void configurar_valores_de_cache(t_config_cpu *config_cpu, t_config *config);

/**
 * @brief Configura los valores relacionados con el sistema de logs en la estructura de configuración del cpu.
 * 
 * Establece el nivel y la ruta del log a partir de la configuración general.
 * 
 * @param config_io Puntero a la estructura `t_config_cpu` a completar.
 * @param config Puntero a la configuración general desde donde se obtendrán los valores.
 * @return No devuelve ningún valor.
 */
void configurar_valores_de_log(t_config_cpu* config_io, t_config* config);

#endif // CPU_CONFIG_H