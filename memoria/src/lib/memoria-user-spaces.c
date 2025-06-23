#include "../include/memoria-user-spaces.h"

void init_free_frames_bitmap() {
    // Inicializamos el tamaño del bitarray:
    int size_bitarray = (quantity_frames + 7) / 8;

    // Creamos las variables correspondientes:
    frames_memory = malloc(size_bitarray);
    frames_bitmap = bitarray_create_with_mode(frames_memory, size_bitarray, LSB_FIRST);
}

t_list *is_memory_sufficient(int size_process) {
    // Lista para almacenar índices de frames libres
    t_list *free_frames = list_create();
    
    int quantity_frames_process = required_frames_for_process(size_process);
    int index_frames = 0;
    int added_frames = 0;

    // Recorremos todos los frames hasta encontrar los suficientes para el proceso
    while (index_frames < quantity_frames && added_frames < quantity_frames_process) {
        if (!bitarray_test_bit(frames_bitmap, index_frames)) {
            list_add(free_frames, (void*)(intptr_t)index_frames);
            added_frames++;
        }
        
        index_frames++;
    }

    // Validamos que haya suficientes frames libres
    if (added_frames < quantity_frames_process) {
        list_destroy(free_frames);  // Liberar lista parcial
        return NULL;
    }

    return free_frames;
}
