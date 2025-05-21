#include "include/io.h"

int main(int argc, char *argv[]) {

    
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <nombreIO> \n", argv[0]);
        return 1;
    }

    nombreIO = argv[1];

    // Inicializamos un logger
    logger = log_create("io.log", "IO", true, LOG_LEVEL_INFO);

    // Inicializamos la configuración de IO y nos conectamos al servidor:
    io_config_init();
    io_connect();

    return 0;
}