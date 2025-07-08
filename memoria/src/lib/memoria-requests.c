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
    log_info(logger, "PID: %d - Proceso %s para crearse", id_process, status_process);
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

void finish_process(int client_socket) {
    // Inicializamos las variables necesaria para el proceso:
    int id_process;
    int resquest = OK;

    // Llamos a la función que recibe y configura los valores necesarios para el proceso.
    rcv_process_to_end(client_socket, &id_process);
    log_info(logger, "Finalizar proceso: ## PID: %d", id_process);

    //// Verificamos si el proceso ya ha finalizado o no:
    //if (is_process_end(id_process)) { // To Do: Verificar si el proceso ya ha finalizado, funcion que mezcla consulta y estado del proceso
    //    log_info(logger, "El proceso %d ya ha finalizado", id_process);
    //    resquest = OK;
    //} else {
    //    log_info(logger, "El proceso %d no ha finalizado", id_process);
    //    resquest = ERROR;     
    //}

    // Enviamos la respuesta al cliente:
    log_info(logger, "Enviando respuesta al cliente: %d", resquest);
    send(client_socket, &resquest, sizeof(resquest), 0);
}