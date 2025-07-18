#ifndef MEMORIA_STATE_H_
#define MEMORIA_STATE_H_

// Incluimos las bibliotecas necesarias:
#include <stdlib.h>
#include <utils/logger.h>
#include <commons/collections/dictionary.h>
#include <commons/collections/list.h>
#include <signal.h>

// Incluimos bibliotecas propias:
#include "memoria-user-spaces.h"
#include "memoria-types.h"
#include "swap-funtions.h"
#include "memoria-pages-tables.h"
#include "memoria-config.h"

// Definimos las variables globales:
extern t_config_memoria *config_memoria;
extern t_memoria_servers *memoria_servers;
extern t_dictionary *instrucciones_por_procesos;
extern t_dictionary *metricas_por_procesos;
extern t_dictionary *all_process_page_tables;
extern t_bitarray *frames_bitmap;
extern void *espacio_usuario;
extern void *frames_memory;
extern int quantity_frames;
extern t_dictionary* diccionario_swap_metadata; 
extern FILE* archivo_swap;

/**
 * @brief Inicializa el estado del servidor de Memoria.
 * 
 * Esta función se encarga de configurar el estado inicial del servidor de Memoria, incluyendo la 
 * preparación de cualquier recurso necesario para su operación. Esto puede involucrar la inicialización 
 * de estructuras de datos internas, la asignación de memoria, y la configuración de parámetros necesarios 
 * para que el servidor pueda gestionar correctamente las solicitudes que recibirá a lo largo de su ciclo 
 * de vida.
 */
void memoria_state_init(void);

/**
 * @brief Libera todos los recursos utilizados por el módulo de memoria y finaliza el programa.
 *
 * Esta función realiza un cierre ordenado del módulo de memoria cuando se recibe una señal de terminación
 * (como SIGINT). Libera estructuras dinámicas, destruye diccionarios, libera memoria asignada,
 * cierra archivos y evita fugas de recursos.
 *
 * Las acciones realizadas incluyen:
 * - Liberación de instrucciones por proceso.
 * - Liberación de métricas por proceso.
 * - Liberación de metadatos de swap.
 * - Destrucción recursiva de tablas de páginas por proceso.
 * - Destrucción de la configuración de memoria.
 * - Liberación del bitmap de frames.
 * - Cierre del archivo de swap.
 * - Liberación de los bloques de memoria reservados.
 *
 * @param signal Señal recibida que dispara la rutina de apagado (por convención, SIGINT).
 */
void memory_handle_shutdown(int signal);

/**
 * @brief Configura el manejador de señal para el apagado controlado del módulo de memoria.
 *
 * Registra la función `cerrar_programa` como handler de la señal SIGINT (Ctrl+C), utilizando la estructura `sigaction`.
 * Esta configuración permite interceptar interrupciones del usuario y realizar un cierre ordenado de la memoria
 * en lugar de finalizar abruptamente.
 *
 * Si ocurre un error al registrar el handler, se imprime un mensaje de error por consola y el programa se termina
 * inmediatamente con código de salida 1.
 */
void memory_setup_shutdown_signal();

#endif // CPU_STATE_H_