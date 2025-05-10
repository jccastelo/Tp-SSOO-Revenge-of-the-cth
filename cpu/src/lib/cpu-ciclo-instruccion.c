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
    return instr;
}

void excecute(t_instruccion* instruccion) { //TODO
    switch (instruccion->tipo)
    {
    case NOOP:
        usleep(500000);
        break;
    case WRITE: 
        int direccion_wr = atoi(instruccion->argv[1]);
        char* mensaje = instruccion->argv[2];
        //TODO con traduccion lógica a física
        break;
    case READ:
        int direccion_rd = atoi(instruccion->argv[1]);
        int tamanio_rd = atoi(instruccion->argv[2]);
        //TODO con traduccion lógica a física
        break;
    case GOTO:
        contexto->pc = atoi(instruccion->argv[1]);
        break;
    case IO:
        char* dispositivo = instruccion->argv[1];
        int tiempo = atoi(instruccion->argv[2]);
        syscall_io(dispositivo, tiempo);
        break;
    case INIT_PROC:
        char* archivo_instrucciones = instruccion->argv[1];
        int tamanio_proc = atoi(instruccion->argv[2]);
        syscall_init_proc(archivo_instrucciones, tamanio_proc);
        break;
    case DUMP_MEMORY:
        syscall_dump_memory();
        break;
    case EXIT:
        syscall_exit();
        break;
    default:
        break;
    }
    if (instruccion->tipo != GOTO)
        contexto->pc++;
}

bool es_syscall_que_frena(t_tipo_instruccion tipo) {
    return tipo == IO || tipo == DUMP_MEMORY || tipo == EXIT;
}


bool check_interrupt() {
    bool finaliza = false;
    if (recibir_interrupciones()) {
        enviar_contexto_desalojo();
        finaliza = true;
    }
    return finaliza;
}

t_tipo_instruccion mapeo_string_tipo(char* tipo_instruccion) {
    if (string_equals_ignore_case(tipo_instruccion, "NOOP")) return NOOP;
    else if (string_equals_ignore_case(tipo_instruccion, "WRITE")) return WRITE;
    else if (string_equals_ignore_case(tipo_instruccion, "READ")) return READ;
    else if (string_equals_ignore_case(tipo_instruccion, "GOTO")) return GOTO;
    else if (string_equals_ignore_case(tipo_instruccion, "IO")) return IO;
    else if (string_equals_ignore_case(tipo_instruccion, "INIT_PROC")) return INIT_PROC;
    else if (string_equals_ignore_case(tipo_instruccion, "DUMP_MEMORY")) return DUMP_MEMORY;
    else if (string_equals_ignore_case(tipo_instruccion, "EXIT")) return EXIT;
    else return -1;
}