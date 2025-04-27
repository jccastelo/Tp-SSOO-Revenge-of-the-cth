#ifndef KERNEL_TYPES_H
#define KERNEL_TYPES_H

#include <commons/collections/queue.h>
#include <commons/temporal.h>

//Configuracion del kernel
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

//Servers del kernel
typedef struct {
    int id_server_dispatch;
    int id_server_interrupt;
    int id_server_io;
} t_kernel_servers;


//Planificador
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

//Procesos

typedef struct {
    int new;
    int ready;
    int execute;
    int blocked;
    int blocked_suspended;
    int ready_suspended;
}t_metricas_de_estados;

typedef struct {
    t_temporal  *new;
    t_temporal  *ready;
    t_temporal  *execute;
    t_temporal  *blocked;
    t_temporal  *blocked_suspended;
    t_temporal  *ready_suspended;
}t_metricas_de_tiempo;
typedef struct {
    char *process_name;
    int tamano_proceso;
    int pid;
    int pc;
    t_metricas_de_estados metricas_de_estado;
    t_metricas_de_tiempo metricas_de_tiempo;
}t_pcb;


# endif // KERNEL_TYPES_H