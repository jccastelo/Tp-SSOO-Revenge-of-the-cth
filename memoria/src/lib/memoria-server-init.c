#include "include/memoria-server-init.h" 

void memoria_servers_init(void) {
    // Inicializamos variables:
    memoria_server = malloc(sizeof(t_memoria_servers));

    // Inicializamos la estructura de servidores:
    setup_server("MEMORIA_ESCUCHA","", string_itoa(config_memoria->PUERTO_ESCUCHA),  set_escucha_id);//Rellenar variable de IP
    
}

void set_escucha_id (int id_server) {
    memoria_servers->id_server_escucha_memoria = id_server;
    log_info(logger, "ID del servidor ESCUCHA MEMORIA: %d", memoria_servers->id_server_escucha_memoria);
}
