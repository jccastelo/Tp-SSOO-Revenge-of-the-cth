#include "include/kernel.h"

int main(int argc, char *argv[]) {

    /*
    if (argc < 3) {
        fprintf(stderr, "Uso: %s <mensaje> <Tamanio_prco>\n", argv[0]);
        return 1;
    }

    char *mensaje = argv[1];
    int Tamanio_proc = atoi(argv[2]);
    */

    // Inicializamos un logger
    logger = log_create("kernel.log", "KERNEL", true, LOG_LEVEL_INFO);

    char *archivo_pseudocodigo = "ARCHIVOPSEUDO";
    int Tamanio_proc = 7;

    // Inicializamos la configuración del kernel y los servidores:
    kernel_config_init();

    planner_init();

    if (planner == NULL || planner->long_term == NULL) {
    log_error(logger,"Planner no inicializado");
    return -1;
    }

    kernel_servers_init();

    // Comenzamos a escuchar las conexiones de los clientes:
    kernel_servers_listening();
    log_info(logger,"ESta escuchadno");

    //Nos conectamos a la memoria como clientes
    kernel_memory_connection();

    log_info(logger, "Esperando conexion de alguna CPU para iniciar primer proceso...");
    while(list_size(list_cpus->queue_ESTADO) == 0){}
    log_info(logger, "Primera Cpu conectada");

    //INICIO PRIMER PROCESO
    init_fist_process(archivo_pseudocodigo,Tamanio_proc);

    char *archivo_pseudocodigo2 = "ARCHI22222";
    int Tamanio_proc2 = 102;
    init_fist_process(archivo_pseudocodigo2,Tamanio_proc2 );

    char *archivo_pseudocodigo3 = "ARCHI3";
    int Tamanio_proc3 = 103;    
    init_fist_process(archivo_pseudocodigo3,Tamanio_proc3);

    char *archivo_pseudocodigo4 = "ARCHI4";
    int Tamanio_proc4 = 104;
    init_fist_process(archivo_pseudocodigo4,Tamanio_proc4);

    char *archivo_pseudocodigo5 = "ARCHI5";
    int Tamanio_proc5 = 105;
    init_fist_process(archivo_pseudocodigo5,Tamanio_proc5);


    // Nota: Esto es un parche para evitar que el programa termine inmediatamente.
    // Ya que detachamos los hilos, no podemos esperar a que terminen.]
    while(list_size(list_cpus->queue_ESTADO) > 0) {}
 
    terminar_kernel();
    return 0;


}