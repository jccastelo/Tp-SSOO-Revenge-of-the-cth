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
    log_debug(logger, "PID: %d - Proceso a punto de crearse - Tamaño: %d - Archivo: %s",
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
        log_info(logger, "PID: %d , - Proceso creado - Tamaño: %d ", id_process , tam_process);
    } else {
        status_process = "denegado";
        request = ERROR;
    }

    // Enviamos la respuesta indicando si el proceso fue creado correctamente o no
    log_debug(logger, "PID: %d - Proceso %s para crearse", id_process, status_process);
    send(client_socket, &request, sizeof(request), 0);
}

void access_to_page_tables(int client_socket) {
    // Se recibe la dirección física, representada por las entradas por nivel asociadas al proceso.
    int id_process;
    t_list *entries_per_levels = rcv_entries_per_levels(client_socket, &id_process);

    // Logueamos la información del proceso y las entradas recibidas:
    log_info(logger, "PID: %d - Acceso a tablas de páginas", id_process);

    // Buscamos el frame correspondiente a partir del ID de proceso y las entradas obtenidas
    int searched_frame = find_frame_from_entries(id_process, entries_per_levels);

    aumentar_contador(metricas_por_procesos, TABLAS_REQUESTS, string_itoa(id_process));
    log_debug(logger, "PID: %d - Frame encontrado: %d", id_process, searched_frame);

    // Enviar el frame encontrado al cliente que lo solicitó, y liberamos memoria
    send(client_socket, &searched_frame, sizeof(int), 0);
    list_destroy(entries_per_levels);
}

void write_in_user_spaces(int client_socket) {
    int id_process;
    int physical_address;
    char* content_to_write;

    // Inicializamos el espacio de usuario para el proceso:
    rcv_physical_memory_and_content_to_write(client_socket, &id_process, &physical_address, &content_to_write);
    write_memory(client_socket, id_process, content_to_write, physical_address);

    // Logueamos el contenido del espacio de usuario después de la escritura:
    aumentar_contador(metricas_por_procesos, MEM_WRITE_REQUESTS, string_itoa(id_process));
}

void read_in_user_spaces(int client_socket) {
    int id_process;
    int physical_address;
    int quantity_bytes;

    // Inicializamos el espacio de usuario para el proceso:
    rcv_physical_memory_and_quantity_bytes(client_socket, &id_process, &physical_address, &quantity_bytes);
    read_memory(client_socket, id_process, quantity_bytes, physical_address);
    aumentar_contador(metricas_por_procesos, MEM_READ_REQUESTS, string_itoa(id_process));
}

void send_process_instruction(int cliente_socket) {
    // Inicializamos las variables necesarias para el proceso:
    int id_process;
    int program_counter;
    char *instruction;
    
    // Llamamos a la función que recibe y configura los valores necesarios para el proceso. Luego, enviamos la instrucción correspondiente:
    rcv_instruction_consumer(cliente_socket, &id_process, &program_counter);
    log_debug(logger, " ## PID: %d - Obtener instrucción: %d", id_process, program_counter);

    // Obtenemos la instrucción del proceso y la enviamos al consumidor:
    get_instruction(cliente_socket, id_process, program_counter, &instruction);
    send_instruction_consumer(cliente_socket, id_process, program_counter, instruction);
    aumentar_contador(metricas_por_procesos, INSTRS_REQUESTS, string_itoa(id_process));
}


