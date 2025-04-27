#include "../include/memoria-handlers.h"

void memoria_server_escucha_handler(int client_socket, int operation, const char *server_name) {
    if (operation == HANDSHAKE) {
        recibir_handshake(client_socket);
    } else {
        log_error(logger, "Error al recibir la operación del cliente io-handler");
        close(client_socket);
    }
}