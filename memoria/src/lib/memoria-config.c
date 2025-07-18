#include "../include/memoria-config.h"

t_config_memoria* inicializar_config_memoria() {
    t_config_memoria* config_memoria = malloc(sizeof(t_config_memoria));

    config_memoria->IP_SERVER = NULL;
    config_memoria->IP_MEMORIA = NULL; // Fixed initialization
    config_memoria->PUERTO_ESCUCHA = 0;
    config_memoria->TAM_MEMORIA = 0;
    config_memoria->TAM_PAGINA = 0;
    config_memoria->ENTRADAS_POR_TABLA = 0;
    config_memoria->CANTIDAD_NIVELES = 0;
    config_memoria->RETARDO_MEMORIA = 0;
    config_memoria->PATH_SWAPFILE = NULL; // Fixed initialization
    config_memoria->PATH_INSTRUCCIONES = NULL;
    config_memoria->RETARDO_SWAP = 0;
    config_memoria->LOG_LEVEL = NULL;
    config_memoria->DUMP_PATH = NULL; // Fixed initialization

    return config_memoria; // Ensure the allocated memory is returned
}

void memoria_config_init(char* config_path) {
    // Inicializamos variables:
    config_memoria = inicializar_config_memoria();
    t_config* config = config_create(config_path);

    // To Do: Implementar una función que valide la existencia de los parámetros en el archivo de configuración
    // Establecemos la configuración del kernel:
    configurar_valores_de_puerto(config_memoria, config);
    configurar_valores_de_tamanios(config_memoria, config);
    configurar_valores_de_paths(config_memoria, config);
    configurar_valores_de_log(config_memoria, config);
    config_destroy(config);
}

void configurar_valores_de_puerto(t_config_memoria* config_io, t_config* config) {
    config_memoria->IP_SERVER = strdup(config_get_string_value(config, "IP_SERVER"));
    config_memoria->IP_MEMORIA = strdup(config_get_string_value(config, "IP_MEMORIA"));
    config_memoria-> PUERTO_ESCUCHA = config_get_int_value(config, "PUERTO_ESCUCHA");
}

void configurar_valores_de_tamanios(t_config_memoria* config_io , t_config* config) {
    config_memoria-> TAM_MEMORIA = config_get_int_value(config,"TAM_MEMORIA");
    config_memoria-> TAM_PAGINA = config_get_int_value(config, "TAM_PAGINA");
    config_memoria-> ENTRADAS_POR_TABLA = config_get_int_value(config,"ENTRADAS_POR_TABLA");
    config_memoria-> CANTIDAD_NIVELES = config_get_int_value(config, "CANTIDAD_NIVELES");
    config_memoria-> RETARDO_SWAP = config_get_int_value(config, "RETARDO_SWAP"); 
    config_memoria-> RETARDO_MEMORIA = config_get_int_value(config, "RETARDO_MEMORIA");
}

void configurar_valores_de_paths(t_config_memoria* config_io, t_config* config) {
    config_memoria-> PATH_SWAPFILE = strdup(config_get_string_value(config,"PATH_SWAPFILE"));
    config_memoria->PATH_INSTRUCCIONES = strdup(config_get_string_value(config, "PATH_INSTRUCCIONES"));
    config_memoria-> DUMP_PATH = strdup(config_get_string_value(config,"DUMP_PATH"));
}

void configurar_valores_de_log(t_config_memoria* config_io, t_config* config) {
    config_memoria->LOG_LEVEL = strdup(config_get_string_value(config, "LOG_LEVEL"));
}

void destroy_config_memoria(t_config_memoria* config) {
    if (config == NULL) return;

    free(config->IP_SERVER);
    free(config->IP_MEMORIA);
    free(config->PATH_SWAPFILE);
    free(config->PATH_INSTRUCCIONES);
    free(config->LOG_LEVEL);
    free(config->DUMP_PATH);

    free(config);  // finalmente, liberar la estructura
}