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
            finish_process(client_socket);
        break;
        case GET_INSTRUCTION:
            send_process_instruction(client_socket);
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
        // Otros casos de operaciones pueden ir aquí.
        default:
            log_warning(logger, "Operación desconocida: %d", operation);
        break;
    }
}