#include "include/kernel.h"

int main() {
    // Inicializamos un logger
    logger = log_create("kernel.log", "KERNEL", true, LOG_LEVEL_INFO);

    // Inicializamos la configuración del kernel y los servidores:
    kernel_config_init();
    kernel_servers_init();

    // Comenzamos a escuchar las conexiones de los clientes:
    kernel_server_io_listening();

    return 0;
}

