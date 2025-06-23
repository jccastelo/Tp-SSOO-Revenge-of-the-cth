#include "include/cpu-cache.h"

void inicializar_cache() {
    cache = malloc(sizeof(t_entrada_cache) * config_cpu->ENTRADAS_CACHE);

    for (int i=0; i < config_cpu->ENTRADAS_CACHE; i++) {
        cache[i].pagina = -1;
        cache[i].contenido = malloc(TAM_PAGINA);

        memset(cache[i].contenido, 0, TAM_PAGINA);

        cache[i].bit_uso = 0;
        cache[i].bit_modificado = 0;
        cache[i].libre = 1;
    }

    puntero_clock = 0;
}

void limpiar_cache() {
    for (int i = 0; i < config_cpu->ENTRADAS_CACHE; i++) {
        free(cache[i].contenido);
    }
    free(cache);
}

char* buscar_pagina_cache(int pagina) {
    for (int i=0; i < config_cpu->ENTRADAS_CACHE; i++) {
        if (!cache[i].libre && cache[i].pagina == pagina) {
            cache[i].bit_uso = 1;
            return cache[i].contenido; // TODO LOG CACHE HIT?
        }
    }
    return NULL; // TODO LOG CACHE MISS
}

void agregar_a_cache(int pagina, char* contenido) {
    int entrada_libre = -1;
    for (int i = 0; i < config_cpu->ENTRADAS_CACHE; i++) {
        if (cache[i].libre) {
            entrada_libre = i;
            break;
        }
    }

    if (entrada_libre == -1) {
        entrada_libre = elegir_victima_cache();
    }

    agregar_en_entrada_cache(entrada_libre, pagina, contenido);
}

void agregar_en_entrada_cache(int entrada, int pagina, char* contenido) {
    cache[entrada].pagina = pagina;
    memcpy(cache[entrada].contenido, contenido, TAM_PAGINA);
    cache[entrada].bit_uso = 1;
    cache[entrada].bit_modificado = 0;
    cache[entrada].libre = 0;
}