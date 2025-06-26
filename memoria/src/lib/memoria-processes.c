#include "../include/memoria-processes.h"

t_process_in_memory *initialization_process() {
    // Solicitamos memoria dinámica para un nuevo proceso en memoria:
    t_process_in_memory *process = malloc(sizeof(t_process_in_memory));

    // Inicializamos los miembros de la estructura del proceso:
    process->tablas_requests = 0;        // Cantidad de accesos a tablas de páginas
    process->instrs_requests = 0;        // Cantidad de instrucciones solicitadas
    process->swap_out_requests = 0;      // Cantidad de bajadas a SWAP
    process->swap_in_requests = 0;       // Cantidad de subidas a memoria principal
    process->mem_read_requests = 0;      // Cantidad de lecturas de memoria
    process->mem_write_requests = 0;     // Cantidad de escrituras de memoria

    return process;
}

void add_process_to_memory(int id_process) {
    // Se convierte el ID del proceso a string para usarlo como clave, y se inicializa su estructura en memoria
    char* key_id_process = string_itoa(id_process);
    t_process_in_memory *new_process = initialization_process();

    // Se guarda en el diccionario global de procesos activos:
    dictionary_put(metricas_por_procesos, key_id_process, new_process);
}

int required_frames_for_process(int size_process) {
    int tam_pagina = config_memoria->TAM_PAGINA;
    return (size_process + tam_pagina - 1) / tam_pagina;
}