#include "include/kernel-init-connection.h" 

void kernel_servers_init(void) {
    // Inicializamos variables:
    kernel_servers = malloc(sizeof(t_kernel_servers));

    // Inicializamos la estructura de servidores:
    char *puertoEScuchaDispatch = string_itoa(config_kernel->PUERTO_ESCUCHA_DISPATCH);
    setup_server("DISPATCH",config_kernel->IP_SERVER, puertoEScuchaDispatch,  set_dispatcher_id);

    char *puertoEscuchaIO=string_itoa(config_kernel->PUERTO_ESCUCHA_IO);
    setup_server("I0",config_kernel->IP_SERVER,puertoEscuchaIO , set_io_id);

    char *puertoEscuchaInterrupt=string_itoa(config_kernel->PUERTO_ESCUCHA_INTERRUPT);
    setup_server("INTERRUPT",config_kernel->IP_SERVER, puertoEscuchaInterrupt, set_interrupt_id);

    free(puertoEScuchaDispatch);
    free(puertoEscuchaIO);
    free(puertoEscuchaInterrupt);
}

void set_dispatcher_id (int id_server) {
    kernel_servers->id_server_dispatch = id_server;
    log_warning(logger, "ID del servidor DISPATCH: %d", kernel_servers->id_server_dispatch);
}

void set_io_id (int id_server) {
    kernel_servers->id_server_io = id_server;
    log_warning(logger, "ID del servidor IO: %d", kernel_servers->id_server_io);
}

void set_interrupt_id (int id_server) {
    kernel_servers->id_server_interrupt = id_server;
    log_warning(logger, "ID del servidor INTERRUPT: %d", kernel_servers->id_server_interrupt);
}