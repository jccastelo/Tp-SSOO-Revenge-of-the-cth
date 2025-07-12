#include "../include/memoria-state.h"

// Definimos las variables globales:
t_config_memoria *config_memoria;
t_memoria_servers *memoria_servers;
t_dictionary *instrucciones_por_procesos;
t_dictionary *metricas_por_procesos;
t_dictionary *all_process_page_tables;
t_bitarray *frames_bitmap;
void *espacio_usuario;
void *frames_memory;
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

void cerrar_programa(int signal) {
    if(signal == SIGINT) {
        // Cerramos el archivo de swap y liberamos la memoria del espacio de usuario
        fclose(archivo_swap);
        free(espacio_usuario);

        // Destruimos el bitmap de frames
        bitarray_destroy(frames_bitmap);

        // Destruimos los diccionarios de métricas e instrucciones por proceso
        dictionary_destroy_and_destroy_elements(metricas_por_procesos, free);
        dictionary_destroy_and_destroy_elements(instrucciones_por_procesos, free);

        // Definimos la función para limpiar y destruir cada tabla de páginas de procesos
        void closure_clean(void *root_table_ptr) {
            t_list *root_table = (t_list *) root_table_ptr;
            delete_page_tables(1, config_memoria->CANTIDAD_NIVELES, root_table);
        }

        // Destruimos el diccionario que contiene las tablas de páginas de todos los procesos
        dictionary_destroy_and_destroy_elements(all_process_page_tables, closure_clean);

        // Destruimos la configuración general de memoria
        destruir_config_memoria(config_memoria);
    }
}

void configurar_senial_cierre() {
    struct sigaction sa;
    sa.sa_handler = cerrar_programa;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Error al configurar la senial de cierre");
        exit(1);
    }
}