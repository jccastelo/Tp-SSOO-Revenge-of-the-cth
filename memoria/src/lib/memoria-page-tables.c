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
        }
        return;
    }
    
    // Calculamos el índice del frame en el nivel actual, y obtenemos o creamos el hijo correspondiente al índice calculado:
    int index = calculate_index(frame_id, level, entries_per_level, divisors);
    t_list *child = get_or_create_child(current_node, index);

    // Recurre al siguiente nivel para insertar el frame:
    insert_frame_in_table(child, level + 1, total_levels, entries_per_level, divisors, frame, frame_id);
}

int calculate_index(int frame_id, int level, int entries_per_level, int *divisors) {
    return (frame_id / divisors[level - 1]) % entries_per_level;
}

t_list* get_or_create_child(t_list *parent, int index) {
    t_list *child = (index < list_size(parent)) ? list_get(parent, index) : NULL;

    if (!child) {
        child = list_create();
        list_add(parent, child);
    } 

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
    int total_levels = config_memoria->CANTIDAD_NIVELES;
    int retardo_memoria = config_memoria->RETARDO_MEMORIA;
    t_list *current_table = get_root_table(id_process);

    for (int current_level = 1; current_level <= total_levels; current_level++) {
        if (list_size(entries_per_level) == 0) {
            // No hay entradas, no se puede seguir
            return -1;
        }

        int entry_index = *(int *)list_get(entries_per_level, current_level - 1);
        void *entry = list_get(current_table, entry_index);

        usleep(retardo_memoria * 1000);

        if (current_level == total_levels) 
            frame = (int)(intptr_t)entry;
        else 
            current_table = (t_list *)entry;
    }

    return frame;
}

t_list *get_frames_from_entries(int id_process) {
    // Inicializamos variables para el seguimiento de niveles de la tabla de páginas
    int current_level = 1;
    int total_levels = config_memoria->CANTIDAD_NIVELES;
    
    // Inicializamos estructuras administrativas:
    t_list *current_table = get_root_table(id_process);
    t_list *frame_as_busy = list_create();

    // Iteramos sobre los niveles de la tabla de páginas:
    get_occupied_frames_from_page_table(current_level, total_levels, current_table, frame_as_busy);

    return frame_as_busy;
}

void get_occupied_frames_from_page_table(int current_level, int total_levels, t_list *current_table, t_list *frame_as_busy) {
    if(current_level == total_levels) {
        // Si estamos en el último nivel, agregamos los frames ocupados a la lista
        for (int i = 0; i < list_size(current_table); i++) {
            int frame = (int)(intptr_t)list_get(current_table, i);
            list_add(frame_as_busy, (void *)(intptr_t)frame);
        }
    } else {
        // Si no estamos en el último nivel, iteramos sobre las entradas de la tabla actual
        for (int i = 0; i < list_size(current_table); i++) {
            t_list *child_table = (t_list *)list_get(current_table, i);
            get_occupied_frames_from_page_table(current_level + 1, total_levels, child_table, frame_as_busy);
        }
    }
}

t_list *get_root_table(int id_process) {
    // Convertimos el ID del proceso a cadena para usar como clave en el diccionario
    char *key_id_process = string_itoa(id_process);

    // Obtenemos la tabla de páginas raíz asociada al proceso desde el diccionario global
    return dictionary_get(all_process_page_tables, key_id_process);
}
