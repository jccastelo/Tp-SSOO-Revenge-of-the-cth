#include "include/io.h"

int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Error: se debe inicia una instancia de IO con su nombre");
        return 1;
    }
    pid_proceso_Actual = -1;
    
    char *nombre = strdup(argv[1]);

     // Capturamos Ctrl + C
    signal(SIGINT, cierre_de_io);
    io_config_init();
    // Inicializamos un logger
    logger = log_create("io.log", "IO", true, log_level_from_string(config_io->LOG_LEVEL));

    // Inicializamos la configuración de IO y nos conectamos al servidor:
    
    io_connect(nombre);
   
    while (1)
    {
        ciclo_de_io();
    }
 }