#include "include/kernel-escucha.h"

void kernel_server_io_handler(int client_socket, int operation, const char *server_name) {
    if (operation == HANDSHAKE) {
        recibir_handshake(client_socket);
    } else 
        log_error(logger, "Operación no válida para el servidor IO: %d", operation);
    
    return;
}

void kernel_server_interrupt_handler(int client_socket, int operation, const char *server_name) {
    if (operation == HANDSHAKE) {
        recibir_handshake(client_socket);
    } else 
        log_error(logger, "Operación no válida para el servidor Interrupt: %d", operation);
    
    return;
}

void kernel_server_dispatch_handler(int cpu_socket, int operation, const char *server_name) {

    t_buffer* new_buffer = malloc(sizeof(t_buffer));
    new_buffer->size = 0;
    new_buffer->stream = NULL;

    new_buffer->stream = recibir_buffer(&new_buffer->size, cpu_socket);

    switch(operation)
    {
    case HANDSHAKE:
        recibir_handshake(cpu_socket);
        break;

    case CPU_ID:
        iniciar_cpu(new_buffer,cpu_socket);
        log_info(logger,"Se recibio la ID de la CPU desde el server %s",server_name);
        break;

    case INIT_PROC:
        recibir_proceso(new_buffer,cpu_socket);
        log_info(logger,"Se recibio la syscall INIC_PROC desde el server %s",server_name);
        break;

    case DUMP_MEMORY:
    
        log_info(logger,"Se recibio la syscall INIC_PROC desde el server %s",server_name);
        break;

    case IO:

        log_info(logger,"Se recibio la syscall IO desde el server %s",server_name);
        break;

     case EXIT_Sys:
        delate_process(new_buffer,cpu_socket);
        set_cpu(cpu_socket, DISPONIBLE);
         
        log_info(logger,"Se recibio la syscall EXIT_Sys desde el server %s",server_name);
        break;

    default:
        log_error(logger, "Operación no válida para el servidor INTERRUPT: %d", operation);
        break;
    }

    free(new_buffer);
    
    return;
}
