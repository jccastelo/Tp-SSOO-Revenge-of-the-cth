#include "../include/conexion_kernel.h"

void crear_conexion_con_kernel(char *ip_kernel, char *puerto_kernel) {
    int socket_a_servidor = crear_conexion("KERNEL", ip_kernel, puerto_kernel);

    if (socket_a_servidor == -1) {
        log_error(logger, "Error al conectar con el kernel");
        exit(EXIT_FAILURE);
    }

    // Realizamos el handshake:
    generar_handshake(socket_a_servidor, "KERNEL", ip_kernel, puerto_kernel);
    log_info(logger, "Conexión establecida con el kernel en %s:%s", ip_kernel, puerto_kernel);

    // Guardamos el socket del kernel:
    socket_a_kernel = socket_a_servidor;
}