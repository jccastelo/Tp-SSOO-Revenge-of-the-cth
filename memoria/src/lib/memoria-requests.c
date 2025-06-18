#include "../include/memoria-requests.h"

void init_process (int client_socket) {
    // Inicializamos las variables necesarias para el proceso:
    int id_process;
    int tam_process;
    int resquest = 1;
    char *file_procces;
    
    // Llamamos a la función que recibe y configura los valores para el proceso:
    rcv_setup_to_process(client_socket, &id_process, &tam_process, &file_procces);
    log_info(logger, "PID: %d - Proceso a punto de crearse - Tamanio: %d - Archivo: %s", id_process, tam_process, file_procces);

    // To Do: Deberiamos de validar, si memoria tiene espacio para el proceso

    // Cargar las instrucciones en la estructura administrativa: instrucciones por proceso
    loading_process_instructions(id_process, file_procces);

    // Enviamos respuesta afimartiva que se mando se creo el proceso:
    send(client_socket, &resquest, sizeof(resquest), 0);
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

    // Verificamos si el proceso ya ha finalizado o no:
    if (is_process_end(id_process)) { // To Do: Verificar si el proceso ya ha finalizado, funcion que mezcla consulta y estado del proceso
        log_info(logger, "El proceso %d ya ha finalizado", id_process);
        resquest = OK;
    } else {
        log_info(logger, "El proceso %d no ha finalizado", id_process);
        resquest = ERROR;     
    }

    // Enviamos la respuesta al cliente:
    log_info(logger, "Enviando respuesta al cliente: %d", resquest);
    send(client_socket, &resquest, sizeof(resquest), 0);
}