#include <include/utils.h>


int main(int argc, char* argv[]) {

    t_config_kernel* config_kernel;
    iniciar_logger(log_level);

    leer_configuraciones(config_kernel);

    return 0;
}
