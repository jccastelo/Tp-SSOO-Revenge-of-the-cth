#include "include/kernel.h"

int main(int argc, char *argv[]) {

    /*
    if (argc < 3) {
        fprintf(stderr, "Uso: %s <mensaje> <Tamanio_prco>\n", argv[0]);
        return 1;
    }*/

    char *archivo_pseudocodigo = "test.text";
    int Tamanio_proc = 32;


    // Inicializamos un logger
    logger = log_create("kernel.log", "KERNEL", true, LOG_LEVEL_INFO);


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
    log_info(logger,"ESta escuchadno");

    // //Nos conectamos a la memoria como clientes
    // kernel_memory_connection();

    log_info(logger, "Esperando conexion de alguna CPU para iniciar primer proceso...");
    while(list_size(list_cpus->cola) == 0){}
    // log_info(logger, "Primera Cpu conectada");

    //INICIO PRIMER PROCESO
    // init_fist_process(archivo_pseudocodigo,Tamanio_proc);


    // Nota: Esto es un parche para evitar que el programa termine inmediatamente.
    // Ya que detachamos los hilos, no podemos esperar a que terminen.]
    while(list_size(list_cpus->cola) > 0) {}
    // while(1) {}
 
    terminar_kernel();
    return 0;
}