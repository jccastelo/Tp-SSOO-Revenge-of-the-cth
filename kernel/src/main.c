#include "include/kernel.h"

int main() {
    // Inicializamos un logger
    logger = log_create("kernel.log", "KERNEL", true, LOG_LEVEL_INFO);

    // Inicializamos la configuración del kernel y los servidores:
    kernel_config_init();
    kernel_servers_init();

    // Comenzamos a escuchar las conexiones de los clientes:
    kernel_servers_listening();

    //Nos conectamos a la memoria como clientes
    kernel_memory_connection();

    

    //PLANIFICADOR LARGO, MEDIANO, CORTO PLAZO
    planner_init();
    
    //RAMA TEEEEEEEEEEEEEEEEEEEEEEEEEEEEST

    // Nota: Esto es un parche para evitar que el programa termine inmediatamente.
    // Ya que detachamos los hilos, no podemos esperar a que terminen.]
    while(1) {
        
    }
 
    return 0;
}