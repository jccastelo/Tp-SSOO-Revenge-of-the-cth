#include "../include/memoria-state.h"

// Definimos las variables globales:
t_config_memoria *config_memoria = NULL;
t_memoria_servers *memoria_servers = NULL;
t_dictionary *instrucciones_por_procesos = NULL;
t_dictionary *metricas_por_procesos = NULL;
t_dictionary *all_process_page_tables = NULL;
t_bitarray *frames_bitmap = NULL;
void *espacio_usuario = NULL;
void *frames_memory = NULL;
int quantity_frames;
FILE* archivo_swap = NULL;
t_dictionary* diccionario_swap_metadata = NULL;

void memoria_state_init() {
    // Inicializamos las variables globales:
    instrucciones_por_procesos = dictionary_create();
    metricas_por_procesos = dictionary_create();
    all_process_page_tables = dictionary_create();
    espacio_usuario = malloc(config_memoria->TAM_MEMORIA);
    quantity_frames = config_memoria->TAM_MEMORIA / config_memoria->TAM_PAGINA;

    // Inicializamos el bitmap que representa qué frames están libres u ocupados en memoria:
    init_free_frames_bitmap();
    init_swap();
    
    if (!espacio_usuario) {
        log_error(logger, "No se pudo reservar espacio en memoria");
        exit(EXIT_FAILURE);
    }
}

void memory_handle_shutdown(int signal) {

    // ---------- CLOSURES PARA DESTRUCCIÓN ----------

    void closure_destroy_instructions(void* instructions_list) {
        list_destroy_and_destroy_elements(instructions_list, free);
    }

    void closure_free_page_table(void* root_table_list) {
        destroy_page_table_structure((t_list*) root_table_list, 1, config_memoria->CANTIDAD_NIVELES);
    }

    void closure_destroy_swap_metadata(void* swap_metadata_list) {
        list_destroy_and_destroy_elements(swap_metadata_list, free);
    }

    // ---------- DESTRUCCIÓN DE DICCIONARIOS ----------

    dictionary_destroy_and_destroy_elements(instrucciones_por_procesos, closure_destroy_instructions);
    dictionary_destroy_and_destroy_elements(metricas_por_procesos, free);
    dictionary_destroy_and_destroy_elements(diccionario_swap_metadata, closure_destroy_swap_metadata);
    dictionary_destroy_and_destroy_elements(all_process_page_tables, closure_free_page_table);

    // ---------- DESTRUCCIÓN DE OTRAS VARIABLES ----------
    destroy_config_memoria(config_memoria);
    bitarray_destroy(frames_bitmap);
    fclose(archivo_swap);
    free(espacio_usuario);
    free(frames_memory);
    free(memoria_servers);
    log_destroy(logger);

    exit(0);  // Cierre limpio
}

void memory_setup_shutdown_signal() {
    struct sigaction sa;
    sa.sa_handler = memory_handle_shutdown;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Error al configurar la senial de cierre");
        exit(1);
    }
}