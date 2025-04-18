#include "connection-strategy.h"

void conection_strategy_persistence(void * args) {
    // Inicializamos las variables necesarias para el manejo de la conexión:
    client_args_t* args_t = (client_args_t*) args;
    int client_socket = args_t->client_socket;
    char* server_name = args_t->server_name;
    void (*client_handler)(int client_socket, int operation, char *server_name) = args_t->client_handler;

    for(;;) {
        int operation = recibir_operacion(client_socket);

        // Si la operación es -1, significa que hubo un error al recibir la operación:
        if (operation == -1) 
            log_error(logger, "Error al recibir la operacion");

        client_handler(client_socket, operation, server_name);
    }
}

void conection_strategy_once(void * args) {
    // Inicializamos las variables necesarias para el manejo de la conexión:
    client_args_t* args_t = (client_args_t*) args;
    int client_socket = args_t->client_socket;
    char* server_name = args_t->server_name;
    void (*client_handler)(int client_socket, int operation, char *server_name) = args_t->client_handler;

    // Recibimos la operación del cliente:
    int operation = recibir_operacion(client_socket);

    // Si la operación es -1, significa que hubo un error al recibir la operación:
    if (operation == -1) 
        log_error(logger, "Error al recibir la operacion");

    client_handler(client_socket, operation, server_name);
}