#include <include/utils.h>

int main(int argc, char* argv[]) {
    
    char* ip_kernel, log_level, puerto_kernel;

    iniciar_logger();

    leer_configuraciones(ip_kernel, puerto_kernel, log_level);
    
    crear_conexion("KERNEL", ip_kernel, puerto_kernel); 
    
    escucha_peticiones_kernel();

    return 0;
}