#include "include/memoria-server-init.h" 

void memoria_servers_init(void) {
    // Inicializamos variables:
    memoria_servers = malloc(sizeof(t_memoria_servers));

    // Inicializamos la estructura de servidores:
    setup_server("Memoria", "127.0.0.1", string_itoa(config_memoria->PUERTO_ESCUCHA),  set_escucha_id);//Rellenar variable de IP
}

void set_escucha_id (int id_server) {
    memoria_servers->id_server_escucha_memoria = id_server;
    log_info(logger, "ID del servidor Memoria: %d", memoria_servers->id_server_escucha_memoria);
}
