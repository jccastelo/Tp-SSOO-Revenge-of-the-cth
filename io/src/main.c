#include "include/io.h"

int main() {
    // Inicializamos un logger
    logger = log_create("io.log", "IO", true, LOG_LEVEL_INFO);

    // Inicializamos la configuración de IO y nos conectamos al servidor:
    io_config_init();
    io_connect();

    return 0;
}