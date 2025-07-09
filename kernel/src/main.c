#include "include/kernel.h"

int main(int argc, char *argv[]) {

    // Inicializamos un logger
    logger = log_create("kernel.log", "KERNEL", true, LOG_LEVEL_INFO);

    char *archivo_pseudocodigo;
    int Tamanio_proc;

    if (argc < 3) {
        //fprintf(stderr, "Uso: %s <archivoProceso> <Tamanio_prco>\n", argv[0]);
        //return 1;
        log_warning(logger, "INICIALIZADO EN MODO DEFAULT");
        archivo_pseudocodigo = "MEMORIA_IO.txt"; //PARA DEBUG O POR DEFAULT
        Tamanio_proc = 90;
    } else {
        archivo_pseudocodigo = argv[1];
        Tamanio_proc = atoi(argv[2]);
    }

    // Inicializamos la configuración del kernel y los servidores:
    kernel_config_init();

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
    
    signal(SIGINT, terminar_kernel);

    getchar();

    // INICIO PRIMER PROCESO
    init_fist_process(archivo_pseudocodigo,Tamanio_proc);

    // Nota: Esto es un parche para evitar que el programa termine inmediatamente.
    // Ya que detachamos los hilos, no podemos esperar a que terminen.
    while(list_size(list_cpus->cola) > 0) {}

    return 0;
}