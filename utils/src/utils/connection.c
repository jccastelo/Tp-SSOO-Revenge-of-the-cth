#include "connection.h"

void setup_server(char *server_name, char *ip, char *puerto, void (*callback)(int id_server)) {
    int id_server;
    id_server = iniciar_servidor(server_name, ip, puerto);
    
    if(id_server == -1) {
        log_error(logger, "Error al iniciar el servidor %s", server_name);
        exit(EXIT_FAILURE);
    }

    // Llamamos al callback con el id del servidor, donde se puede asignar el id_server:
    log_info(logger, "Servidor %s iniciado con id %d", server_name, id_server);
    callback(id_server);
}

void setup_connection_with_server(char *server_name, char *ip, char *puerto, void (*callback)(int socket_client)) {
    int socket_client;
    socket_client = crear_conexion(server_name, ip, puerto);

    if(socket_client == -1) {
        log_error(logger, "Error al conectar con el servidor %s", server_name);
        exit(EXIT_FAILURE);
    }

    // Llamamos al callback con el socket del cliente, donde se puede asignar el socket_client:
    callback(socket_client);
}

void connection_validate(int *execute_server, int client_socket) {
    char buffer;
    int resultado = recv(client_socket, &buffer, sizeof(buffer), MSG_PEEK | MSG_DONTWAIT);

    if (resultado > 0) {
        log_info(logger, "Cliente sigue conectado");
        return;
    }

    if (resultado == 0) {
        log_error(logger, "Cliente desconectado (cerró la conexión)");
    } else {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            log_info(logger, "Cliente aún conectado (sin datos disponibles por ahora)");
            return;
        }
        
        log_error(logger, "Error al verificar la conexión: %s", strerror(errno));
    }

    *execute_server = 0;
}
