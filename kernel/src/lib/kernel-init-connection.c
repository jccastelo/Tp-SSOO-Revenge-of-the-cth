#include "include/kernel-init-connection.h" 

void kernel_servers_init(void) {
    // Inicializamos variables:
    kernel_servers = malloc(sizeof(t_kernel_servers));

    // Inicializamos la estructura de servidores:
    setup_server("DISPATCH",config_kernel->IP_MEMORIA, string_itoa(config_kernel->PUERTO_ESCUCHA_DISPATCH),  set_dispatcher_id);    // config_kernle->IP_MEMORIA??
    setup_server("I0",config_kernel->IP_MEMORIA, string_itoa(config_kernel->PUERTO_ESCUCHA_IO), set_io_id);                         // config_kernle->IP_MEMORIA??
    setup_server("INTERRUPT",config_kernel->IP_MEMORIA, string_itoa(config_kernel->PUERTO_ESCUCHA_INTERRUPT), set_interrupt_id);    // config_kernle->IP_MEMORIA??    
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