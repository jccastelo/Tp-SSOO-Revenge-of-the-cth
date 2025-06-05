#include "../include/memoria-handlers.h"

void memoria_server_escucha_handler(int client_socket, int operation, const char *server_name) {
    switch (operation) {
        case HANDSHAKE:
            log_info(logger, "Se ha recibido un handshake.");
            recibir_handshake(client_socket);
        break;
        case INIT_PROC:
            log_info(logger, "Kernel solicito proceso");
            init_process(client_socket);
        break;
        // Otros casos de operaciones pueden ir aquí.
        default:
            log_warning(logger, "Operación desconocida: %d", operation);
        break;
    }
}