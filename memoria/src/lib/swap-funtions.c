#include "../include/swap-funtions.h"


void init_swap() { 
    char * ruta = config_memoria->PATH_SWAPFILE;
    archivo_swap = fopen(ruta, "wb");
    if (!archivo_swap) {
        perror("Error creando swap.bin");
        exit(EXIT_FAILURE);
    }
    fclose(archivo_swap);
    
    diccionario_swap_metadata = dictionary_create();

    archivo_swap = fopen(ruta, "rb+");
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

void swap_in(char* pid_key, int pid, int client_socket) {
    // Preparamos respuestas:
    char *status;
    int resquest;
    int index_frames = 0;

    // Obtenemos la lista de entradas (frames) del proceso suspendido desde el diccionario de swap, y calculamos la cantidad total de páginas
    t_list *metadata_swap = remove_marcos_list_of_proc(pid_key, diccionario_swap_metadata);
    aumentar_contador(metricas_por_procesos,SWAP_OUT_REQUESTS, pid_key);
    int cant_paginas = list_size(metadata_swap);

    // Comprobamos si hay memoria suficiente: 
    t_list *marcos_libres = is_memory_sufficient(cant_paginas * config_memoria->TAM_PAGINA);

    if (!marcos_libres) {
        log_error(logger, "No hay memoria suficiente para reanudar proceso %d", pid);
        resquest = ERROR;
        status = "ERROR";
        dictionary_put(diccionario_swap_metadata, pid_key, metadata_swap);
    } else {
        void closure_swap_in(void *entry_swap) {
            // Inicializamos variables:
            swap_entry_t *entry = (swap_entry_t *)entry_swap;
            int free_frame_id = (int)(intptr_t)list_get(marcos_libres, index_frames);
            char *contenido = malloc(config_memoria->TAM_PAGINA);
            index_frames++;

            // Nos preparamos para el leer contenido del archivo, y disponerlo en el espacio de usuario:
            fseek(archivo_swap, entry->offset_swap, SEEK_SET);
            fread(contenido, config_memoria->TAM_PAGINA, 1, archivo_swap);
            memcpy(espacio_usuario + free_frame_id * config_memoria->TAM_PAGINA, contenido, config_memoria->TAM_PAGINA);
            log_info(logger, 
            "Swap_in exitoso: PID %d, página %d cargada en frame %d. Espacio de usuario actual: %s", 
                pid, 
                index_frames, 
                free_frame_id, 
                mem_hexstring(espacio_usuario, config_memoria->TAM_MEMORIA)
            );
        }

        list_iterate(metadata_swap, closure_swap_in);
        resquest = OK;
        status = "OK";

        // Obtenemos la tabla de páginas raíz del proceso, la populamos con marcos libres y actualizamos métricas relacionadas a escrituras en memoria y operaciones de swap in
       // t_list *root_table = get_root_table(pid);
        setup_page_tables( pid, marcos_libres);
        aumentar_contador(metricas_por_procesos, MEM_WRITE_REQUESTS, pid_key);
        aumentar_contador(metricas_por_procesos, SWAP_IN_REQUESTS, pid_key);
    }

    log_info(logger, "Proceso PID %d: swap-in finalizado con estado %s", pid, status);
    send(client_socket, &resquest, sizeof(resquest), 0);
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
