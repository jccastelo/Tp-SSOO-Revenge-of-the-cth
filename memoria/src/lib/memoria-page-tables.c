#include "../include/memoria-pages-tables.h"

void setup_page_tables(int id_process, t_list *free_frames) {
    // Creamos el nodo raíz de la tabla de páginas del proceso
    t_list *page_table_root = list_create();

    // Registramos la tabla de páginas del proceso en la estructura global
    char *key_id_process = string_itoa(id_process);
    dictionary_put(all_process_page_tables, key_id_process, page_table_root);

    // Construimos la tabla de páginas asignando frames bajo demanda
    populate_page_table(free_frames, page_table_root);
}

void populate_page_table(t_list *free_frames, t_list *page_table_root) {
    // Obtenemos configuración de memoria
    int total_levels = config_memoria->CANTIDAD_NIVELES;
    int entries_per_level = config_memoria->ENTRADAS_POR_TABLA;

    // Creamos arreglo para divisores precomputados
    int divisors[total_levels];
    precompute_divisors(total_levels, entries_per_level, divisors);

    // Función para insertar cada frame:
    insert_each_frame(free_frames, page_table_root, divisors, total_levels, entries_per_level);
}

void insert_each_frame(t_list *free_frames, t_list *page_table_root, int *divisors, int total_levels, int entries_per_level) {
    int number_frames = list_size(free_frames);

    for (int i = 0; i < number_frames; i++) {
        int frame_id = (int)(intptr_t)list_get(free_frames, i);
        insert_frame_in_table(page_table_root, 1, total_levels, entries_per_level, divisors, frame_id, i);
    }
}

void insert_frame_in_table(t_list *current_node, int level, int total_levels, int entries_per_level, int *divisors, int frame, int frame_id) {
    // Si estamos en el último nivel y hay espacio, insertamos el frame directamente
    if (level == total_levels) {
        if (list_size(current_node) < entries_per_level) {
            list_add(current_node, (void *)(intptr_t)frame);
            log_info(logger, "Se agregó el frame %d en el nivel %d (nivel final)", frame, level);
        }
        return;
    }
    
    // Calculamos el índice del frame en el nivel actual, y obtenemos o creamos el hijo correspondiente al índice calculado:
    int index = calculate_index(frame_id, level, entries_per_level, divisors);
    t_list *child = get_or_create_child(current_node, index);
    log_info(logger, "Nivel %d | frame_id %d → índice calculado %d", level, frame_id, index);

    // Recurre al siguiente nivel para insertar el frame:
    insert_frame_in_table(child, level + 1, total_levels, entries_per_level, divisors, frame, frame_id);
}

int calculate_index(int frame_id, int level, int entries_per_level, int *divisors) {
    return (frame_id / divisors[level - 1]) % entries_per_level;
}

t_list* get_or_create_child(t_list *parent, int index) {
    t_list *child = (index < list_size(parent)) ? list_get(parent, index) : NULL;

    const char* mensaje = NULL;

    if (!child) {
        child = list_create();
        list_add(parent, child);
        mensaje = "creó";
    } else
        mensaje = "obtuvo";

    log_info(logger, "Se %s un hijo en el índice %d", mensaje, index);

    return child;
}

void precompute_divisors(int total_levels, int entries_per_level, int *divisors) {
    int prod = 1;
    for (int i = total_levels - 1; i >= 0; i--) {
        divisors[i] = prod;
        prod *= entries_per_level;
    }
}

int find_frame_from_entries(int id_process, t_list *entries_per_level) {
    int frame = -1;
    int current_level = 1;
    int total_levels = config_memoria->CANTIDAD_NIVELES;
    t_list *current_table = get_root_table(id_process);

    void closure(void *entry_index_ptr) {
        int entry_index = (int) entry_index_ptr;

        // Obtener la entrada de la tabla actual
        void *entry = list_get(current_table, entry_index);

        // Si estamos en el último nivel, asignamos el frame correspondiente, de lo contrario, continuemos recorriendo los niveles siguientes.
        if (current_level == total_levels) 
            frame = (int) entry;
        else 
            current_table = (t_list *) entry;

        current_level++;
    }

    list_iterate(entries_per_level, closure);

    return frame;
}

t_list *get_root_table(int id_process) {
    // Convertimos el ID del proceso a cadena para usar como clave en el diccionario
    char *key_id_process = string_itoa(id_process);

    // Obtenemos la tabla de páginas raíz asociada al proceso desde el diccionario global
    return dictionary_get(all_process_page_tables, key_id_process);
}
