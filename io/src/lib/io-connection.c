#include "include/io-connection.h"

void io_connect(char* nombre) {
    // Incializamos variables necesarias para la conexión:
    char *puerto = string_itoa(config_io->PUERTO_KERNEL);

    setup_connection_with_server("KERNEL", config_io->IP_KERNEL, puerto, set_socket_kernel);

    enviar_nombre_kernel(nombre);
}

void set_socket_kernel(int socket) {
    // Guardamos el socket del kernel en la variable global:
    socket_kernel = socket;

    // Enviamos el handshake al kernel:
    generar_handshake(socket_kernel, "KERNEL");
}

void enviar_nombre_kernel(char* nombre){

    int tamanio = string_length(nombre);
    send(socket_kernel, &tamanio, sizeof(int), 0);
    send(socket_kernel, nombre, tamanio + 1, 0);
}