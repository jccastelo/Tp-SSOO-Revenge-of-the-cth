#ifndef KERNEL_TYPES_H
#define KERNEL_TYPES_H

#include <commons/collections/queue.h>
#include <commons/temporal.h>
#include <pthread.h>

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

//Procesos

typedef struct {
    int NEW; 
    int READY;
    int EXECUTE;
    int BLOCKED;
    int BLOCKED_SUSPENDED;
    int READY_SUSPENDED;
    int EXIT;
}t_metricas_de_estados;

typedef struct {
    t_temporal  *NEW;
    t_temporal  *READY;
    t_temporal  *BLOCKED;
    t_temporal  *EXECUTE;
    t_temporal  *BLOCKED_SUSPENDED;
    t_temporal  *READY_SUSPENDED;
    t_temporal  *metrica_actual;
}t_metricas_de_tiempo;
typedef struct {
    char *process_name;
    int tamanio_proceso;
    int pid;
    int pc;
    int estado_actual;
    t_metricas_de_estados *metricas_de_estado;
    t_metricas_de_tiempo *metricas_de_tiempo;
}t_pcb;


//Planificador

typedef struct {
    t_queue* queue_ESTADO;
    pthread_mutex_t* mutex;
}t_mutex_queue;

typedef struct {

    t_mutex_queue* queue_READY;
    void (*algoritmo_planificador)(t_pcb* process, t_queue* estado);
} t_short_term;

typedef struct {

    t_mutex_queue* queue_NEW;
    t_mutex_queue* queue_BLOCKED;
    void (*algoritmo_planificador)(t_pcb* process, t_queue* estado);
} t_long_term;
typedef struct {

    t_short_term *short_term;
    t_long_term *long_term;
} t_planner;

typedef enum{
    NEW,
    READY,
    EXECUTE,
    BLOCKED,
    BLOCKED_SUSPENDED,
    READY_SUSPENDED,
    EXIT,
}t_estados;

typedef enum {
    FIFO,
    PMCP,
    SJFsD,
    SJFcD,
}t_planner_algorithm;

# endif // KERNEL_TYPES_H