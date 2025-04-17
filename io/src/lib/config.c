
#include <../include/config.h>

void leer_configuraciones(char *ip_kernel, char *puerto_kernel, char *level_info_kernel){

    config = iniciar_config();

    ip_kernel = config_get_string_value(config, "IP_KERNEL");
	puerto_kernel = config_get_string_value(config , "PUERTO_KERNEL");
	level_info_kernel = config_get_string_value(config, "LOG_LEVEL");
}

t_config* iniciar_config(void) {

    t_config* nuevo_config = config_create("../io.config");

	if (nuevo_config == NULL)
	{
        abort();
	}
	
	return nuevo_config;
}