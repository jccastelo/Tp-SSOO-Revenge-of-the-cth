#include "../include/cpu-comunicacion-memoria.h"

void conseguir_siguiente_instruccion() {
    log_info(logger, "pidiendo la instruccion a memoria");
    t_paquete* paquete = crear_paquete(GET_INSTRUCTION);
    agregar_a_paquete(paquete, &contexto->pc, sizeof(int));
    agregar_a_paquete(paquete, &contexto->pid, sizeof(int));
    enviar_paquete(paquete, socket_memoria);
}


char* devolver_instruccion_a_ejecutar() {
    int cod_op;
    char* instruccion;

    t_buffer* new_buffer = malloc(sizeof(t_buffer));
    new_buffer->size = 0;
    new_buffer->stream = NULL;

    cod_op = recibir_operacion(socket_memoria);
    
    if (cod_op == RETURN_INSTRUCCION)
    {
        new_buffer->stream = recibir_buffer(&new_buffer->size, socket_memoria);
        log_info(logger, "deserializando la instruccion..");
        instruccion = deserializar_instruccion(new_buffer);
    }
    else
        log_error(logger, "error deserializando la instruccion que viene de memoria.");

    free(new_buffer);
    return instruccion;
}

char* deserializar_instruccion(t_buffer *buffer) {
    char* buffer_char = (char*) buffer->stream;
    char* instr = strdup(buffer_char);
    return instr;
}