#include "server.h"

void server_clients(
    int server_socket,
    const char* server_name,
    void (*connection_strategy)(void *),
    void (*server_strategy)(int, const char* server_name, void (*connection_strategy)(void *args), void (*client_handler)(int, int, const char *)),
    void (*client_handler)(int client_socket, int operation, const char *server_name)
) {
    // Inicializamos el logger:
    log_info(logger, "Esperando conexiones en %s", server_name);

    while (1) {
        int client_socket = esperar_cliente(server_name, server_socket);

        if (client_socket == -1) {
            log_error(logger, "Error en la conexión del cliente");
            continue;
        }

        server_strategy(client_socket, server_name, connection_strategy, client_handler);
    }
}

void server_strategy_thread(
    int client_socket,
    const char* server_name,
    void (*connection_strategy)(void *args),
    void (*client_handler)(int client_socket, int operation, const char *server_name)
) {
    // Inicializamos variables necesarias para el manejo de la conexión:
    pthread_t thread;
    client_args_t* args = malloc(sizeof(client_args_t));
    args->client_socket = client_socket;
    args->server_name = server_name;
    args->client_handler = client_handler;

    // Inicializamos el hilo para manejar la conexión del cliente:
    pthread_create(&thread, NULL, (void*)connection_strategy, (void*)args);
    pthread_detach(thread);
}

void server_strategy_unique_thread(
    int client_socket,
    const char* server_name,
    void (*connection_strategy)(void *args),
    void (*client_handler)(int client_socket, int operation, const char *server_name)
) {
    // Inicializamos variables necesarias para el manejo de la conexión:
    client_args_t* args = malloc(sizeof(client_args_t));
    args->client_socket = client_socket;
    args->server_name = server_name;
    args->client_handler = client_handler;

    connection_strategy((void*)args);

    // Liberamos la memoria asignada para los argumentos:
    free(args);
}

void server_with_thread(
    int server_socket, 
    const char* server_name, 
    void (*connection_strategy)(void *args),
    void (*client_handler)(int client_socket, int operation, const char *server_name)
) {
    server_clients(server_socket, server_name, connection_strategy, server_strategy_thread, client_handler);
}

void server_with_unique_thread(
    int server_socket, 
    const char* server_name, 
    void (*connection_strategy)(void *args),
    void (*client_handler)(int client_socket, int operation, const char *server_name)
) {
    server_clients(server_socket, server_name, connection_strategy, server_strategy_unique_thread, client_handler);
}
