#include "../include/kernel-state.h"

// Definimos la variable global de configuración de Kernel:
t_config_kernel *config_kernel;
t_kernel_servers *kernel_servers;

t_planner *planner;
t_short_term *short_term_planner;
t_long_term *long_term_planner;

int socket_memoria;
t_list *list_procesos;
t_list *list_cpu;