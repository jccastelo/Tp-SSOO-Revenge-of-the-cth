#include "../include/cpu-connect.h"

void cpu_connect() {
    // Incializamos variables necesarias para la conexión:
    char *puerto_dispatch = string_itoa(config_cpu->PUERTO_KERNEL_DISPATCH);
    char *puerto_interrupt= string_itoa(config_cpu->PUERTO_KERNEL_INTERRUPT);

    // Establecemos la conexión con el kernel dispatch y el kernel interrupt:
    setup_connection_with_server("KERNEL DISPATCH", config_cpu->IP_KERNEL, puerto_dispatch, set_socket_kernel_dispatch);
    setup_connection_with_server("KERNEL INTERRUPT", config_cpu->IP_KERNEL, puerto_interrupt, set_socket_kernel_interrupt);
}

void set_socket_kernel_dispatch(int socket) {
    // Guardamos el socket del kernel dispatch en la variable global:
    socket_dispatch = socket;

    // Enviamos el handshake al kernel:
    generar_handshake(socket_dispatch, "KERNEL DISPATCH");
}

void set_socket_kernel_interrupt(int socket) {
    // Guardamos el socket del kernel interrupt en la variable global:
    socket_interrupt = socket;

    // Enviamos el handshake al kernel interrupt:
    generar_handshake(socket_interrupt, "KERNEL INTERRUPT");
}
