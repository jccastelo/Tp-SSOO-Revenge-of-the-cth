#include "include/cpu.h"

int main(int argc, char* argv[]) {

    // Conseguimos la ID de la CPU ./cpu [ID]
    if (argc < 2) {
        id_cpu = "4";
    } else {
        id_cpu = argv[1];
    }
    

    // Inicializamos un logger:
    char log_name[10];
    sprintf(log_name, "cpu%s.log", id_cpu);
    logger = log_create(log_name, "CPU", true, LOG_LEVEL_INFO);
    log_info(logger, "cpu iniciada con ID %s", id_cpu);

    if (argc < 2) {
        log_warning(logger, "INICIALIZADA CPU EN MODO DEFAULT");
    }

    // Inicializamos la configuración
    cpu_config_init();

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
        guardado_cache_por_desalojo();
        limpiar_cache();
    }
    
    return 0;
}
