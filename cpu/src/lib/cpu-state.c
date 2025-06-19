#include "include/cpu-state.h"

// Definimos la variable global de configuración de CPU:
t_config_cpu *config_cpu;
t_contexto* contexto;
int socket_dispatch;
int socket_interrupt;
int socket_memoria;

// otras variables globales
char *id_cpu;
int TAM_PAGINA;
int ENTRADAS_POR_TABLA;
int CANTIDAD_NIVELES;