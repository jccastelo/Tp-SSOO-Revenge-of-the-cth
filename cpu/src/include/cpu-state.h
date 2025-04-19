#ifndef CPU_STATE_H_
#define CPU_STATE_H_

// Incluimos las bibliotecas necesarias:
#include <stdlib.h>

// Incluimos bibliotecas propias:
#include "cpu-types.h"

// Definimos las variables globales:
extern t_config_cpu *config_cpu;
extern int socket_dispatch;
extern int socket_interrupt;
extern int socket_memoria;

#endif // CPU_STATE_H_