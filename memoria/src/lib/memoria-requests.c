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
    get_instruction(cliente_socket, id_process, program_counter, &instruction);
    send_instruction_consumer(cliente_socket, id_process, program_counter, instruction);
}