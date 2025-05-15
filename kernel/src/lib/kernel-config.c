#include "../include/kernel-config.h"

t_config_kernel* inicializar_config_kernel() {
    t_config_kernel* config_kernel = malloc(sizeof(t_config_kernel));
    
    // Inicializamos los valores de la estructura:
    config_kernel->IP_MEMORIA = NULL;
    config_kernel->PUERTO_MEMORIA = 0;
    config_kernel->PUERTO_ESCUCHA_DISPATCH = 0;
    config_kernel->PUERTO_ESCUCHA_INTERRUPT = 0;
    config_kernel->PUERTO_ESCUCHA_IO = 0;
    config_kernel->ALGORITMO_CORTO_PLAZO = NULL;
    config_kernel->ALGORITMO_INGRESO_A_READY = NULL;
    config_kernel->ALFA = 0.0;
    config_kernel->TIEMPO_SUSPENSION = 0;
    config_kernel->TIEMPO_DEADLOCK = 0;   
    config_kernel->LOG_LEVEL = NULL;

    return config_kernel;
}

void kernel_config_init() {
    // Inicializamos variables:
    config_kernel = inicializar_config_kernel();
    t_config* config = config_create("kernel.config");

    // Verificamos que el archivo de configuración se haya abierto correctamente:
    if (config == NULL) {
        log_error(logger, "No se pudo abrir el archivo de configuración");
        exit(EXIT_FAILURE);
    }

    // To Do: Implementar una función que valide la existencia de los parámetros en el archivo de configuración
    // Establecemos la configuración del kernel:
    configurar_valores_de_memoria(config_kernel, config);
    configurar_valores_de_servidores(config_kernel, config);
    configurar_valores_de_planificacion(config_kernel, config);
    configurar_valores_de_log(config_kernel, config);
    config_destroy(config);

    // Verificamos que la configuración del kernel se haya inicializado correctamente:
    log_info(logger, "Configuración del kernel inicializada correctamente");
}

void configurar_valores_de_memoria(t_config_kernel* config_io, t_config* config) {
    config_kernel->IP_MEMORIA = strdup(config_get_string_value(config, "IP_MEMORIA"));
    config_kernel->PUERTO_MEMORIA = config_get_int_value(config, "PUERTO_MEMORIA");
}


void configurar_valores_de_servidores(t_config_kernel* config_io, t_config* config) {
    config_kernel->PUERTO_ESCUCHA_DISPATCH = config_get_int_value(config, "PUERTO_ESCUCHA_DISPATCH");
    config_kernel->PUERTO_ESCUCHA_INTERRUPT = config_get_int_value(config, "PUERTO_ESCUCHA_INTERRUPT");
    config_kernel->PUERTO_ESCUCHA_IO = config_get_int_value(config, "PUERTO_ESCUCHA_IO");
}

// To Do: Implementar una función que valide los valores de configuración de los algoritmos de planificación
void configurar_valores_de_planificacion(t_config_kernel* config_io, t_config* config) {
    config_kernel->ALGORITMO_CORTO_PLAZO = strdup(config_get_string_value(config, "ALGORITMO_CORTO_PLAZO"));

    if (config_kernel->ALGORITMO_CORTO_PLAZO == NULL) {
    // Manejo de error si no se pudo asignar la cadena
    log_info(logger, "Error al asignar ALGORITMO_CORTO_PLAZO desde el archivo de configuración");}

    config_kernel->ALGORITMO_INGRESO_A_READY = strdup(config_get_string_value(config, "ALGORITMO_INGRESO_A_READY"));
    config_kernel->ALFA = config_get_double_value(config, "ALFA");
    config_kernel->TIEMPO_SUSPENSION = config_get_int_value(config, "TIEMPO_SUSPENSION");
}

void configurar_valores_de_log(t_config_kernel* config_io, t_config* config) {
    config_kernel->LOG_LEVEL = strdup(config_get_string_value(config, "LOG_LEVEL"));
}