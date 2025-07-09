#include "../include/cpu-connect.h"

void cpu_connect() {
    // Incializamos variables necesarias para la conexión:
    char *puerto_dispatch = string_itoa(config_cpu->PUERTO_KERNEL_DISPATCH);
    char *puerto_interrupt = string_itoa(config_cpu->PUERTO_KERNEL_INTERRUPT);
    char *puerto_memoria = string_itoa(config_cpu->PUERTO_MEMORIA);

    // Establecemos la conexión con el kernel dispatch y el kernel interrupt:
    setup_connection_with_server("Kernel DISPATCH", config_cpu->IP_KERNEL, puerto_dispatch, set_socket_kernel_dispatch);
    setup_connection_with_server("Kernel INTERRUPT", config_cpu->IP_KERNEL, puerto_interrupt, set_socket_kernel_interrupt);
    setup_connection_with_server("Memoria", config_cpu->IP_MEMORIA, puerto_memoria, set_socket_memoria);
}

void set_socket_kernel_dispatch(int socket) {
    // Guardamos el socket del kernel dispatch en la variable global:
    socket_dispatch = socket;

    // Enviamos el handshake al kernel:
    generar_handshake(socket_dispatch, "Kernel DISPATCH");
    enviar_id_cpu(socket_dispatch); // Post handshake, envía a KERNEL su ID
}

void set_socket_kernel_interrupt(int socket) {
    // Guardamos el socket del kernel interrupt en la variable global:
    socket_interrupt = socket;
    
    // Enviamos el handshake al kernel interrupt:
    generar_handshake(socket_interrupt, "Kernel INTERRUPT");
    enviar_id_cpu(socket_interrupt);
}

void set_socket_memoria(int socket) {
    // Guardamos el socket de memoria en la variable global:ñ
    socket_memoria = socket;

    // Enviamos el handshake a memoria:
    generar_handshake(socket_memoria, "Memoria");
    init_estructura_memoria();
}

void enviar_id_cpu(int socket) {
    t_paquete* paquete = crear_paquete(CPU_ID);
    int cpu_id = atoi(id_cpu);
    agregar_a_paquete(paquete, &cpu_id, sizeof(int));
    enviar_paquete(paquete, socket);
}

void init_estructura_memoria() {

    int cod_op = MEMORY_CONFIG;
    log_info(logger,"ANTES DE SEND A MEMORIA INIT");
    send(socket_memoria, &cod_op, sizeof(int), 0);
    log_info(logger, "YA ENVIE EL MENSAJE");

    t_buffer* new_buffer = malloc(sizeof(t_buffer));
    new_buffer->size = 0;
    new_buffer->stream = NULL;

    cod_op = recibir_operacion(socket_memoria);
    new_buffer->stream = recibir_buffer(&new_buffer->size, socket_memoria);

    if (cod_op == MEMORY_CONFIG) {
        int desplazamiento = 0;
        memcpy(&(TAM_PAGINA), new_buffer->stream + desplazamiento, sizeof(int));
        desplazamiento += sizeof(int);
        memcpy(&(ENTRADAS_POR_TABLA), new_buffer->stream + desplazamiento, sizeof(int));
        desplazamiento += sizeof(int);
        memcpy(&(CANTIDAD_NIVELES), new_buffer->stream + desplazamiento, sizeof(int));

        log_info(logger, "YA DESERIALICE LO DE MEMORIA. TAM_PAG: %d, ENTRADAS_X_TABLA: %d, CANT_NIVELES: %d", TAM_PAGINA, ENTRADAS_POR_TABLA, CANTIDAD_NIVELES);
    }
    else
        log_info(logger,"ERROR INICIALIZANDO LOS DATOS DE MEMORIA");
        

    free(new_buffer);
}