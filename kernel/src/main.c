#include "include/kernel.h"

pthread_mutex_t mutex_control_kernel = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[]) {

    //KERNEL SIN LEAAAAKS!!!

    pthread_mutex_lock(&mutex_control_kernel);

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
    logger = log_create("kernel.log", "KERNEL", true, log_level_from_string(config_kernel->LOG_LEVEL));

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
    
    log_info(logger,"## Kernel esperando confirmacion para iniciar");
    getchar();

    // INICIO PRIMER PROCESO
    init_fist_process(archivo_pseudocodigo,Tamanio_proc);
    
    pthread_mutex_lock(&mutex_control_kernel);

    pthread_mutex_destroy(&mutex_control_kernel);
    
    return 0;
}