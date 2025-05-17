#include "../include/cpu-connect.h"

void cpu_connect(int cod_mensaje_tipo) {
    // Incializamos variables necesarias para la conexión:
    char *puerto_dispatch = string_itoa(config_cpu->PUERTO_KERNEL_DISPATCH);
    char *puerto_interrupt = string_itoa(config_cpu->PUERTO_KERNEL_INTERRUPT);
    //char *puerto_memoria = string_itoa(config_cpu->PUERTO_MEMORIA); //test

    // Establecemos la conexión con el kernel dispatch y el kernel interrupt:
    if(cod_mensaje_tipo == 0)
    {setup_connection_with_server("Kernel DISPATCH", config_cpu->IP_KERNEL, puerto_dispatch, set_socket_kernel_dispatch);}
    else if(cod_mensaje_tipo == 1)
    {setup_connection_with_server("Kernel DISPATCH", config_cpu->IP_KERNEL, puerto_dispatch, identificar_cpu_A_kernel);}
    setup_connection_with_server("Kernel INTERRUPT", config_cpu->IP_KERNEL, puerto_interrupt, set_socket_kernel_interrupt);
    //setup_connection_with_server("Memoria", config_cpu->IP_MEMORIA, puerto_memoria, set_socket_memoria); //tedst
}

void set_socket_kernel_dispatch(int socket) {
    // Guardamos el socket del kernel dispatch en la variable global:
    int socket_dispatch = socket;

    // Enviamos el handshake al kernel:
    generar_handshake(socket_dispatch, "Kernel DISPATCH"); //test


}

void identificar_cpu_A_kernel(int socket)
{
    //IDENTIFICARSE
    int idCPU =88;
    t_paquete* paquete = crear_paquete(CPU_ID); // QUE VERGA LE PONGO

    crear_buffer(paquete);

    int tamanio_id = sizeof(int);  

    // Primero mandamos el tamaño del ID
    agregar_a_paquete(paquete, &tamanio_id, sizeof(int));

    agregar_a_paquete(paquete, &idCPU, sizeof(int));

    enviar_paquete(paquete, socket);

    eliminar_paquete(paquete);
}

void set_socket_kernel_interrupt(int socket) {
    // Guardamos el socket del kernel interrupt en la variable global:
    int socket_interrupt = socket;

    

    // Enviamos el handshake al kernel interrupt:
    generar_handshake(socket_interrupt, "Kernel INTERRUPT"); //test
}

void set_socket_memoria(int socket) {
    // Guardamos el socket de memoria en la variable global:
    int socket_memoria = socket;

    // Enviamos el handshake a memoria:
    //generar_handshake(socket_memoria, "Memoria"); //test
}
