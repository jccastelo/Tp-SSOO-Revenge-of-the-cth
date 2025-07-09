#include "../include/memoria-handlers.h"

void memoria_server_escucha_handler(int client_socket, int operation, const char *server_name) {
    switch (operation) {
        case HANDSHAKE:
            recibir_handshake(client_socket);
            log_info(logger, "Se ha recibido un handshake del socket %d ", client_socket);
        break;
        case INIT_PROC:
            init_process(client_socket);
        break;
        case EXIT_SYS:

            //Hardcode que siempre diga si eliminalo, no se que tiene que chequear
           // log_info(logger, "Kernel solicito Eliminar proceso");

            //Es importante siempre gacer el recv del buffer aunque no haga nada porque sino,
            // cuando haga otro recv lo voy a agarrar a este
            finish_process(client_socket);
            //int ELIMINALO = 1;
            //send(client_socket,&ELIMINALO,sizeof(int),0);

        break;
        case GET_INSTRUCTION:
            send_process_instruction(client_socket);
        break; 
        case SUSPENDER:
            suspend_process(client_socket);
        break; 
        case DESUSPENDER:
            remove_suspend_process(client_socket);
            
        break;
        case DUMP_MEMORY:
            dump_process(client_socket);
        break;
      
        case GET_FRAME:
            access_to_page_tables(client_socket);
        break;
        case WRITE_MEM:
            write_in_user_spaces(client_socket);
        break;
        case READ_MEM:
            read_in_user_spaces(client_socket);
        break;
        case MEMORY_CONFIG:
            log_info(logger,"QIESO");
            send_values_memory(client_socket);
        break;
        // Otros casos de operaciones pueden ir aquí.
        default:
            log_warning(logger, "Operación desconocida: %d", operation);
        break;
    }
}