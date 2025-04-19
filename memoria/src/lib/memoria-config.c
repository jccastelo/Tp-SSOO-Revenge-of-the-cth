#include "../include/memoria-config.h"

t_config_memoria* inicializar_config_memoria(){
    t_config_memoria* config_memoria = malloc(sizeof(t_config_memoria));

    config_memoria->PUERTO_ESCUCHA=0;
    config_memoria->TAM_MEMORIA=0;
    config_memoria->TAM_PAGINA=0;
    config_memoria->ENTRADAS_POR_TABLA=0;
    config_memoria->CANTIDAD_NIVELES=0;
    config_memoria-> RETARDO_MEMORIA=0;
    config_memoria->PATH_SWAPFILE=0;
    config_memoria-> RETARDO_SWAP =0;
    config_memoria->LOG_LEVEL =NULL;
    config_memoria->DUMP_PATH =NULL;
}

void memoria_config_init(){
    // Inicializamos variables:
    config_memoria = inicializar_config_memoria();
    t_config* config = config_create("kernel.config");

    // Verificamos que el archivo de configuración se haya abierto correctamente:
    if (config == NULL) {
    log_error(logger, "No se pudo abrir el archivo de configuración");
    exit(EXIT_FAILURE);
    }

    // To Do: Implementar una función que valide la existencia de los parámetros en el archivo de configuración
    // Establecemos la configuración del kernel:
    configurar_valores_de_puerto(config_memoria, config);
    configurar_valores_de_tamanios(config_memoria, config);
    configurar_valores_de_paths(config_memoria, config);
    configurar_valores_de_log(config_memoria, config);
    config_destroy(config);

    // Verificamos que la configuración de la memoria se haya inicializado correctamente:
    log_info(logger, "Configuración de la memoria inicializada correctamente");
}


void configurar_valores_de_puerto(t_config_memoria* config_io, t_config* config){
    config_memoria-> PUERTO_ESCUCHA = strdup(config_get_int_value(config, "PUERTO_ESCUCHA"));

}

void configurar_valores_de_tamanios(t_config_memoria* config_io , t_config* config){
    config_memoria-> TAM_MEMORIA = strdup(config_get_int_value(config,"TAM_MEMORIA"));
    config_memoria-> TAM_PAGINA = strdup(config_get_int_value(config, "TAM_PAGINA"));
    config_memoria-> ENTRADAS_POR_TABLA = strdup(config_get_int_value(config,"ENTRADAS_POR_TABLA"));
    config_memoria-> CANTIDAD_NIVELES = strdup(config_get_int_value(config, "CANTIDAD_NIVELES"));
    config_memoria-> TAM_MEMORIA = strdup(config_get_int_value(config,"TAM_MEMORIA"));
    config_memoria-> RETARDO_SWAP = strdup(config_get_int_value(config, "RETARDO_SWAP"));
}
void configurar_valores_de_paths(t_config_memoria* config_io, t_config* config){
    config_memoria-> PATH_SWAPFILE = strdup(config_get_string_value(config,"PATH_SWAPFILE"));
    config_memoria-> DUMP_PATH = strdup(config_get_string_value(config,"DUMP_PATH"));
}

void configurar_valores_de_log(t_config_memoria* config_io, t_config* config) {
    config_memoria->LOG_LEVEL = strdup(config_get_string_value(config, "LOG_LEVEL"));
}