#include "include/kernel-handlers.h"

void kernel_server_io_handler(int client_socket, int operation, char *server_name) {
    if (operation == HANDSHAKE) {
        recibir_handshake(client_socket);
    } else 
        log_error(logger, "Operación no válida para el servidor IO: %d", operation);
}

void kernel_server_dispatch_handler(int client_socket, int operation, char *server_name) {
    if (operation == HANDSHAKE) {
        recibir_handshake(client_socket);
    } else 
        log_error(logger, "Operación no válida para el servidor DISPATCH: %d", operation);
}

void kernel_server_interrupt_handler(int client_socket, int operation, char *server_name) {
    if (operation == HANDSHAKE) {
        recibir_handshake(client_socket);
    } else 
        log_error(logger, "Operación no válida para el servidor INTERRUPT: %d", operation);
}