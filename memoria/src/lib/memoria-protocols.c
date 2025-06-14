#include "../include/memoria-protocols.h"

void rcv_setup_to_process(int client_socket, int *id_process, int *tam_process, char **file_procces) {
    // Inicializamos las variables necesarias:
    int size; 
    int desplazamiento = 0;  
    void *buffer = recibir_buffer(&size, client_socket);  

    // Parseamos los primeros valores enteros del buffer, actualizando el desplazamiento en cada paso:
    parsear_int(buffer, &desplazamiento, id_process);
    parsear_int(buffer, &desplazamiento, tam_process); 

    // Parseamos las cadenas de caracteres contenidas en el buffer y las almacenamos en una lista:
    parsear_string(buffer, &desplazamiento, file_procces);
}

void rcv_instruction_consumer(int client_socket, int *id_process, int *program_counter) {
    // Inicializamos las variables necessarias:
    int size;
    int desplazamiento = 0;
    void *buffer = recibir_buffer(&size, client_socket);

    // Parseamos los primeros valores enteros del buffer, actualizando el desplazamiento en cada paso:
    parsear_int(buffer, &desplazamiento, id_process);
    parsear_int(buffer, &desplazamiento, program_counter);
}

void send_instruction_consumer(int cliente_socket, int id_process, int program_counter, char *instruction) {
    // Verificamos que haya instruccion para enviar al cliente.
    // Si no hay instruccion valida, registramos un error y salimos. 
    if (!instruction) {
        log_error(logger, "No se puede enviar la instrucción solicitada por CPU. Program Counter: %d | ID de Proceso: %d", program_counter, id_process);
        return;
    }
 
    t_paquete *instruction_package = crear_paquete(RETURN_INSTRUCCION);
    agregar_a_paquete_string(instruction_package, instruction, strlen(instruction) + 1);
    enviar_paquete(instruction_package, cliente_socket);
    eliminar_paquete(instruction_package);
} 