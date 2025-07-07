#include "../include/swap-funtions.h"
FILE* archivo_swap = NULL;
t_dictionary* diccionario_swap_metadata = NULL;

void init_swap() {
    archivo_swap = fopen("swap.bin", "wb");
    if (!archivo_swap) {
        perror("Error creando swap.bin");
        exit(EXIT_FAILURE);
    }
    fclose(archivo_swap);
    
    diccionario_swap_metadata = dictionary_create();

    archivo_swap = fopen("swap.bin", "rb+");
    if (!archivo_swap) {
        perror("Error abriendo swap para lectura/escritura");
        exit(EXIT_FAILURE);
    }
}


t_list* get_marcos_list_of_proc(char* pid_key, t_dictionary *tabla_de_paginas) {
    
    t_list* lista_de_marcos = dictionary_get(tabla_de_paginas, pid_key);
    free(pid_key);

    if (lista_de_marcos == NULL) {
        log_warning(logger, "No se encontraron marcos para el proceso PID %d", atoi(pid_key));
        return NULL;
    }
    return lista_de_marcos;
}

t_list* remove_marcos_list_of_proc(char* pid_key , t_dictionary *diccionario_swap){
    t_list* metadata_swap = dictionary_remove(diccionario_swap_metadata, pid_key);

    if (!metadata_swap) {
        log_error(logger, "No hay metadata de swap para el proceso %d", atoi(pid_key));
        free(pid_key);
        return NULL;
    }
    return metadata_swap;
}

void finalizar_swap() {
    
    if (archivo_swap != NULL) {
        fclose(archivo_swap);
        archivo_swap = NULL;
    }
    if (diccionario_swap_metadata != NULL) {
        dictionary_destroy_and_destroy_elements(diccionario_swap_metadata, free);
        diccionario_swap_metadata = NULL;
    }

}
