#include "../include/memoria-handlers.h"

void memoria_server_escucha_handler(int client_socket, int operation, const char *server_name) {

    if (operation == HANDSHAKE) {
        recibir_handshake(client_socket);
    } 

    if (operation == INIT_PROC) {
        int ok=99;

        int enviados = send(client_socket, &ok, sizeof(int), 0);
        if (enviados <= 0) {
            perror("send");
            log_error(logger, "Error al enviar respuesta INIT_PROC");
        }

        log_info(logger,"POdes inciializar el proceso pa");
        sleep(1); // para darle tiempo al kernel a leer
        
    } 
}