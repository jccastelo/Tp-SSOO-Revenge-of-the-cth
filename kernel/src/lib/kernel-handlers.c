#include "include/kernel-handlers.h"

void kernel_server_io_handler(int client_socket, int operation, char *server_name) {
    if (operation == HANDSHAKE) {
        recibir_handshake(client_socket);
    } else {
        log_error(logger, "Error al recibir la operación del cliente");
        close(client_socket);
    }
}