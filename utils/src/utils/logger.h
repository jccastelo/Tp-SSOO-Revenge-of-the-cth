#ifndef LOGGER_H
#define LOGGER_H

#include <commons/log.h>
#include <commons/string.h>

extern t_log * logger; 

char* concatenar_parametros(char** argv, int argc);

#endif //LOGGER_H