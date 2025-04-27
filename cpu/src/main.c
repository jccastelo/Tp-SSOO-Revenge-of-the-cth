#include "include/cpu.h"

int main(int argc, char* argv[]) {
    // Inicializamos un logger:
    logger = log_create("cpu.log", "CPU", true, LOG_LEVEL_INFO);

    // Inicializamos la configuración del kernel y nos conectamos a él:
    cpu_config_init();

    // Conectamos CPU a Kernel
    cpu_connect();

    close(cpu_servers->socket_memoria);
    
    return 0;
}
