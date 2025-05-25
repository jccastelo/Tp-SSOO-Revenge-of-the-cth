#include "include/io.h"

int main(int argc, char *argv[]) {

    if (argc != 1) {
        fprintf(stderr, "Error: se debe inicia una instancia de IO con su nombre");
        return 1;
    }

    char *nombre = strdup(argv[0]);

    // Inicializamos un logger
    logger = log_create("io.log", "IO", true, LOG_LEVEL_INFO);

    // Inicializamos la configuración de IO y nos conectamos al servidor:
    io_config_init();
    io_connect(nombre);

    ciclo_de_io();

    atexit(fin_de_io());

    return 0;
}