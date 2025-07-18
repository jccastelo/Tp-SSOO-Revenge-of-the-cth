#ifndef MEMORIA_CONFIG_H
#define MEMORIA_CONFIG_H

// Incluye las bibliotecas necesarias:
#include <stdlib.h>
#include <utils/logger.h>
#include <commons/config.h>
#include <commons/string.h>
#include <string.h>

// Incluye las estructuras necesarias:
#include "memoria-types.h"
#include "memoria-state.h"

/**
 * @brief Crea e inicializa una nueva estructura de configuración de memoria.
 * 
 * Esta función reserva memoria dinámica para un objeto `t_config_memoria` e inicializa todos sus campos
 * a valores por defecto. Los campos numéricos se establecen en 0 y los campos de tipo cadena
 * (`char*`) se inicializan en `NULL` para evitar accesos inválidos.
 * 
 * @return t_config_memoria* Puntero a la estructura `t_config_memoria` inicializada.
 * 
 * @note Es responsabilidad del llamador liberar la memoria reservada mediante `free`
 *       cuando la estructura ya no sea necesaria.
 */
t_config_memoria* inicializar_config_memoria(void);

/**
 * @brief Inicializa la configuración de la memoria.
 * 
 * Esta función se encarga de preparar la configuración inicial necesaria para el módulo de memoria
 * del sistema. Carga los parámetros desde un archivo de configuración externo ("memoria.config"),
 * valida su existencia y asigna los valores correspondientes en la estructura global `config_memoria`.
 * 
 * Si el archivo de configuración no se encuentra o no puede abrirse, el programa finalizará con un error.
 * 
 * Generalmente, esta función se invoca durante la fase de inicialización del proceso de Memoria.
 */
 void memoria_config_init(char *config_path);
 
/**
 * @brief Configura el valor del puerto de escucha de la memoria.
 * 
 * Lee el parámetro "PUERTO_ESCUCHA" del archivo de configuración y lo asigna a `config_memoria`.
 */
 void configurar_valores_de_puerto(t_config_memoria* config_memoria, t_config* config);

 /**
  * @brief Configura los tamaños de memoria, página y estructura de tablas.
  * 
  * Lee los valores relacionados a tamaños desde el archivo de configuración
  * y los asigna a los campos correspondientes en `config_memoria`.
  */
 void configurar_valores_de_tamanios(t_config_memoria* config_memoria, t_config* config);
 
 /**
  * @brief Configura las rutas de archivos utilizadas por el módulo de memoria.
  * 
  * Asigna las rutas para el archivo de swap y los archivos de dump a `config_memoria`.
  */
 void configurar_valores_de_paths(t_config_memoria* config_memoria, t_config* config);
 
 /**
  * @brief Configura el nivel de log del módulo de memoria.
  * 
  * Lee y asigna el nivel de logeo que utilizará la memoria para registrar eventos.
  */
 void configurar_valores_de_log(t_config_memoria* config_memoria, t_config* config);

/**
 * @brief Libera la memoria asociada a una estructura `t_config_memoria`.
 * 
 * Esta función se encarga de liberar todos los recursos dinámicamente reservados
 * dentro de la estructura `t_config_memoria`, incluyendo los campos de tipo `char*`,
 * y finalmente libera la estructura principal.
 * 
 * Se recomienda llamar a esta función una vez que la configuración ya no es necesaria,
 * para evitar fugas de memoria.
 * 
 * @param config Puntero a la estructura `t_config_memoria` a liberar. Si es `NULL`, la función no realiza ninguna acción.
 */
void destroy_config_memoria(t_config_memoria* config);
 
#endif //MEMORIA_CONFIG_H