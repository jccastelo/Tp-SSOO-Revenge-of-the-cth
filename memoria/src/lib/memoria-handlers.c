#include "../include/memoria-handlers.h"

void memoria_server_escucha_handler(int client_socket, int operation, const char *server_name) {
    switch (operation) {
        case HANDSHAKE:
            recibir_handshake(client_socket);
            log_info(logger, "Se ha recibido un handshake del socket %d ", client_socket);
        break;
        case INIT_PROC:
            log_info(logger, "Kernel solicito proceso");
            init_process(client_socket);
            log_info(logger, "Proceso acpetado para iniciar");
        break;
        case EXIT_SYS:
            //Hardcode que siempre diga si eliminalo, no se que tiene que chequear
            log_info(logger, "Kernel solicito Elinar proceso");

            //Es importante siempre gacer el recv del buffer aunque no haga nada porque sino,
            // cuando haga otro recv lo voy a agarrar a este

            int ELIMINALO = 51;
            send(client_socket,&ELIMINALO,sizeof(int),0);
        break;
        case GET_INSTRUCTION:
            
            log_info(logger, "CPU ME PIDIOOOOO INSTRUCCUON, NO HAGA NADA GG");
            sleep(5);            
            //Es importante siempre gacer el recv del buffer aunque no haga nada porque sino,
            // cuando haga otro recv lo voy a agarrar a este

        break;


        // Otros casos de operaciones pueden ir aquí.
        default:
            log_warning(logger, "Operación desconocida: %d", operation);
        break;
    }
}