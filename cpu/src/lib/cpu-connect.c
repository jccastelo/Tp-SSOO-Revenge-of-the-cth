#include "../include/cpu-connect.h"

void cpu_connect() {
    // Inicializamos la variable global de servidores:
    cpu_servers = malloc(sizeof(t_cpu_servers));

    // Incializamos variables necesarias para la conexión:
    char *puerto_dispatch = string_itoa(config_cpu->PUERTO_KERNEL_DISPATCH);
    char *puerto_interrupt = string_itoa(config_cpu->PUERTO_KERNEL_INTERRUPT);
    char *puerto_memoria = string_itoa(config_cpu->PUERTO_MEMORIA);

    // Establecemos la conexión con el kernel dispatch y el kernel interrupt:
    setup_connection_with_server("Memoria", config_cpu->IP_MEMORIA, puerto_memoria, set_socket_memoria);
    setup_connection_with_server("Kernel DISPATCH", config_cpu->IP_KERNEL, puerto_dispatch, set_socket_kernel_dispatch);
    setup_connection_with_server("Kernel INTERRUPT", config_cpu->IP_KERNEL, puerto_interrupt, set_socket_kernel_interrupt);
}

void set_socket_kernel_dispatch(int socket) {
    // Guardamos el socket del kernel dispatch en la variable global:
    cpu_servers->socket_dispatch = socket;

    // Enviamos el handshake al kernel:
    generar_handshake(cpu_servers->socket_dispatch, "Kernel DISPATCH");
}

void set_socket_kernel_interrupt(int socket) {
    // Guardamos el socket del kernel interrupt en la variable global:
    cpu_servers->socket_interrupt = socket;

    // Enviamos el handshake al kernel interrupt:
    generar_handshake(cpu_servers->socket_interrupt, "Kernel INTERRUPT");
}

void set_socket_memoria(int socket) {
    // Guardamos el socket de memoria en la variable global:ñ
    cpu_servers->socket_memoria = socket;

    // Enviamos el handshake a memoria:
    generar_handshake(cpu_servers->socket_memoria, "Memoria");
}
