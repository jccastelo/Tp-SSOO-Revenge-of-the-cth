#include "include/memoria.h"

int main(int argc, char* argv[]) {

    char* config_path;
    config_path = argv[1];

    memoria_config_init(config_path);

    // Inicializamos un logger:
    logger = log_create("memoria.log", "MEMORIA", true, LOG_LEVEL_TRACE);

    // Inicializamos la configuración del memoria, y las variables globales:
    // configurar_senial_cierre();
    memoria_config_init(config_path);
    memoria_state_init();

    // Inicializamos el server y comenzamos a escuchar:
    memoria_servers_init();
    memoria_servers_listening();

    return 0;
}
