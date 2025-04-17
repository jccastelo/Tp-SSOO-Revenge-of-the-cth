#include "main.h"


int main(int argc, char* argv[]) {

    t_config_kernel* config_kernel;

    leer_configuraciones(config_kernel);

    iniciar_logger(config_kernel->log_level);

    int servidor = iniciar_servidor();

    escuchar_peticiones();//WTF!!!

    return 0;
}