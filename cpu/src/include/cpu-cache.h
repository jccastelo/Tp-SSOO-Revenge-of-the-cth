#ifndef CPU_CACHE_H
#define CPU_CACHE_H

//Librerias externas
#include <utils/logger.h>

//Librerias internas
#include "cpu-state.h"
#include "cpu-config.h"

void inicializar_cache();

void limpiar_cache();

bool cache_habilitada();

int buscar_pagina_cache(int pagina);

char* leer_pagina_cache(int entrada);

char* leer_pagina_cache_parcial(int entrada, int offset, int tamanio);

void escribir_pagina_cache(int entrada, int offset, char* contenido);

int conseguir_entrada_libre();

void agregar_en_entrada_cache(int entrada, int pagina, char* contenido);

int elegir_victima_cache();

void aumentar_puntero_clock();

t_algoritmo_cache get_algoritmo_cache(char* algoritmo_str);

#endif //CPU_CACHE_H