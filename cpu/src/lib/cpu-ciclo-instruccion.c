#include "../include/cpu-ciclo-instruccion.h"


void realizar_ciclo_de_instruccion() {
    bool finaliza = false;
    while(!finaliza)
    { 
        // CONSIGO LA INSTRUCCION A EJECUTAR (formato string)
        char* instruccion_a_ejecutar = fetch_instruction();

        // DECODIFICO LA INSTRUCCION
        t_instruccion* instruccion = decode(instruccion_a_ejecutar);

        // EJECUTO LA INSTRUCCION
        excecute(instruccion);

        // REVISO SI FINALIZÓ POR SYSCALLS o POR INTERRUPCION
        finaliza = es_syscall_que_frena(instruccion->tipo);
        if (!finaliza)
            finaliza = check_interrupt();

        // LIBERO LA MEMORIA
        free(instruccion_a_ejecutar);
        free(instruccion);
    }
}

char* fetch_instruction() {
    conseguir_siguiente_instruccion();
    char* instruccion = devolver_instruccion_a_ejecutar();
    log_info(logger, "## PID: %d - FETCH - Program Counter: %d", contexto->pid, contexto->pc);
    return instruccion;
}

t_instruccion* decode(char* instruccion) {
    t_instruccion* instr = malloc(sizeof(t_instruccion));
    char** palabras_instr = string_split(instruccion, " ");
    instr->argv = palabras_instr;
    instr->argc = string_array_size(palabras_instr);
    instr->tipo = mapeo_string_tipo(palabras_instr[0]);
    string_array_destroy(palabras_instr);
    return instr;
}

void excecute(t_instruccion* instruccion) {
    char* parametros = concatenar_parametros(instruccion->argv, instruccion->argc);
    log_info(logger, "## PID: %d - Ejecutando: %s - %s", contexto->pid, instruccion->argv[0], parametros);
    free(parametros);

    if (instruccion->tipo != INSTR_GOTO)
        contexto->pc++;

    switch (instruccion->tipo)
    {
    case INSTR_NOOP:
        usleep(500000);
        break;
    case INSTR_WRITE: 
        int direccion_logica_wr = atoi(instruccion->argv[1]);
        char* mensaje = instruccion->argv[2];
        escribir(direccion_logica_wr, mensaje);
        break;
    case INSTR_READ:
        int direccion_logica_rd = atoi(instruccion->argv[1]);
        int tamanio_rd = atoi(instruccion->argv[2]);
        leer(direccion_logica_rd, tamanio_rd);
        break;
    case INSTR_GOTO:
        contexto->pc = atoi(instruccion->argv[1]);
        break;
    case INSTR_IO:
        char* dispositivo = instruccion->argv[1];
        int tiempo = atoi(instruccion->argv[2]);
        guardado_cache_por_desalojo();
        syscall_io(dispositivo, tiempo);
        break;
    case INSTR_INIT_PROC:
        char* archivo_instrucciones = instruccion->argv[1];
        int tamanio_proc = atoi(instruccion->argv[2]);
        syscall_init_proc(archivo_instrucciones, tamanio_proc);
        break;
    case INSTR_DUMP_MEMORY:
        guardado_cache_por_desalojo();
        syscall_dump_memory();
        break;
    case INSTR_EXIT:
        guardado_cache_por_desalojo();
        syscall_exit();
        break;
    default:
        break;
    }
}

bool es_syscall_que_frena(t_tipo_instruccion tipo) {
    return tipo == INSTR_IO || tipo == INSTR_DUMP_MEMORY || tipo == INSTR_EXIT;
}


bool check_interrupt() {
    bool finaliza = false;
    if (recibir_interrupciones()) {
        log_warning(logger, "## Llega interrupción al puerto Interrupt");
        guardado_cache_por_desalojo();
        enviar_contexto_desalojo();
        finaliza = true;
    }
    return finaliza;
}

t_tipo_instruccion mapeo_string_tipo(char* tipo_instruccion) {
    if (string_equals_ignore_case(tipo_instruccion, "NOOP")) return INSTR_NOOP;
    else if (string_equals_ignore_case(tipo_instruccion, "WRITE")) return INSTR_WRITE;
    else if (string_equals_ignore_case(tipo_instruccion, "READ")) return INSTR_READ;
    else if (string_equals_ignore_case(tipo_instruccion, "GOTO")) return INSTR_GOTO;
    else if (string_equals_ignore_case(tipo_instruccion, "IO")) return INSTR_IO;
    else if (string_equals_ignore_case(tipo_instruccion, "INIT_PROC")) return INSTR_INIT_PROC;
    else if (string_equals_ignore_case(tipo_instruccion, "DUMP_MEMORY")) return INSTR_DUMP_MEMORY;
    else if (string_equals_ignore_case(tipo_instruccion, "EXIT")) return INSTR_EXIT;
    else return -1;
}