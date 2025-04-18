#ifndef CONFIGURACION_H_
#define CONFIGURACION_H_

#include "utils.h"

t_config* iniciar_config(void);
void leer_configuraciones(char **ip_kernel, char **puerto_kernel, char **level_info_kernel);

#endif  /*CONFIGURACION_H_*/