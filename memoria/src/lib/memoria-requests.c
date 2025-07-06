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
        mark_frames_as_busy(free_frames);
        request = OK;
    } else
        request = ERROR;

    // Enviamos la respuesta indicando si el proceso fue creado correctamente o no
    send(client_socket, &request, sizeof(request), 0);
}

void access_to_page_tables(int client_socket) {
    // Se recibe la dirección física, representada por las entradas por nivel asociadas al proceso.
    int id_process;
    t_list *physical_address = rcv_and_parse_memory_access(client_socket, &id_process, NULL, noop_parse_entry);

    // Buscamos el frame correspondiente a partir del ID de proceso y las entradas obtenidas
    int searched_frame = find_frame_from_entries(id_process, physical_address);

    // Enviar el frame encontrado al cliente que lo solicitó, y liberamos memoria
    send(client_socket, &searched_frame, sizeof(int), 0);
    list_destroy(physical_address);
}

void operation_in_user_spaces(int cliente_socket, t_execute_operation execute_operation, parse_func_t parse_fn) {
    // Recibimos la dirección física y obtenemos las entradas por nivel asociadas al proceso
    int offeset = -1;
    int id_process;
    void *extra_data;
    t_list *physical_address = rcv_and_parse_memory_access(cliente_socket, &id_process, extra_data, parse_fn);

    // Buscamos el frame correspondiente a partir del ID de proceso y las entradas obtenidas
    int searched_frame = resolve_physical_frame(id_process, physical_address, &offeset);

    execute_operation(cliente_socket, id_process, searched_frame, extra_data, offeset);
}

void write_in_user_spaces(int client_socket) {
    operation_in_user_spaces(client_socket, write_memory, parsear_string);
}

void read_in_user_spaces(int client_socket) {
    operation_in_user_spaces(client_socket, read_memory, noop_parse_entry);
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