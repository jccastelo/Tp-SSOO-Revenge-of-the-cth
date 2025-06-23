#include "include/cpu-cache.h"

void inicializar_cache() {
    cache = malloc(sizeof(t_entrada_cache) * config_cpu->ENTRADAS_CACHE);

    for (int i = 0; i < config_cpu->ENTRADAS_CACHE; i++) {
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