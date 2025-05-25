#include "../include/kernel-state.h"

// Definimos la variable global de configuración de Kernel:
t_config_kernel *config_kernel;
t_kernel_servers *kernel_servers;

t_planner *planner;

int socket_memoria;

t_monitor *list_procesos;
t_monitor *list_cpus;
t_monitor *list_ios;