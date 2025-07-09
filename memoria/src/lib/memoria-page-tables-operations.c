#include "../include/memoria-page-tables-operations.h"

void recorrer_arbol_rec(t_list* nodo, accion_sobre_nodo_t accion) {
    for (int i = 0; i < list_size(nodo); i++) {
        void* elem = list_get(nodo, i);
        if (elem != NULL && list_size((t_list*)elem) >= 0) {
            recorrer_arbol_rec((t_list*)elem, accion);
        }
    }
    accion(nodo);
}

void recorrer_arbol_paginas(t_list* raiz, accion_sobre_nodo_t accion) {
    recorrer_arbol_rec(raiz, accion);
}


void eliminar_marco(int frame_id, char* pid_key) {
    int total_levels = config_memoria->CANTIDAD_NIVELES;
    int entries_per_level = config_memoria->ENTRADAS_POR_TABLA;
    int divisors[total_levels];
    precompute_divisors(total_levels, entries_per_level, divisors);

    t_list* current_node = dictionary_get(all_process_page_tables, pid_key);

    if (!current_node) return;

    for (int level = 0; level < total_levels - 1; level++) {
        int index = (frame_id / divisors[level + 1]) % entries_per_level;
        if (index >= list_size(current_node)) return;
        current_node = list_get(current_node, index);
        if (!current_node) return;
    }

    for (int i = 0; i < list_size(current_node); i++) {
        int marco_en_hoja = (int)(intptr_t)list_get(current_node, i);
        if (marco_en_hoja == frame_id) {
            list_remove(current_node, i);
            return;
        }
    }
}

void destruir_nodo(t_list* nodo) {
    for (int i = 0; i < list_size(nodo); i++) {
        void* elem = list_get(nodo, i);
        if (elem != NULL && list_size((t_list*)elem) >= 0) {
            destruir_nodo((t_list*)elem);
        }
    }
    list_destroy(nodo);
}

void destruir_tabla_de_paginas(char *pid_key) {
    t_list* tabla = dictionary_remove(all_process_page_tables, pid_key);

    if (tabla != NULL) {
        destruir_nodo(tabla);
    }
}