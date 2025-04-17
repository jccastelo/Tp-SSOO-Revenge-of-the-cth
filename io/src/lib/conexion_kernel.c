#include "../include/conexion_kernel.h"

int crear_conexion(const char* server_name, char* ip, char* puerto) {

    struct addrinfo hints, *servinfo;

    // Init de hints
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    // Recibe addrinfo
    getaddrinfo(ip, puerto, &hints, &servinfo);

    // Crea un socket con la informacion recibida (del primero, suficiente)
    int socket_cliente = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

    // Fallo en crear el socket
    if(socket_cliente == -1) {
        log_error(logger, "Error creando el socket para %s:%s", ip, puerto);
        return -1;
    }

    // Error conectando
    if(connect(socket_cliente, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
        log_error(logger, "Error al conectar (a %s)\n", server_name);
        freeaddrinfo(servinfo);
        return -1;
    } else
        log_info(logger, "Cliente conectado en %s:%s (a %s)\n", ip, puerto, server_name);

    // Error handshake
    if(handshake(socket_cliente) == -1){
        log_error(logger, "Error en el handshake con %s", server_name);
        return -1;
    } 

    freeaddrinfo(servinfo);

    return socket_cliente;
}

    
void escucha_peticiones_kernel() {
    
}

int handshake(int conexion)
{
    // size_t bytes;
	int32_t handshake = 1;
	int32_t result;

	send(conexion, &handshake, sizeof(int32_t), 0);
	recv(conexion, &result, sizeof(int32_t), MSG_WAITALL);

	if (result != 0) {
        return -1;
    } else 
        return 0;
}