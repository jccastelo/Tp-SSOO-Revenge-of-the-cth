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
    free(key_id_process);
}

int is_process_end(int id_process) {
    t_process_in_memory *process;
    t_list *frames_as_busy;
    t_list *instructions;
    t_list *page_tables;

    // Convertimos el id a string para buscar y remover el proceso del diccionario de métricas
    char *key_id_process = string_itoa(id_process);
    process = dictionary_remove(metricas_por_procesos, key_id_process);
    instructions = dictionary_remove(instrucciones_por_procesos, key_id_process); 

    // Obtenemos los frames ocupados por el proceso y los marcamos como libres
    if(!dictionary_remove(diccionario_swap_metadata, key_id_process)) {   
        frames_as_busy = get_frames_from_entries(id_process);
        mark_frames_as_free(frames_as_busy);
        page_tables = dictionary_remove(all_process_page_tables, key_id_process);
        delete_page_tables(1, config_memoria->CANTIDAD_NIVELES, page_tables);
        list_destroy_and_destroy_elements(frames_as_busy, free);
    } else
        log_debug(logger, "El proceso PID: %d fue suspendido y se procede a finalizar sin liberar ningún marco", id_process);

    // Mostramos los contadores del proceso:
    imprimir_contadores_del_proceso(id_process, process);
    list_destroy_and_destroy_elements(instructions, free);
    free(key_id_process);
    free(process);

    return OK;
}

int required_frames_for_process(int size_process) {
    int tam_pagina = config_memoria->TAM_PAGINA;
    return (size_process + tam_pagina - 1) / tam_pagina;
}

bool estaEn(t_dictionary* diccionario , char* pid_key){
    t_list* lista_de_marcos = get_marcos_list_of_proc(pid_key, diccionario);
    if(list_size(lista_de_marcos) > 0){
        list_destroy(lista_de_marcos);
        return true;
    }
    list_destroy(lista_de_marcos);
    return false;
}


void aumentar_contador(t_dictionary* dictionary, t_campo campo, char* pid_key) {
    t_process_in_memory* estructura = dictionary_get(dictionary, pid_key);
    if (!estructura) return;

    switch(campo) {
        case TABLAS_REQUESTS:
            estructura->tablas_requests++;
            break;
        case INSTRS_REQUESTS:
            estructura->instrs_requests++;
            break;
        case SWAP_OUT_REQUESTS:
            estructura->swap_out_requests++;
            break;
        case SWAP_IN_REQUESTS:
            estructura->swap_in_requests++;
            break;
        case MEM_READ_REQUESTS: 
            estructura->mem_read_requests++;
            break;
        case MEM_WRITE_REQUESTS:
            estructura->mem_write_requests++;
            break;
    }
}

void imprimir_contadores_del_proceso(int id_process, t_process_in_memory* estructura) {
    if (!estructura) return;

    log_info(logger, "## PID: %d - Proceso Destruido - Métricas - Acc.T.Pag: %d; Inst.Sol.: %d; SWAP: %d; Mem.Prin.: %d; Lec.Mem.: %d; Esc.Mem.: %d", 
        id_process, 
        estructura->tablas_requests, 
        estructura->instrs_requests, 
        estructura->swap_out_requests + estructura->swap_in_requests, 
        estructura->swap_in_requests, 
        estructura->mem_read_requests, 
        estructura->mem_write_requests
    );
}
