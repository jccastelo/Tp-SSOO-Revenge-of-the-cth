#include "include/memoria.h"

int main(int argc, char* argv[]) {
    // Inicializamos un logger:
    logger = log_create("memoria.log", "MEMORIA", true, LOG_LEVEL_INFO);

    // Inicializamos la configuración del kernel y los servidores:
    memoria_config_init();

    //Inicializamos el server
    memoria_servers_init();

    // Comenzamos a escuchar las conexiones de los clientes:
    memoria_servers_listening();

    return 0;
}
