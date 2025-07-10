#include "../include/cpu-comunicacion-memoria.h"

void conseguir_siguiente_instruccion() {
    // log_info(logger, "pidiendo la instruccion a memoria");
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
        // log_info(logger, "deserializando la instruccion..");
        parsear_string(new_buffer->stream, &desplazamiento , &instruccion);
        // log_info(logger, "Instruccion: %s", instruccion);
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

    // int cod_op;
    int frame;
    // int desplazamiento = 0;


    // t_buffer* new_buffer = malloc(sizeof(t_buffer));
    // new_buffer->size = 0;
    // new_buffer->stream = NULL;

    recv(socket_memoria, &frame, sizeof(int), 0);
    // new_buffer->stream = recibir_buffer(&new_buffer->size, socket_memoria);
    // memcpy(&(frame), new_buffer->stream + desplazamiento, sizeof(int));

    // else
    //     log_error(logger, "error deserializando el frame proveniente de memoria.");

    // free(new_buffer);
    log_info(logger, "PID: %d - OBTENER MARCO - Página: %d - Marco: %d", contexto->pid, traduccion->nro_pagina, frame);
    return frame;
}

char* leer_en_memoria(int dir_fisica, int tamanio) {
    t_paquete* paquete = crear_paquete(READ_MEM);
    agregar_a_paquete(paquete, &contexto->pid, sizeof(int));
    agregar_a_paquete(paquete, &tamanio, sizeof(int));
    agregar_a_paquete(paquete, &dir_fisica, sizeof(int));
    enviar_paquete(paquete, socket_memoria);

    int cod_op = recibir_operacion(socket_memoria);
    if (cod_op != OK) {
        log_error(logger, "Error al recibir contenido parcial de memoria.");
    }

    int size = 0;
    int desplazamiento = 0;
    void *buffer = recibir_buffer(&size, socket_memoria);

    int size_string;
    char* resultado = malloc(tamanio + 1);

    memcpy(&size_string, buffer + desplazamiento, sizeof(int));
    desplazamiento += sizeof(int);

    memcpy(resultado, buffer + desplazamiento, size_string);
    resultado[size_string] = '\0'; // Aseguramos que el string esté terminado

    // parsear_string(buffer->stream, &tamanio, &resultado);

    log_info(logger, "PID: %d - Acción: LEER - Dirección Física: %d - Valor: %s", contexto->pid, dir_fisica, resultado);
    return resultado;
}

void escribir_en_memoria(int dir_fisica, char* contenido) {
    log_info(logger, "PID: %d - Acción: ESCRIBIR - Dirección Física: %d - Valor: %s", contexto->pid, dir_fisica, contenido);
    t_paquete* paquete = crear_paquete(WRITE_MEM);
    agregar_a_paquete(paquete, &contexto->pid, sizeof(int));
    agregar_a_paquete_string(paquete, contenido, string_length(contenido));
    agregar_a_paquete(paquete, &dir_fisica, sizeof(int));
    enviar_paquete(paquete, socket_memoria);

    int respuesta;
    recv(socket_memoria, &respuesta, sizeof(int), 0);
    
    if (respuesta != OK) {
        log_error(logger, "Error al escribir en memoria.");
    }
}