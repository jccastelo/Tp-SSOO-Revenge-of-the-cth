#include "include/memoria.h"

int main(int argc, char* argv[]) {
    // Inicializamos un logger:
    logger = log_create("memoria.log", "MEMORIA", true, LOG_LEVEL_INFO);

    // Inicializamos la configuración del memoria, y las variables globales:
    memoria_config_init();
    memoria_state_init();

    // Inicializamos el server y comenzamos a escuchar:
    memoria_servers_init();
    memoria_servers_listening();

    return 0;
}
