#ifndef CPU_COMUNICACION_MEMORIA_H
#define CPU_COMUNICACION_MEMORIA_H


#include <utils/logger.h>
#include <utils/protocolo.h>

#include "cpu-types.h"
#include "cpu-state.h"

#include "../include/cpu-comunicacion-memoria.h"

void conseguir_siguiente_instruccion();
char* devolver_instruccion_a_ejecutar();
char* deserializar_instruccion(void* buffer);

#endif // CPU_COMUNICACION_MEMORIA_H_