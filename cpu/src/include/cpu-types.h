#ifndef CPU_TYPES_H_
#define CPU_TYPES_H_

typedef struct {
    char *IP_MEMORIA;
    char *IP_KERNEL;
    int PUERTO_MEMORIA;
    int PUERTO_KERNEL_DISPATCH;
    int PUERTO_KERNEL_INTERRUPT;
    char *REEMPLAZO_TLB;
    int  ENTRADAS_CACHE;
    int ENTRADAS_TLB;
    char * REEMPLAZO_CACHE;
    int RETARDO_CACHE;
    char *LOG_LEVEL;
} t_config_cpu;

typedef struct {
    int socket_dispatch;
    int socket_interrupt;
    int socket_memoria;
} t_cpu_servers;


#endif // CPU_TYPES_H_ 