#ifndef CPU_CACHE_H
#define CPU_CACHE_H

//Librerias externas
#include <utils/logger.h>

//Librerias internas
#include "cpu-state.h"
#include "cpu-config.h"

void inicializar_cache();

void limpiar_cache();

#endif //CPU_CACHE_H