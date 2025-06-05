#include "../include/cpu-comunicacion-kernel.h"

void recibir_contexto_de_kernel() {
    op_code cod_op;
    int buffer_size;
    void* buffer;

    cod_op = recibir_operacion(socket_dispatch);
    buffer = recibir_buffer(&buffer_size, socket_dispatch);

    if (cod_op == CONTEXT_PROCESS) {
        log_info(logger, "PID: %d - Programa a ejecutar - Program Counter: %d", contexto->pid, contexto->pc);
        deserializar_contexto(buffer);
    }

    else
        //TODO ERROR

    free(buffer);
}

void deserializar_contexto(void* buffer) {
    contexto = malloc(sizeof(t_contexto));
    int desplazamiento = 0;

    memcpy(&(contexto->pid), buffer + desplazamiento, sizeof(int));
    desplazamiento += sizeof(int);

    memcpy(&(contexto->pc), buffer + desplazamiento, sizeof(int));
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
}


//SYSCALLS

void syscall_io(char* dispositivo, int tiempo) {
    t_paquete* paquete = crear_paquete(IO);
    agregar_a_paquete(paquete, dispositivo, strlen(dispositivo) + 1);
    agregar_a_paquete(paquete, &tiempo, sizeof(int));
    agregar_contexto_al_paquete(paquete);
    enviar_paquete(paquete, socket_dispatch);
}

void syscall_init_proc(char* archivo, int tamanio) {
    t_paquete* paquete = crear_paquete(INIT_PROC);
    agregar_a_paquete(paquete, archivo, strlen(archivo) + 1);
    agregar_a_paquete(paquete, &tamanio, sizeof(int));
    agregar_contexto_al_paquete(paquete);
    enviar_paquete(paquete, socket_dispatch);
}

void syscall_dump_memory() {
    t_paquete* paquete = crear_paquete(DUMP_MEMORY);
    agregar_contexto_al_paquete(paquete);
    enviar_paquete(paquete, socket_dispatch);
}

void syscall_exit() {
    t_paquete* paquete = crear_paquete(EXIT_SYS);
    agregar_contexto_al_paquete(paquete);
    enviar_paquete(paquete, socket_dispatch);
}

void agregar_contexto_al_paquete(t_paquete* paquete) {
    agregar_a_paquete(paquete, &contexto->pid, sizeof(int));
    agregar_a_paquete(paquete, &contexto->pc, sizeof(int));
}
