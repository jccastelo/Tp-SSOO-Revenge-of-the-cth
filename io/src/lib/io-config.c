#include "include/io-config.h"

t_config_io *inicializar_config_io() {
    t_config_io *config_io = malloc(sizeof(t_config_io));

    // Inicializamos los valores de la estructura:
    config_io->IP_KERNEL = NULL;
    config_io->PUERTO_KERNEL = 0;
    config_io->LOG_LEVEL = NULL;

    return config_io;
}

void io_config_init() {
    // Inicializamos variables:
    config_io = inicializar_config_io();
    t_config* config = config_create("io.config");

    // Verificamos que el archivo de configuración se haya abierto correctamente:
    if (config == NULL) {
        log_error(logger, "No se pudo abrir el archivo de configuración");
        exit(EXIT_FAILURE);
    }

    // Establecemos la configuración de IO:
    configurar_valores_de_kernel(config_io, config);
    configurar_valores_de_log(config_io, config);
    config_destroy(config);

    // Verificamos que la configuración de IO se haya inicializado correctamente:
    log_info(logger, "Configuración de IO inicializada correctamente");
}

void configurar_valores_de_kernel(t_config_io *config_io, t_config *config) {
    config_io->IP_KERNEL = strdup(config_get_string_value(config, "IP_KERNEL"));
    config_io->PUERTO_KERNEL = config_get_int_value(config, "PUERTO_KERNEL");
}

void configurar_valores_de_log(t_config_io *config_io, t_config *config) {
    config_io->LOG_LEVEL = strdup(config_get_string_value(config, "LOG_LEVEL"));
}