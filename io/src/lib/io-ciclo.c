#include "../include/io-ciclo.h"

void ciclo_de_io(){

    t_proceso* proceso = recibir_proceso();

    pid_proceso_Actual = proceso->pid;

    usleep(proceso->milisegundos * 1000);
    //sleep(600);
    notificar_liberacion(proceso);

    free(proceso);
}

t_proceso* recibir_proceso(){

    t_buffer* buffer = malloc(sizeof(t_buffer));
    buffer->size = 0;
    buffer->stream = NULL;

    // Leer el tamaño del buffer
    recv(socket_kernel, &buffer->size, sizeof(int), MSG_WAITALL);

    // Reservar memoria para el stream y recibirlo
    buffer->stream = malloc(buffer->size);
    recv(socket_kernel, buffer->stream, buffer->size, 0);

    t_proceso* proceso = malloc(sizeof(t_proceso));

    memcpy(&(proceso->pid), buffer->stream, sizeof(int));
    memcpy(&(proceso->milisegundos), buffer->stream + sizeof(int), sizeof(int));

    free(buffer->stream);
    free(buffer);

    log_info(logger, "Llego PID %d con MILISEGUNDOS %d ",proceso->pid,proceso->milisegundos);

    return proceso;
}

void notificar_liberacion(t_proceso* proceso){

    t_paquete* paquete = crear_paquete(DESBLOQUEO_IO);
    agregar_a_paquete(paquete, &proceso->pid, sizeof(int));
    enviar_paquete(paquete, socket_kernel);
    eliminar_paquete(paquete);
    pid_proceso_Actual = -1;
}