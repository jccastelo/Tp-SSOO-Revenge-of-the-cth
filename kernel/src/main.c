#include "include/kernel.h"

int main(int argc, char *argv[]) {

    char *archivo_pseudocodigo;
    int Tamanio_proc;
    char *config_path;

    // ./bin/kernel "ARCHIVO" TAMAÑO ./path

    archivo_pseudocodigo = argv[1];
    Tamanio_proc = atoi(argv[2]);
    config_path = argv[3];

    signal(SIGINT, terminar_kernel);

    // Inicializamos la configuración del kernel y los servidores:
    kernel_config_init(config_path);

    // Inicializamos un logger
    logger = log_create("kernel.log", "KERNEL", true, LOG_LEVEL_INFO);

    planner_init();

    if (planner == NULL || planner->long_term == NULL) {
        log_error(logger,"Planner no inicializado");
        return -1;
    }

    kernel_servers_init();

    // // Comenzamos a escuchar las conexiones de los clientes:
    kernel_servers_listening();

    // //Nos conectamos a la memoria como clientes
    kernel_memory_connection();
   
    while(list_size(list_cpus->cola) == 0) {}
    

    getchar();

    // INICIO PRIMER PROCESO
    init_fist_process(archivo_pseudocodigo,Tamanio_proc);

    // Nota: Esto es un parche para evitar que el programa termine inmediatamente.
    // Ya que detachamos los hilos, no podemos esperar a que terminen.
    while(list_size(list_cpus->cola) > 0) {}

    return 0;
}