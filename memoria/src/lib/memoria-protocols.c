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
    free(buffer);
}

void rcv_instruction_consumer(int client_socket, int *id_process, int *program_counter) {
    // Inicializamos las variables necessarias:
    int size;
    int desplazamiento = 0;
    void *buffer = recibir_buffer(&size, client_socket);

    // Parseamos los primeros valores enteros del buffer, actualizando el desplazamiento en cada paso:
    parsear_int(buffer, &desplazamiento, id_process);
    parsear_int(buffer, &desplazamiento, program_counter);
    free(buffer);
}

void rcv_physical_memory_and_parse_memory_access(int client_socket, int *id_process, int *physical_memory, void* extra_data, parse_func_t parse_fn) {
    // Inicializamos las variables necessarias:
    int size;
    int desplazamiento = 0;
    void *buffer = recibir_buffer(&size, client_socket);

    // Extraemos el ID del proceso, datos adicionales y las entradas por nivel del buffer recibido
    parsear_int(buffer, &desplazamiento, id_process);
    parse_fn(buffer, &desplazamiento, extra_data);
    parsear_int(buffer, &desplazamiento, physical_memory);
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

void rcv_process_to_end(int client_socket, int *id_process) {
    // Inicializamos las variables necesarias:
    int size;
    int desplazamiento = 0;
    void *buffer = recibir_buffer(&size, client_socket);

    // Parseamos el ID del proceso del buffer:
    parsear_int(buffer, &desplazamiento, id_process);
    free(buffer);
}

void send_read_content(int client_socket, char *buffer, int response) {
    t_paquete *response_package = crear_paquete(response);

    if(response == OK)
        agregar_a_paquete_string(response_package, buffer, string_length(buffer));
    
    enviar_paquete(response_package, client_socket);
    eliminar_paquete(response_package);
}

t_list *rcv_entries_per_levels(int client_socket, int *id_process) {
    // Inicializamos las variables necessarias:
    int size;
    int desplazamiento = 0;
    void *buffer = recibir_buffer(&size, client_socket);

    parsear_int(buffer, &desplazamiento, id_process);
    t_list *entries_per_level = parsear_ints(buffer, &desplazamiento, size);

    return entries_per_level;
}

void send_values_memory(int client_socket) {
    t_paquete *values_packages = crear_paquete(MEMORY_CONFIG);
    agregar_a_paquete(values_packages, &config_memoria->TAM_PAGINA, sizeof(int));
    agregar_a_paquete(values_packages, &config_memoria->ENTRADAS_POR_TABLA, sizeof(int));
    agregar_a_paquete(values_packages, &config_memoria->CANTIDAD_NIVELES, sizeof(int));
    enviar_paquete(values_packages, client_socket);
    eliminar_paquete(values_packages);
}