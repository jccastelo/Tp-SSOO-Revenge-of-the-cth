#include "../include/memoria-state.h"

// Definimos las variables globales:
t_config_memoria *config_memoria;
t_memoria_servers *memoria_servers;
t_dictionary *instrucciones_por_procesos;
t_dictionary *metricas_por_procesos;
void *espacio_usuario;

void memoria_state_init() {
    // Inicializamos los diccionarios:
    instrucciones_por_procesos = dictionary_create();
    metricas_por_procesos = dictionary_create();

    // Inicializamos la memoria:
    espacio_usuario = malloc(config_memoria->TAM_MEMORIA);

    if (espacio_usuario == NULL) {
        log_error(logger, "No se pudo reservar espacio en memoria");
        exit(EXIT_FAILURE);
    }
}