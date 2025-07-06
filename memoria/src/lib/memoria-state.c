#include "../include/memoria-state.h"

// Definimos las variables globales:
t_config_memoria *config_memoria;
t_memoria_servers *memoria_servers;
t_dictionary *instrucciones_por_procesos;
t_dictionary *metricas_por_procesos;
t_dictionary *all_process_page_tables;
void *espacio_usuario;
void *frames_memory;
void *frames_bitmap;
int quantity_frames;

void memoria_state_init() {
    // Inicializamos las variables globales:
    instrucciones_por_procesos = dictionary_create();
    metricas_por_procesos = dictionary_create();
    all_process_page_tables = dictionary_create();
    espacio_usuario = malloc(config_memoria->TAM_MEMORIA);
    quantity_frames = config_memoria->TAM_MEMORIA / config_memoria->TAM_PAGINA;

    // Inicializamos el bitmap que representa qué frames están libres u ocupados en memoria:
    init_free_frames_bitmap();

    if (!espacio_usuario) {
        log_error(logger, "No se pudo reservar espacio en memoria");
        exit(EXIT_FAILURE);
    }
}