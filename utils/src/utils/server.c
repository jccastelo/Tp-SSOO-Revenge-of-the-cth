#include "server.h"

void server_clients(int server_socket, const char* server_name, void (*connection_strategy)(int, const char* server_name, void (*client_handler)(void*)), void (*client_handler)(void*)) {
    for (;;) {
        int client_socket = esperar_cliente(server_name, server_socket);

        if (client_socket == -1) {
            log_error(logger, "Error en la conexion del cliente");
            continue;
        }

        connection_strategy(client_socket, client_handler);
    }
}

void serve_client_with_thread(int client_socket, const char* server_name, void (*client_handler)(void*)) {
    server_clients(client_socket, server_name, conection_strategy_thread, client_handler);
}

void serve_client_with_unique_thread(int client_socket, const char* server_name, void (*client_handler)(void*)) {
    server_clients(client_socket, server_name, conection_strategy_unique_thread, client_handler);
}

