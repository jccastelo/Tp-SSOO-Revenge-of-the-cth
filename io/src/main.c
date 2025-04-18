#include "main.h"

int main(int argc, char* argv[]) {
    
    char *ip_kernel = NULL;
    char *log_level = NULL;
    char *puerto_kernel = NULL;

    iniciar_logger();

    leer_configuraciones(&ip_kernel, &puerto_kernel, &log_level);
    crear_conexion_con_kernel(ip_kernel, puerto_kernel); 
    
    
    escucha_peticiones_kernel();

    return 0;
}