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

    t_paquete *paquete = crear_paquete(IDENTIFICAR_IO);

    int tamanio = string_length(nombre);
    agregar_a_paquete(paquete, &tamanio, sizeof(int)); 

    agregar_a_paquete(paquete,nombre,tamanio);

    enviar_paquete(paquete, socket_kernel); 
    
    eliminar_paquete(paquete);

    log_info(logger,"Paquete de identificacion de IO enviado a Kernel");
}