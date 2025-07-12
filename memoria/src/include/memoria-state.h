#ifndef MEMORIA_STATE_H_
#define MEMORIA_STATE_H_

// Incluimos las bibliotecas necesarias:
#include <stdlib.h>
#include <utils/logger.h>
#include <commons/collections/dictionary.h>
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
 * @brief Maneja el cierre ordenado del programa ante la señal SIGINT (Ctrl+C).
 *
 * Esta función se ejecuta cuando el programa recibe la señal SIGINT y realiza
 * la liberación ordenada de todos los recursos utilizados, asegurando evitar fugas de memoria
 * o archivos abiertos. Las acciones realizadas son:
 *
 * - Cerrar el archivo de swap abierto.
 * - Liberar la memoria reservada para el espacio de usuario.
 * - Destruir el bitmap de frames.
 * - Destruir los diccionarios de métricas e instrucciones por proceso.
 * - Recorrer y destruir todas las tablas de páginas de los procesos.
 * - Destruir la configuración general de memoria.
 *
 * @param signal Señal recibida (se espera SIGINT).
 */
void cerrar_programa(int signal);

/**
 * @brief Configura el manejador de la señal SIGINT para realizar un cierre ordenado.
 *
 * Esta función establece `cerrar_programa` como el handler de la señal SIGINT,
 * que se genera al presionar Ctrl+C. Se utiliza la estructura `sigaction`
 * para registrar la función y limpiar correctamente la máscara de señales.
 *
 * Si ocurre un error al registrar el handler, se imprime un mensaje de error
 * y el programa finaliza con código de error 1.
 */
void configurar_senial_cierre();


#endif // CPU_STATE_H_