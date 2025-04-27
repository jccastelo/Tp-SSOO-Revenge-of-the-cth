#include "../include/memoria-servers-listening.h"

void memoria_servers_listening() {
    int server_socket = memoria_servers->id_server_escucha_memoria;

    server_with_thread(
        server_socket, 
        "Memoria", 
        conection_strategy_persistence, 
        memoria_server_escucha_handler);
}