void suspend_process(int client_socket) {
    // Inicializamos variables:
    int id_process = rcv_only_pid(client_socket);
    char *id_process_key = string_itoa(id_process);

    log_info(logger, "El proceso con PID %d ha entrado en estado de suspensión.", id_process);

    // Aumentamos contadores de metricas:
    aumentar_contador(metricas_por_procesos, SWAP_IN_REQUESTS, id_process_key);
    aumentar_contador(metricas_por_procesos, SWAP_OUT_REQUESTS, id_process_key);
    aumentar_contador(metricas_por_procesos, MEM_READ_REQUESTS, id_process_key); //ver si esto se tiene que sumar por cada entrada a un marco, o con solo cambiar listo

    // Obtenemos los marcos ocupados del proceso, y creamos :
    t_list *frames_as_busy_process = get_frames_from_entries(id_process);
    t_list* swap_metadata_proceso = list_create();

    void closure_swap_out(void *frame) {
        int frame_id = (int)(intptr_t)frame;

        // Inicializamos contenido a escribir:
        char *buffer = malloc(config_memoria->TAM_PAGINA);
        memcpy(buffer, espacio_usuario + frame_id * config_memoria->TAM_PAGINA, config_memoria->TAM_PAGINA);

        // Escribimos en el archivo y, al mismo tiempo, inicializamos una variable que indica la posición donde se realizó la escritura.
        int pointer_location = ftell(archivo_swap);
        fwrite(buffer, config_memoria->TAM_PAGINA, 1, archivo_swap);

        // Creamos la entrada de SWAP:
        swap_entry_t* entrada = malloc(sizeof(swap_entry_t));
        entrada->nro_pagina = frame_id;
        entrada->offset_swap = pointer_location;
        list_add(swap_metadata_proceso, entrada);
    }

    // Ejecutamos el swap out sobre los frames ocupados del proceso, liberamos los frames y actualizamos el diccionario de metadata de swap
    list_iterate(frames_as_busy_process, closure_swap_out);
    mark_frames_as_free(frames_as_busy_process);
    dictionary_put(diccionario_swap_metadata, id_process_key, swap_metadata_proceso);
    dictionary_remove(all_process_page_tables, id_process_key);
    
    // Mandamos respuesta a kernel:
    int resquest = OK;
    send(client_socket, &resquest, sizeof(resquest), 0);
}

void remove_suspend_process(int client_socket) {
    int pid = rcv_only_pid(client_socket);
    char* pid_key = string_itoa(pid);
    swap_in(pid_key, pid, client_socket);
}

void dump_process(int client_socket) {
    // Se recibe el PID del proceso que se quiere hacer dump:
    int resquest = OK;
    int id_process = rcv_only_pid(client_socket);
    
    // Logueamos la solicitud de dump:
    log_error(logger, "## PID: %d - Memory Dump solicitado", id_process);

    // Creamos el archivo de dump con el nombre del PID:
    char *filename = string_from_format("%sdump_%d.bin", config_memoria->DUMP_PATH, id_process);
    FILE *archivo_dump = fopen(filename, "wb");

    // Obtenemos el lista de marcos ocupados por el proceso:
    t_list *frames_as_busy = get_frames_from_entries(id_process);

    if(!archivo_dump) {
        log_error(logger, "Error al crear el archivo de dump: %s", filename);
        resquest = ERROR;
        send(client_socket, &resquest, sizeof(resquest), 0);
        return;
    }

    void closure_dump(void *frame) {
        int frame_id = (int)(intptr_t)frame;
        char *buffer = malloc(config_memoria->TAM_PAGINA);

        // Copiamos el contenido del espacio de usuario al buffer:
        memcpy(buffer, espacio_usuario + frame_id * config_memoria->TAM_PAGINA, config_memoria->TAM_PAGINA);
        fwrite(buffer, config_memoria->TAM_PAGINA, 1, archivo_dump);
    }

    // Iteramos sobre los marcos ocupados y escribimos su contenido en el archivo de dump:
    list_iterate(frames_as_busy, closure_dump);
    fclose(archivo_dump);

    // Liberamos la lista de marcos ocupados, y enviamos la respuesta al cliente:
    list_destroy(frames_as_busy);
    send(client_socket, &resquest, sizeof(resquest), 0);
    log_error(logger, "## PID: %d - Memory DUMP ACABADOOOO", id_process);
}

void finish_process(int client_socket) {
    int resquest;
    int pid = rcv_only_pid(client_socket);

    log_debug(logger, "Proceso PID: %d va a finalizar", pid);

    // Verificamos si el proceso ya ha finalizado o no:
    if (is_process_end(pid)) { // To Do: Verificar si el proceso ya ha finalizado, funcion que mezcla consulta y estado del proceso
       log_debug(logger, "El proceso %d ya ha finalizado", pid);
       resquest = OK;
    } else {
       log_debug(logger, "El proceso %d no ha finalizado", pid);
       resquest = ERROR;     
    }

    // Enviamos la respuesta al cliente:
    send(client_socket, &resquest, sizeof(resquest), 0);
}
