#include "io.h"

int main() {
    // Inicializamos un logger
    logger = log_create("io.log", "IO", true, LOG_LEVEL_INFO);

    // Inicializamos la configuración de IO
    io_config_init();

    return 0;
}
