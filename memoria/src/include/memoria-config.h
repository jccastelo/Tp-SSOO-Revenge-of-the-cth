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

t_config_memoria* inicializar_config_memoria();
void memoria_config_init();
void configurar_valores_de_puerto(t_config_memoria*  t_config* );
void configurar_valores_de_log(t_config_memoria* , t_config* );

#endif //MEMORIA_CONFIG_H