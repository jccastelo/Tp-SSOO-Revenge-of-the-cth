#ifndef KERNEL_TYPES_H
#define KERNEL_TYPES_H

#include <commons/collections/queue.h>
typedef struct {
    char *IP_MEMORIA;
    int PUERTO_MEMORIA;
    int PUERTO_ESCUCHA_DISPATCH;
    int PUERTO_ESCUCHA_INTERRUPT;
    int PUERTO_ESCUCHA_IO;
    char *ALGORITMO_CORTO_PLAZO;
    char *ALGORITMO_INGRESO_A_READY;
    float ALFA;
    int TIEMPO_SUSPENSION;
    int TIEMPO_DEADLOCK;
    char *LOG_LEVEL;
} t_config_kernel;

typedef struct {
    int id_server_dispatch;
    int id_server_interrupt;
    int id_server_io;
} t_kernel_servers;


typedef struct {

    t_queue* queue_READY;
    char *algoritmoPlanificador_short;

} t_short_term;

typedef struct {

    t_queue* queue_NEW;
    t_queue* queue_BLOCKED;
    char *algoritmoPlanificador_long;
} t_long_term;
typedef struct {

    t_short_term *short_term;
    t_long_term *long_term;
} t_planner;





# endif // KERNEL_TYPES_H