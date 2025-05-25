#include "../include/cpu-comunicacion-memoria.h"

void conseguir_siguiente_instruccion() {
    t_paquete* paquete = crear_paquete(GET_INSTRUCTION);
    agregar_a_paquete(paquete, &contexto->pc, sizeof(int));
    agregar_a_paquete(paquete, &contexto->pid, sizeof(int));
    enviar_paquete(paquete, socket_memoria);
}


char* devolver_instruccion_a_ejecutar() {
    int cod_op;
    int buffer_size;
    void* buffer;
    char* instruccion;

    cod_op = recibir_operacion(socket_memoria);
    buffer = recibir_buffer(&buffer_size, socket_memoria);

    if (cod_op == RETURN_INSTRUCCION)
        instruccion = deserializar_instruccion(buffer);
    else
        //TODO: ERROR

    free(buffer);
    return instruccion;
}

char* deserializar_instruccion(void* buffer) {
    char* buffer_char = (char*) buffer;
    char* instr = strdup(buffer_char);
    return instr;
}