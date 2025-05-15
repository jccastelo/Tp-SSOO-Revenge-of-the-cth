#ifndef KERNEL_STATE_H_
#define KERNEL_STATE_H_

// Incluimos las bibliotecas necesarias:
#include <stdlib.h>

// Incluimos bibliotecas propias:
#include "kernel-types.h"

// Definimos las variables globales:
extern t_config_kernel *config_kernel;
extern t_kernel_servers *kernel_servers;

extern t_planner *planner;


extern int socket_memoria;

extern t_list *list_procesos;
extern t_list *list_cpus;

#endif // IO_STATE_H_