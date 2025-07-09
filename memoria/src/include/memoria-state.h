#ifndef MEMORIA_STATE_H_
#define MEMORIA_STATE_H_

// Incluimos las bibliotecas necesarias:
#include <stdlib.h>
#include <utils/logger.h>
#include <commons/collections/dictionary.h>

// Incluimos bibliotecas propias:
#include "memoria-types.h"
#include "memoria-user-spaces.h"
#include "swap-funtions.h"
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


#endif // CPU_STATE_H_