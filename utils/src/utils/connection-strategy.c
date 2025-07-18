#include "connection-strategy.h"

void conection_strategy_persistence(void * args) {
    // Inicializamos las variables necesarias para el manejo de la conexión:
    client_args_t* args_t = (client_args_t*) args;
    int client_socket = args_t->client_socket;
    const char* server_name = args_t->server_name;
    void (*client_handler)(int client_socket, int operation, const char *server_name) = args_t->client_handler;

    // Inicializamos la variable para controlar el bucle de atención al cliente:
    int execute_server = 1;

    while (execute_server) {
        // Validamos si el cliente sigue conectado antes de recibir:
        connection_validate(&execute_server, client_socket);
        if (!execute_server) break;

        // Intentamos recibir una operación del cliente:
        int operation = recibir_operacion(client_socket);

        if (operation == -1) 
            log_debug(logger, "Error al recibir la operacion");
        else {
            log_debug(logger, "Operacion recibida: %d", operation);
            client_handler(client_socket, operation, server_name);
        }
    }

    free(args_t);
}

void conection_strategy_once(void * args) {
    // Inicializamos las variables necesarias para el manejo de la conexión:
    client_args_t* args_t = (client_args_t*) args;
    int client_socket = args_t->client_socket;
    const char* server_name = args_t->server_name;
    void (*client_handler)(int client_socket, int operation, const char *server_name) = args_t->client_handler;

    // Recibimos la operación del cliente:
    int operation = recibir_operacion(client_socket);

    // Si la operación es -1, significa que hubo un error al recibir la operación:
    if (operation == -1) 
        log_debug(logger, "Error al recibir la operacion");

    client_handler(client_socket, operation, server_name);

    free(args_t);
}


