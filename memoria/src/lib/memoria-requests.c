#include "../include/memoria-requests.h"

void init_process(int client_socket) {
    // Inicializamos las variables necesarias para el proceso
    int id_process;
    int tam_process;
    int request;
    char *file_process;  

    // Recibimos y configuramos los valores para el proceso
    rcv_setup_to_process(client_socket, &id_process, &tam_process, &file_process);
    log_info(logger, "PID: %d - Proceso a punto de crearse - Tamaño: %d - Archivo: %s",
            id_process, tam_process, file_process);

    // Verificamos si hay suficiente espacio en memoria para el proceso
    t_list *free_frames = is_memory_sufficient(tam_process);

    if(free_frames) {
        loading_process_instructions(id_process, file_process);
        add_process_to_memory(id_process);
        setup_page_tables(id_process, free_frames);
        request = OK;
    } else
        request = ERROR;

    // Enviamos la respuesta indicando si el proceso fue creado correctamente o no
    send(client_socket, &request, sizeof(request), 0);
}

void send_process_instruction(int cliente_socket) {
    // Inicializamos las variables necesarias para el proceso:
    int id_process;
    int program_counter;
    char *instruction;
    
    // Llamamos a la función que recibe y configura los valores necesarios para el proceso. Luego, enviamos la instrucción correspondiente:
    rcv_instruction_consumer(cliente_socket, &id_process, &program_counter);
    log_info(logger, "Obtener instrucción: ## PID: %d - Obtener instrucción: %d", id_process, program_counter);

    // Obtenemos la instrucción del proceso y la enviamos al consumidor:
    get_instruction(cliente_socket, id_process, program_counter, &instruction);
    send_instruction_consumer(cliente_socket, id_process, program_counter, instruction);
}

// === Suspender Proceso ===
void suspend_process(int client_socket) {
    int pid = rcv_only_pid(client_socket);
    char* pid_key = string_itoa(pid);

    t_list* lista_de_marcos = get_marcos_list_of_proc(pid_key, all_process_page_tables);
 

    t_list* swap_metadata_proceso = list_create();

    for (int i = 0; i < list_size(lista_de_marcos); i++) {
        int frame_id = (int)(intptr_t)list_get(lista_de_marcos, i);

        void* contenido = espacio_usuario + frame_id * config_memoria->TAM_PAGINA;
        int offset = ftell(archivo_swap);
        fwrite(contenido, config_memoria->TAM_PAGINA, 1, archivo_swap);

        swap_entry_t* entrada = malloc(sizeof(swap_entry_t));
        entrada->nro_pagina = i;
        entrada->offset_swap = offset;
        list_add(swap_metadata_proceso, entrada);

        bitarray_clean_bit(frames_bitmap, frame_id);
        eliminar_marco_de_mega_arbol(frame_id);
    }

    dictionary_put(diccionario_swap_metadata, pid_key, swap_metadata_proceso);
    destruir_tabla_de_paginas(pid);
    free(pid_key);
}


void remove_suspend_process(int client_socket) {
    int pid = rcv_only_pid(client_socket);
    char* pid_key = string_itoa(pid);
    t_list* metadata_swap = remove_marcos_list_of_proc(pid_key , diccionario_swap_metadata);

    int cant_paginas = list_size(metadata_swap);
    t_list* marcos_libres = is_memory_sufficient(cant_paginas * config_memoria->TAM_PAGINA);

    if (!marcos_libres) {
        log_error(logger, "No hay memoria suficiente para reanudar proceso %d", pid);
        list_destroy(metadata_swap);
        free(pid_key);
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

    list_destroy(metadata_swap);
    list_destroy(marcos_libres);
    free(pid_key);
}





void finish_process(int client_socket) {
    // Inicializamos las variables necesaria para el proceso:
    int id_process;
    int resquest;

    // Llamos a la función que recibe y configura los valores necesarios para el proceso.
    rcv_process_to_end(client_socket, &id_process);
    log_info(logger, "Finalizar proceso: ## PID: %d", id_process);

    // // Verificamos si el proceso ya ha finalizado o no:
    // if (is_process_end(id_process)) { // To Do: Verificar si el proceso ya ha finalizado, funcion que mezcla consulta y estado del proceso
    //     log_info(logger, "El proceso %d ya ha finalizado", id_process);
    //     resquest = OK;
    // } else {
    //     log_info(logger, "El proceso %d no ha finalizado", id_process);
    //     resquest = ERROR;     
    // }

    // Enviamos la respuesta al cliente:
    log_info(logger, "Enviando respuesta al cliente: %d", resquest);
    send(client_socket, &resquest, sizeof(resquest), 0);
}