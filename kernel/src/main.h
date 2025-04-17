#ifndef MAIN_H_
#define MAIN_H_

#include "include/utils.h"

void leer_configuraciones(config_kernel);

void iniciar_logger(config_kernel->log_level);

void iniciar_servidor();

void escuchar_peticiones();

#endif  /*MAIN_H_*/