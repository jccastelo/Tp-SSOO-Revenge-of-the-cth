#include "include/memoria.h"

int main(int argc, char* argv[]) {

    char* config_path;
    config_path = argv[1];

    memoria_config_init(config_path);

    // Inicializamos un logger:
    logger = log_create("memoria.log", "MEMORIA", true, log_level_from_string(config_memoria->LOG_LEVEL));

    // Inicializamos la configuración del memoria, y las variables globales:
    // configurar_senial_cierre();
    memoria_config_init(config_path);
    memory_setup_shutdown_signal();
    memoria_state_init();

    // Inicializamos el server y comenzamos a escuchar:
    memoria_servers_init();
    memoria_servers_listening();

    return 0;
}
