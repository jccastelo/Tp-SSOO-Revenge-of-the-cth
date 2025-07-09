#include "../include/memoria-user-spaces.h"

void init_free_frames_bitmap() {
    // Inicializamos el tamaño del bitarray:
    int size_bitarray = (quantity_frames + 7) / 8;
    log_info(logger, "Size bitarray: %d", size_bitarray);

    // Creamos las variables correspondientes:
    frames_memory = malloc(size_bitarray);
    frames_bitmap = bitarray_create_with_mode(frames_memory, size_bitarray, LSB_FIRST);
    inicializar_bitmap(frames_bitmap);
}

// Funcion para inicializar un bitmap:
void inicializar_bitmap(t_bitarray *frames_bitmap) {
    int size = bitarray_get_max_bit(frames_bitmap);

    for(int i = 0; i < size; i++)
        bitarray_clean_bit(frames_bitmap, i);
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
            log_info(logger, "Free Frame: %d", index_frames);
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

void mark_frames_as_busy(t_list *free_frames) {
    // Función auxiliar que marca un único marco como ocupado en el bitmap
    void set_frame_as_busy(void *frame) {
        int value_frame = (int)(intptr_t)frame;
        log_info(logger, "Frame marcado como ocupado: %d", value_frame);
        bitarray_set_bit(frames_bitmap, value_frame);
    }
    
    // Itera sobre todos los marcos de la lista y los marca como ocupados
    list_iterate(free_frames, set_frame_as_busy); 
}

void mark_frames_as_free(t_list *free_frames) {
    // Función auxiliar que marca un único marco como ocupado en el bitmap
    void set_frame_as_busy(void *frame) {
        int value_frame = (int)(intptr_t)frame;
        log_info(logger, "Frame marcado como libre: %d", value_frame);
        bitarray_clean_bit(frames_bitmap, value_frame);
    }
    
    // Itera sobre todos los marcos de la lista y los marca como ocupados
    list_iterate(free_frames, set_frame_as_busy); 
} 

void write_memory(int client_socket, int id_process, char *content_to_write, int physical_address) {
    // Preparamos el contenido a escribir en memoria
    int data_size = string_length(content_to_write);

    // Inicializamos variable para manejar la respuesta al cliente:
    int response;
    char *process_status;

    if ((physical_address + data_size > config_memoria->TAM_MEMORIA) || (physical_address > config_memoria->TAM_PAGINA)) {
        process_status = "ERROR";
        response = ERROR;
    } else {
        memcpy(espacio_usuario + physical_address, content_to_write, data_size);
        process_status = "OK";
        response = OK;
    }

    // Informamos situacion
    log_info(logger, "Process %d: Write to memory (physical_address: %d, size: %d bytes) - Status: %s", id_process, physical_address, data_size, process_status);
    send(client_socket, &response, sizeof(int), 0);
}

void read_memory(int client_socket, int id_process, int quantity_bytes, int physical_address) {
    // Preparamos el contenido a leer de memoria:
    void *buffer = malloc(quantity_bytes); 

    // Inicializamos variable para manejar la respuesta al cliente:
    int response;
    char *process_status;

    if ((physical_address + quantity_bytes > config_memoria->TAM_MEMORIA) || (physical_address > config_memoria->TAM_PAGINA)) {
        process_status = "ERROR";
        response = ERROR;
    } else {
        memcpy(buffer, espacio_usuario + physical_address, quantity_bytes);
        process_status = "OK";
        response = OK;
    }

    // Informamos situacion:
    log_info(logger, "Process %d: Read from memory (physical_address: %d, size: %d bytes) - Status: %s", id_process, physical_address, quantity_bytes, process_status);
    send_read_content(client_socket, buffer, response);
}