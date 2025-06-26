#include "../include/cpu-comunicacion-memoria.h"

void conseguir_siguiente_instruccion() {
    log_info(logger, "pidiendo la instruccion a memoria");
    t_paquete* paquete = crear_paquete(GET_INSTRUCTION);
    agregar_a_paquete(paquete, &contexto->pid, sizeof(int));
    agregar_a_paquete(paquete, &contexto->pc, sizeof(int));
    enviar_paquete(paquete, socket_memoria);
}


char* devolver_instruccion_a_ejecutar() {
    int cod_op;
    char* instruccion;
    int desplazamiento = 0;

    t_buffer* new_buffer = malloc(sizeof(t_buffer));
    new_buffer->size = 0;
    new_buffer->stream = NULL;

    cod_op = recibir_operacion(socket_memoria);
    
    if (cod_op == RETURN_INSTRUCCION)
    {
        new_buffer->stream = recibir_buffer(&new_buffer->size, socket_memoria);
        log_info(logger, "deserializando la instruccion..");
        parsear_string(new_buffer->stream, &desplazamiento , &instruccion);
        log_info(logger, "Instruccion: %s", instruccion);
    }
    else
        log_error(logger, "error deserializando la instruccion que viene de memoria.");

    free(new_buffer);
    return instruccion;
}

int pedir_marco_a_memoria(t_traduccion *traduccion) {
    t_paquete* paquete = crear_paquete(GET_FRAME);
    agregar_a_paquete(paquete, &contexto->pid, sizeof(int));
    agregar_a_paquete(paquete, traduccion->entradas, sizeof(int) * CANTIDAD_NIVELES);
    enviar_paquete(paquete, socket_memoria);

    int cod_op;
    int frame;
    int desplazamiento = 0;

    t_buffer* new_buffer = malloc(sizeof(t_buffer));
    new_buffer->size = 0;
    new_buffer->stream = NULL;

    cod_op = recibir_operacion(socket_memoria);
    
    if (cod_op == RETURN_FRAME)
    {
        new_buffer->stream = recibir_buffer(&new_buffer->size, socket_memoria);
        memcpy(&(frame), new_buffer->stream + desplazamiento, sizeof(int));
    }
    else
        log_error(logger, "error deserializando el frame proveniente de memoria.");

    free(new_buffer);
    return frame;
}

char* conseguir_contenido_frame(int frame) {
    t_paquete* paquete = crear_paquete(GET_CONTENT);
    agregar_a_paquete(paquete, &frame, sizeof(int));
    enviar_paquete(paquete, socket_memoria);

    t_buffer* new_buffer = malloc(sizeof(t_buffer));
    new_buffer->size = 0;
    new_buffer->stream = NULL;

    int cod_op = recibir_operacion(socket_memoria);
    if (cod_op != RETURN_CONTENT) {
        log_error(logger, "Error al recibir el contenido del frame desde Memoria.");
        return NULL;
    }

    char* contenido = malloc(TAM_PAGINA);

    new_buffer->stream = recibir_buffer(&new_buffer->size, socket_memoria);
    memcpy(contenido, new_buffer->stream, TAM_PAGINA);

    free(new_buffer);
    return contenido;
}

void escribir_pagina_en_memoria(int frame, char* contenido) {
    t_paquete* paquete = crear_paquete(WRITE_MEM);
    agregar_a_paquete(paquete, &frame, sizeof(int));
    agregar_a_paquete(paquete, contenido, TAM_PAGINA);
    enviar_paquete(paquete, socket_memoria);
}
