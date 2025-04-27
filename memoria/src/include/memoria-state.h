#ifndef MEMORIA_STATE_H_
#define MEMORIA_STATE_H_

// Incluimos las bibliotecas necesarias:
#include <stdlib.h>
#include <utils/logger.h>
#include <commons/collections/dictionary.h>

// Incluimos bibliotecas propias:
#include "memoria-types.h"

// Definimos las variables globales:
extern t_config_memoria *config_memoria;
extern t_memoria_servers *memoria_servers;
extern t_dictionary *instrucciones_por_procesos;
extern t_dictionary *metricas_por_procesos;
extern void *espacio_usuario;

void memoria_state_init();

#endif // CPU_STATE_H_