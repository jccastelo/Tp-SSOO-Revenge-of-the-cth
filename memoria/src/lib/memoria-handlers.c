#include "../include/memoria-handlers.h"

void memoria_server_escucha_handler(int client_socket, int operation, const char *server_name) {

    t_buffer* new_buffer = malloc(sizeof(t_buffer));
    new_buffer->size = 0;
    new_buffer->stream = NULL;

    if((operation != HANDSHAKE))
        {
            new_buffer->stream = recibir_buffer(&new_buffer->size, client_socket);
        }


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
    
    } 

    if (operation == EXIT_Sys) {
        int ok=51;

        int enviados = send(client_socket, &ok, sizeof(int), 0);
        if (enviados <= 0) {
            perror("send");
            log_error(logger, "Error al enviar respuesta EXIST SYSC");
        }

        log_info(logger,"POdes ELIMINAR el proceso pa");
        
        
    } 
}