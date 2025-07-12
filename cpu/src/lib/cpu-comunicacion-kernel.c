#include "../include/cpu-comunicacion-kernel.h"

void recibir_contexto_de_kernel() {
    int cod_op;
    //int buffer_size;
    //void* buffer;

    t_buffer* new_buffer = malloc(sizeof(t_buffer));
    new_buffer->size = 0;
    new_buffer->stream = NULL;

    log_info(logger, "Recibiendo PID y PC desde Kernel por el puesto DISPATCH");

    cod_op = recibir_operacion(socket_dispatch);
    //buffer = recibir_buffer(&buffer_size, socket_dispatch);

    log_info(logger, "recibi el op_cod %d", cod_op);

    new_buffer->stream = recibir_buffer(&new_buffer->size, socket_dispatch);

    // log_info(logger, "recibi el buffer");

    if (cod_op == CONTEXT_PROCESS) {
        // log_info(logger, "DESERIALIZANDO BUFFER");
        deserializar_contexto(new_buffer);
        // log_info(logger, "PID: %d - Programa a ejecutar - Program Counter: %d", contexto->pid, contexto->pc);
    }
    else
        log_info(logger,"ERROR RECIBIENDO PROCESO DE KERNEL");
        
    free(new_buffer->stream);
    free(new_buffer);
}

void deserializar_contexto(t_buffer *buffer) {
    contexto = malloc(sizeof(t_contexto));
    int desplazamiento = 0;

    memcpy(&(contexto->pid), buffer->stream + desplazamiento, sizeof(int));
    desplazamiento += sizeof(int);

    memcpy(&(contexto->pc), buffer->stream + desplazamiento, sizeof(int));
}

bool recibir_interrupciones() {
    op_code cod_op;
    int recibido = recv(socket_interrupt, &cod_op, sizeof(cod_op), MSG_DONTWAIT);
    return (recibido > 0 && cod_op == INTERRUPT);
        
}

void enviar_contexto_desalojo() {
    t_paquete* paquete = crear_paquete(CONTEXTO_DESALOJO);
    agregar_contexto_al_paquete(paquete);
    enviar_paquete(paquete, socket_dispatch);
    eliminar_paquete(paquete);
}


//SYSCALLS

void syscall_io(char* dispositivo, int tiempo) {
    t_paquete* paquete = crear_paquete(IO);

    int longitudNombre = strlen(dispositivo) +1;
    agregar_a_paquete(paquete, &longitudNombre, sizeof(int));
    agregar_a_paquete(paquete, dispositivo, longitudNombre);
    agregar_a_paquete(paquete, &tiempo, sizeof(int));
    agregar_contexto_al_paquete(paquete);
    enviar_paquete(paquete, socket_dispatch);
    eliminar_paquete(paquete);
}

void syscall_init_proc(char* archivo, int tamanio) {
    t_paquete* paquete = crear_paquete(INIT_PROC);

    int longitudNombre = strlen(archivo) +1;
    agregar_a_paquete(paquete, &longitudNombre, sizeof(int));
    agregar_a_paquete(paquete, archivo, longitudNombre);
    agregar_a_paquete(paquete, &tamanio, sizeof(int));
    // agregar_contexto_al_paquete(paquete);
    enviar_paquete(paquete, socket_dispatch);
    eliminar_paquete(paquete);
}

void syscall_dump_memory() {
    t_paquete* paquete = crear_paquete(DUMP_MEMORY);
    agregar_contexto_al_paquete(paquete);
    enviar_paquete(paquete, socket_dispatch);
    eliminar_paquete(paquete);
}

void syscall_exit() {
    t_paquete* paquete = crear_paquete(EXIT_SYS);
    agregar_contexto_al_paquete(paquete);
    enviar_paquete(paquete, socket_dispatch);
    eliminar_paquete(paquete);
}

void agregar_contexto_al_paquete(t_paquete* paquete) {
    agregar_a_paquete(paquete, &contexto->pid, sizeof(int));
    agregar_a_paquete(paquete, &contexto->pc, sizeof(int));
}
