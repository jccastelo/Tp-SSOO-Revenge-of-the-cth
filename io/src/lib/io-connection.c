#include "include/io-connection.h"

void io_connect() {
    // Incializamos variables necesarias para la conexión:
    char *puerto = string_itoa(config_io->PUERTO_KERNEL);

    setup_connection_with_server("KERNEL", config_io->IP_KERNEL, puerto, set_socket_kernel);
}

void set_socket_kernel(int socket) {
    // Guardamos el socket del kernel en la variable global:
    socket_kernel = socket;

    // Enviamos el handshake al kernel:
    generar_handshake(socket_kernel, "KERNEL");
}