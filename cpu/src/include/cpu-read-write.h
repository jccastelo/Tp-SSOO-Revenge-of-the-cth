#ifndef CPU_READ_WRITE_H
#define CPU_READ_WRITE_H

//Librerias externas
#include <utils/logger.h>

//Librerias internas
#include "cpu-state.h"
#include "cpu-config.h"
#include "cpu-connect.h"

#include "cpu-comunicacion-memoria.h"

#include "mmu.h"
#include "cpu-tlb.h"
#include "cpu-cache.h"

int obtener_direccion_fisica(int direccion_logica);

void leer(int direccion_logica, int tamanio);

void escribir(int direccion_logica, char* contenido);

int busqueda_cache(int nro_pagina);

char* leer_frame_memoria(int nro_pagina);

#endif //CPU_READ_WRITE_H