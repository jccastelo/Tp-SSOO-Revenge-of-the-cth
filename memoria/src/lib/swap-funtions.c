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

void swap_in(char* pid_key, int pid, int client_socket){
    int resquest ;

    t_list* metadata_swap = remove_marcos_list_of_proc(pid_key , diccionario_swap_metadata);

    int cant_paginas = list_size(metadata_swap);
    t_list* marcos_libres = is_memory_sufficient(cant_paginas * config_memoria->TAM_PAGINA);

    if (!marcos_libres) {
        log_error(logger, "No hay memoria suficiente para reanudar proceso %d", pid);
        list_destroy(metadata_swap);
        resquest = ERROR;
        send(client_socket, &resquest, sizeof(resquest), 0);
        return;
    }

    for (int i = 0; i < cant_paginas; i++) {
        swap_entry_t* entrada = list_get(metadata_swap, i);
        int frame_id = (int)(intptr_t)list_get(marcos_libres, i);

        void* destino = espacio_usuario + frame_id * config_memoria->TAM_PAGINA;
        fseek(archivo_swap, entrada->offset_swap, SEEK_SET);
        fread(destino, config_memoria->TAM_PAGINA, 1, archivo_swap);

        free(entrada);
    }

    t_list* tabla_paginas = list_create();
    populate_page_table(marcos_libres, tabla_paginas);
    dictionary_put(all_process_page_tables, pid_key, tabla_paginas);
    
    resquest = OK;
    send(client_socket, &resquest, sizeof(resquest), 0);

    list_destroy(metadata_swap);
    list_destroy(marcos_libres);
    aumentar_contador(metricas_por_procesos,SWAP_IN_REQUESTS,pid_key) ;
    aumentar_contador(metricas_por_procesos,SWAP_OUT_REQUESTS, pid_key);
    aumentar_contador(metricas_por_procesos, MEM_WRITE_REQUESTS,pid_key);
} 

void vaciar_swap_del_proceso(int pid ,char * pid_key) {

    if (!dictionary_has_key(diccionario_swap_metadata, pid_key)) {
        log_info(logger, "El proceso %d no tenía entradas en swap", pid);
        return;
    }

    t_list* swap_metadata = dictionary_remove(diccionario_swap_metadata, pid_key);

    for (int i = 0; i < list_size(swap_metadata); i++) {
        swap_entry_t* entrada = list_get(swap_metadata, i);
        free(entrada); // solo liberás la estructura, no tocás el archivo
    }

    list_destroy(swap_metadata);
    log_info(logger, "Swap del proceso %d vaciado (fragmentación lógica generada)", pid);
}
