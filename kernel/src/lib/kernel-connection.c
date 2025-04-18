#include "include/kernel-connection.h" 

void kernel_servers_init(void) {
    // Inicializamos variables:
    kernel_servers = malloc(sizeof(t_kernel_servers));

    // Inicializamos la estructura de servidores:
    server_init("DISPATCH",config_kernel->IP_MEMORIA, string_itoa(config_kernel->PUERTO_ESCUCHA_DISPATCH),  set_dispatcher_id);
    server_init("I0",config_kernel->IP_MEMORIA, string_itoa(config_kernel->PUERTO_ESCUCHA_IO), set_io_id);
    server_init("INTERRUPT",config_kernel->IP_MEMORIA, string_itoa(config_kernel->PUERTO_ESCUCHA_INTERRUPT), set_interrupt_id);        
}

void server_init(char *server_name, char *ip, char *puerto, void (*callback)(int id_server)) {
    int id_server;
    id_server = iniciar_servidor(server_name, ip, puerto);
    
    if(id_server == -1) {
        log_error(logger, "Error al iniciar el servidor %s", server_name);
        exit(EXIT_FAILURE);
    }

    // Llamamos al callback con el id del servidor, donde se puede asignar el id_server:
    log_info(logger, "Servidor %s iniciado con id %d", server_name, id_server);
    callback(id_server);
}

void set_dispatcher_id (int id_server) {
    kernel_servers->id_server_dispatch = id_server;
    log_info(logger, "ID del servidor DISPATCH: %d", kernel_servers->id_server_dispatch);
}

void set_io_id (int id_server) {
    kernel_servers->id_server_io = id_server;
    log_info(logger, "ID del servidor IO: %d", kernel_servers->id_server_io);
}

void set_interrupt_id (int id_server) {
    kernel_servers->id_server_interrupt = id_server;
    log_info(logger, "ID del servidor INTERRUPT: %d", kernel_servers->id_server_interrupt);
}