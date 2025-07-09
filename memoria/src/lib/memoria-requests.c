#include "../include/memoria-requests.h"

void init_process(int client_socket) {
    // Inicializamos las variables necesarias para el proceso
    int id_process;
    int tam_process;
    int request;
    char *file_process;
    char *status_process;

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
        mark_frames_as_busy(free_frames);
        status_process = "aceptado";
        request = OK;
    } else {
        status_process = "denegado";
        request = ERROR;
    }

    // Enviamos la respuesta indicando si el proceso fue creado correctamente o no
    log_warning(logger, "PID: %d - Proceso %s para crearse", id_process, status_process);
    send(client_socket, &request, sizeof(request), 0);
}

void access_to_page_tables(int client_socket) {
    // Se recibe la dirección física, representada por las entradas por nivel asociadas al proceso.
    int id_process;
    t_list *entries_per_levels = rcv_entries_per_levels(client_socket, &id_process);
    int size_entries_per_levels = list_size(entries_per_levels);
    log_info(logger, "PID: %d - Cantidad de entradas por nivel: %d", id_process, size_entries_per_levels);

    // Buscamos el frame correspondiente a partir del ID de proceso y las entradas obtenidas
    int searched_frame = find_frame_from_entries(id_process, entries_per_levels);
    log_info(logger, "PID: %d - Frame encontrado: %d", id_process, searched_frame);

    // Enviar el frame encontrado al cliente que lo solicitó, y liberamos memoria
    send(client_socket, &searched_frame, sizeof(int), 0);
    list_destroy(entries_per_levels);
}

void write_in_user_spaces(int client_socket) {
    int id_process;
    int physical_address;
    char* content_to_write;

    rcv_physical_memory_and_content_to_write(client_socket, &id_process, &physical_address, &content_to_write);
    write_memory(client_socket, id_process, content_to_write, physical_address);
}

void read_in_user_spaces(int client_socket) {
    int id_process;
    int physical_address;
    int quantity_bytes;

    rcv_physical_memory_and_quantity_bytes(client_socket, &id_process, &physical_address, &quantity_bytes);
    read_memory(client_socket, id_process, quantity_bytes, physical_address);
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


void suspend_process(int client_socket) {
    
    int pid = rcv_only_pid(client_socket);
    log_info(logger, "supendiendoProc %d", pid);
    char* pid_key = string_itoa(pid);

    aumentar_contador(metricas_por_procesos,SWAP_IN_REQUESTS,pid_key);
    aumentar_contador(metricas_por_procesos,SWAP_OUT_REQUESTS, pid_key);
    aumentar_contador(metricas_por_procesos, MEM_READ_REQUESTS,pid_key); //ver si esto se tiene que sumar por cada entrada a un marco, o con solo cambiar listo


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
        eliminar_marco(frame_id,pid);
    }
    dictionary_put(diccionario_swap_metadata, pid_key, swap_metadata_proceso);
    destruir_tabla_de_paginas(pid);
    free(pid_key);
}


void remove_suspend_process(int client_socket) {
    
    int pid = rcv_only_pid(client_socket);
    char* pid_key = string_itoa(pid);
    swap_in(pid_key, pid);
    free(pid_key);
    log_info(logger, "OK");
}

void dump_process(int client_socket){
    int pid = rcv_only_pid(client_socket);
    char* pid_key = string_itoa(pid);
    
    
    if(!estaEn(all_process_page_tables,pid_key)){
        log_info(logger , "el proceso %d no está en memoria", pid);
        log_info(logger, "buscando el proceso %d en swap", pid);
        if(!estaEn(diccionario_swap_metadata, pid_key)){
            log_error(logger, "el proceso pid %d no existe", pid);
            return;
        }
        swap_in(pid_key, pid); 
        
    }

    char filename[64];
    sprintf(filename, "proceso_%d.dump", pid);
    FILE* archivo_dump = fopen(filename, "wb");
    
    t_list* lista_de_marcos = get_marcos_list_of_proc(pid_key, all_process_page_tables);

    for (int i = 0; i < list_size(lista_de_marcos); i++) {
        int frame_id = (int)(intptr_t)list_get(lista_de_marcos, i);
        void* contenido = espacio_usuario + frame_id * config_memoria->TAM_PAGINA;
        fwrite(contenido, config_memoria->TAM_PAGINA, 1, archivo_dump);
    }

    fclose(archivo_dump);
    free(pid_key);
    free(lista_de_marcos);
    log_info(logger, "## PID: %d - Memory Dump solicitado", pid);

}

void finish_process(int client_socket) {

    int pid = rcv_only_pid(client_socket);
    char* pid_key = string_itoa(pid);
    
    
    if(estaEn(all_process_page_tables,pid_key)){   
        t_list* lista_de_marcos = dictionary_get(all_process_page_tables, pid_key);
        for (int i = 0; i < list_size(lista_de_marcos); i++) {
            int frame_id = (int)(intptr_t)list_get(lista_de_marcos, i);
            bitarray_clean_bit(frames_bitmap, frame_id);
            eliminar_marco(frame_id, pid);
        }
        destruir_tabla_de_paginas(pid);
    }
    vaciar_swap_del_proceso(pid , pid_key);
   
    imprimir_contadores_del_proceso( metricas_por_procesos , pid_key);
    free(pid_key);
  /*  // Inicializamos las variables necesaria para el proceso:
    int id_process;
    int resquest = OK;

    // Llamos a la función que recibe y configura los valores necesarios para el proceso.
    rcv_process_to_end(client_socket, &id_process);
    log_info(logger, "Finalizar proceso: ## PID: %d", id_process);

    // Verificamos si el proceso ya ha finalizado o no:
    if (is_process_end(id_process)) { // To Do: Verificar si el proceso ya ha finalizado, funcion que mezcla consulta y estado del proceso
       log_info(logger, "El proceso %d ya ha finalizado", id_process);
       resquest = OK;
    } else {
       log_info(logger, "El proceso %d no ha finalizado", id_process);
       resquest = ERROR;     
    }

    // Enviamos la respuesta al cliente:
  //  log_info(logger, "Enviando respuesta al cliente: %d", resquest);
//    send(client_socket, &resquest, sizeof(resquest), 0);
*/
}