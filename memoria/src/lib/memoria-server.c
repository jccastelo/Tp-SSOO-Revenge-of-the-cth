#include "../include/memoria-server.h" 

void memoria_servers_init(void) {
    // Inicializamos variables:
    memoria_servers = malloc(sizeof(t_memoria_servers));
    char* puerto = string_itoa(config_memoria->PUERTO_ESCUCHA);

    // Inicializamos la estructura de servidores:
    setup_server("Memoria", config_memoria->IP_MEMORIA, puerto ,  set_escucha_id);
     // Rellenar variable de IP 
     free(puerto);
}

void set_escucha_id (int id_server) {
    memoria_servers->id_server_escucha_memoria = id_server;
    log_info(logger, "ID del servidor Memoria: %d", memoria_servers->id_server_escucha_memoria);
}
