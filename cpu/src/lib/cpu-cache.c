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

int conseguir_entrada_libre() {
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

    return entrada_libre;
}

void agregar_en_entrada_cache(int entrada, int pagina, char* contenido) {
    cache[entrada].pagina = pagina;
    memcpy(cache[entrada].contenido, contenido, TAM_PAGINA);
    cache[entrada].bit_uso = 1;
    cache[entrada].bit_modificado = 0;
    cache[entrada].libre = 0;
}

int elegir_victima_cache() {
    int victima = -1;
    switch (get_algoritmo_cache(config_cpu->REEMPLAZO_CACHE))
    {
    case CLOCK:
        while (victima == -1) {
            if (cache[puntero_clock].bit_uso == 0)
                victima = puntero_clock;
            else
                cache[puntero_clock].bit_uso = 0;
            aumentar_puntero_clock();
        }
        return victima;

    case CLOCK_M:
        while (1) {
            int entradas = config_cpu->ENTRADAS_CACHE;

            // (U=0,M=0)
            for(int i=0; i < entradas; i++) {
                if (!cache[puntero_clock].bit_uso && !cache[puntero_clock].bit_modificado) {
                    victima = puntero_clock;
                    aumentar_puntero_clock();
                    return victima;
                }
                aumentar_puntero_clock();
            }

            // (U=0, M=1)
            for (int i=0; i < entradas; i++) {
                if (!cache[puntero_clock].bit_uso && cache[puntero_clock].bit_modificado) {
                    victima = puntero_clock;
                    aumentar_puntero_clock();
                    return victima;
                } else {
                    cache[puntero_clock].bit_uso = 0;
                }
                aumentar_puntero_clock();
            }

            // repito :)
        }
    
    default:
        //TODO ERROR
        return -1;
    }
    
}

void aumentar_puntero_clock() {
    puntero_clock = (puntero_clock + 1) % config_cpu->ENTRADAS_CACHE; 
}

t_algoritmo_cache get_algoritmo_cache(char* algoritmo_str) {
    if (string_equals_ignore_case(algoritmo_str, "CLOCK")) return CLOCK;
    else if (string_equals_ignore_case(algoritmo_str, "CLOCK-M")) return CLOCK_M;
    else return -1;
}