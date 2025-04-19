
#include "../include/cpu-config.h"

t_config_cpu* inicializar_config_cpu() {
    t_config_cpu* config_cpu = malloc(sizeof(t_config_cpu));
    
    // Inicializamos los valores de la estructura:
    config_cpu->IP_MEMORIA = NULL;
    config_cpu->PUERTO_MEMORIA = 0;
    config_cpu->IP_KERNEL = NULL;
    config_cpu->PUERTO_KERNEL_DISPATCH = 0;
    config_cpu->PUERTO_KERNEL_INTERRUPT = 0;
    config_cpu->ENTRADAS_TLB = 0;
    config_cpu->REEMPLAZO_TLB = NULL;
    config_cpu->ENTRADAS_CACHE = 0;
    config_cpu->REEMPLAZO_CACHE = 0;
    config_cpu->RETARDO_CACHE = 0;   
    config_cpu->LOG_LEVEL = NULL;

    return config_cpu;
}

void cpu_config_init() {
    // Inicializamos variables:
    config_cpu = inicializar_config_cpu();
    t_config* config = config_create("cpu.config");

    // Verificamos que el archivo de configuración se haya abierto correctamente:
    if (config == NULL) {
        log_error(logger, "No se pudo abrir el archivo de configuración");
        exit(EXIT_FAILURE);
    }

    // To Do: Implementar una función que valide la existencia de los parámetros en el archivo de configuración
    // Establecemos la configuración del cpu:
    configurar_valores_de_memoria(config_cpu, config);
    configurar_valores_de_kernel(config_cpu, config);
    configurar_valores_de_tlb(config_cpu, config);
    configurar_valores_de_cache(config_cpu, config);
    configurar_valores_de_log(config_cpu, config);
    config_destroy(config);

    // Verificamos que la configuración del cpu se haya inicializado correctamente:
    log_info(logger, "Configuración del cpu inicializada correctamente");
}

void configurar_valores_de_memoria(t_config_cpu* config_cpu, t_config* config) {
    config_cpu->IP_MEMORIA = strdup(config_get_string_value(config, "IP_MEMORIA"));
    config_cpu->PUERTO_MEMORIA = config_get_int_value(config, "PUERTO_MEMORIA");
}

void configurar_valores_de_kernel(t_config_cpu* config_cpu, t_config* config) {
    config_cpu->IP_KERNEL = strdup(config_get_string_value(config, "IP_KERNEL"));
    config_cpu->PUERTO_KERNEL_DISPATCH = config_get_int_value(config, "PUERTO_KERNEL_DISPATCH");
    config_cpu->PUERTO_KERNEL_INTERRUPT = config_get_int_value(config, "PUERTO_KERNEL_INTERRUPT");
}

void configurar_valores_de_tlb(t_config_cpu* config_cpu, t_config* config) {
    config_cpu->ENTRADAS_TLB = config_get_int_value(config, "ENTRADAS_TLB");
    config_cpu->REEMPLAZO_TLB = strdup(config_get_string_value(config, "REEMPLAZO_TLB"));
}

void configurar_valores_de_cache(t_config_cpu *config_cpu, t_config *config) {
    config_cpu->ENTRADAS_CACHE = config_get_int_value(config, "ENTRADAS_CACHE");
    config_cpu->REEMPLAZO_CACHE = strdup(config_get_string_value(config, "REEMPLAZO_CACHE"));
    config_cpu->RETARDO_CACHE = config_get_int_value(config, "RETARDO_CACHE");
}

void configurar_valores_de_log(t_config_cpu* config_cpu, t_config* config) {
    config_cpu->LOG_LEVEL = strdup(config_get_string_value(config, "LOG_LEVEL"));
}