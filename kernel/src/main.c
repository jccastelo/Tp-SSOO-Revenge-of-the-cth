#include "include/kernel.h"

int main() {
    // Inicializamos un logger
    logger = log_create("kernel.log", "KERNEL", true, LOG_LEVEL_INFO);

    // Inicializamos la configuración del kernel y los servidores:
    kernel_config_init();

    // Inializa los servidores de escucha de IO, DISPATCH e INTERRUPT
    kernel_servers_init();

    // lei escuchati
    // kernel_servers_oid();

    // liberar kernel-server

    return 0;
}