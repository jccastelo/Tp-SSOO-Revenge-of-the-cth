#include "include/cpu.h"

int main(int argc, char* argv[]) {

    // Conseguimos la ID de la CPU ./cpu [ID]
    if (argc < 2) {
        abort(); // aborta si no se le pasó un identificador
    }
    id_cpu = argv[1];

    // Inicializamos un logger:
    char log_name[10];
    sprintf(log_name, "cpu%s.log", id_cpu);
    logger = log_create(log_name, "CPU", true, LOG_LEVEL_INFO);

    // Inicializamos la configuración
    cpu_config_init();

    //Conectamos CPU a Kernel y memoria
    cpu_connect();

    while (1) {
        
        recibir_contexto_de_kernel();

        realizar_ciclo_de_instruccion();
    }
    
    return 0;
}
