#ifndef CPU_CACHE_H
#define CPU_CACHE_H

//Librerias externas
#include <utils/logger.h>

//Librerias internas
#include "cpu-state.h"
#include "cpu-config.h"

void inicializar_cache();

void limpiar_cache();

char* buscar_pagina_cache(int pagina);

int conseguir_entrada_libre();

void agregar_en_entrada_cache(int entrada, int pagina, char* contenido);

int elegir_victima_cache();

void aumentar_puntero_clock();

t_algoritmo_cache get_algoritmo_cache(char* algoritmo_str);

#endif //CPU_CACHE_H