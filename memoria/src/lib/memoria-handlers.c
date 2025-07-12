#include "../include/memoria-handlers.h"

void memoria_server_escucha_handler(int client_socket, int operation, const char *server_name) {
    switch (operation) {
        case HANDSHAKE:
            recibir_handshake(client_socket);
            log_debug(logger, "Se ha recibido un handshake del socket %d ", client_socket);
        break;
        case INIT_PROC:
            log_info(logger, "## Kernel Conectado - FD del socket: %d", client_socket);
            init_process(client_socket);
        break;
        case EXIT_SYS:
            finish_process(client_socket);
        break;
        case GET_INSTRUCTION:
            send_process_instruction(client_socket);
        break; 
        case SUSPENDER:
            log_info(logger, "## Kernel Conectado - FD del socket: %d", client_socket);
            suspend_process(client_socket);
            close(client_socket);
        break; 
        case DESUSPENDER:
            log_info(logger, "## Kernel Conectado - FD del socket: %d", client_socket);
            remove_suspend_process(client_socket);
            close(client_socket);    
        break;
        case DUMP_MEMORY:
            log_info(logger, "## Kernel Conectado - FD del socket: %d", client_socket);
            dump_process(client_socket);
            close(client_socket);
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
            send_values_memory(client_socket);
        break;
        // Otros casos de operaciones pueden ir aquí.
        default:
            log_debug(logger, "Operación desconocida: %d", operation);
        break;
    }
}