#ifndef IO_CONFIG_H
#define IO_CONFIG_H

// Incluye las bibliotecas necesarias:
#include <utils/logger.h>
#include <commons/config.h>
#include <stdlib.h>
#include <string.h>

// Incluye las estructuras necesarias:
#include "io-types.h"
#include "io-state.h"

/**
* @brief Inicializa la estructura de configuración de I/O.
* @return Puntero a una estructura t_config_io inicializada.
*/
t_config_io* inicializar_config_io();

/**
 *  @brief Inicializa la configuración de I/O.
 *  @return No devuelve ningún valor.
*/
void io_config_init();

/**
* @brief Configura los valores relacionados con el módulo Kernel en la estructura de configuración de I/O.
* @param config_io Puntero a la estructura de configuración de I/O que se desea completar.
* @param config Configuración general desde donde se obtendrán los valores.
* @return No devuelve ningún valor.
*/
void configurar_valores_de_kernel(t_config_io* config_io, t_config* config);

/**
* @brief Configura los valores relacionados con el módulo de Logging en la estructura de configuración de I/O.
* @param config_io Puntero a la estructura de configuración de I/O que se desea completar.
* @param config Configuración general desde donde se obtendrán los valores.
* @return No devuelve ningún valor.
*/
void configurar_valores_de_log(t_config_io* config_io, t_config* config);

#endif // IO_CONFIG_H