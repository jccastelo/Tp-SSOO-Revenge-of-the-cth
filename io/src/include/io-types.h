#ifndef IO_TYPES_H
#define IO_TYPES_H

typedef struct {
    char *IP_KERNEL;
    int PUERTO_KERNEL;
    char *LOG_LEVEL;
} t_config_io;

typedef struct {
    int pid;
    int milisegundos;
} t_proceso;

# endif // IO_TYPES_H