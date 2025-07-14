#include "include/cpu.h"

int main(int argc, char* argv[]) {

    // Conseguimos la ID de la CPU ./cpu [ID] [CONFIG_PATH]
    id_cpu = argv[1];
    char* config_path = argv[2];

    // Inicializamos la configuración
    cpu_config_init(config_path);

    // Inicializamos un logger:
    char log_name[10];
    sprintf(log_name, "cpu%s.log", id_cpu);
    logger = log_create(log_name, "CPU", true, log_level_from_string(config_cpu->LOG_LEVEL));
    log_info(logger, "cpu iniciada con ID %s", id_cpu);

    if (argc < 2) {
        log_error(logger, "CPU MAL INICIALIZADA");
    }

    // Conectamos CPU a Kernel y memoria
    cpu_connect();

    // Ejecutamos el ciclo de instrucciones durante la vida del cpu
    while (1) {

        recibir_contexto_de_kernel();

        // Inicializamos la TLB y la memoria CACHE
        inicializar_tlb();
        inicializar_cache();

        realizar_ciclo_de_instruccion();

        // Limpiamos al ser desalojado el proceso
        limpiar_tlb();
        limpiar_cache();
        free(contexto);
    }
    
    return 0;
}
