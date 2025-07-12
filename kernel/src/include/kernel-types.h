#ifndef KERNEL_TYPES_H
#define KERNEL_TYPES_H

#include <commons/collections/list.h>
#include <commons/collections/queue.h>
#include <commons/temporal.h>
#include <pthread.h>

//Configuracion del kernel
typedef struct {
    char *IP_SERVER;
    char *IP_MEMORIA;
    int PUERTO_MEMORIA;
    int PUERTO_ESCUCHA_DISPATCH;
    int PUERTO_ESCUCHA_INTERRUPT;
    int PUERTO_ESCUCHA_IO;
    char *ALGORITMO_CORTO_PLAZO;
    char *ALGORITMO_INGRESO_A_READY;
    float ALFA;
    int ESTIMACION_INICIAL;
    int TIEMPO_SUSPENSION;
    char *LOG_LEVEL;
} t_config_kernel;

// Servers del kernel
typedef struct {
    int id_server_dispatch;
    int id_server_interrupt;
    int id_server_io;
} t_kernel_servers;

// Colas de estados

typedef struct {
    t_list* cola;
    pthread_mutex_t mutex;
}t_monitor;

// Procesos

typedef struct {
    int new; 
    int ready;
    int execute;
    int blocked;
    int blocked_suspended;
    int ready_suspended;
    int exit;
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

typedef struct{
    int64_t rafagaEstimada;
    t_temporal *rafagaReal;
    int64_t ultimaEstimacion;
    int64_t rafagaTotalReal;
}t_SJF;

typedef struct {
    char *archivo;
    int tamanio_proceso;
    int pid;
    int pc;
    bool hilo_activo;
    t_monitor* queue_ESTADO_ACTUAL;
    t_metricas_de_estados *metricas_de_estado;
    t_metricas_de_tiempo *metricas_de_tiempo;
    t_SJF *estimaciones_SJF;
    pthread_t hilo_block;
    pthread_mutex_t mutex_estado;
    
}t_pcb;


// Planificador

typedef struct {
    t_monitor* queue_READY;
    void (*algoritmo_planificador)(t_pcb* process, t_list* estado);
    void (*algoritmo_desalojo)(t_pcb* process);
} t_short_term;

typedef struct {
    t_monitor* queue_BLOCKED_SUSPENDED;
    t_monitor* queue_READY_SUSPENDED;
    void (*algoritmo_planificador)(t_pcb* process, t_list* estado);
} t_medium_term;

typedef struct {

    t_monitor* queue_NEW;
    t_monitor* queue_BLOCKED;
    t_monitor* queue_EXIT;
    void (*algoritmo_planificador)(t_pcb* process, t_list* estado);
} t_long_term;
typedef struct {
    t_monitor *queue_EXECUTE;
    t_short_term *short_term;
    t_medium_term *medium_term; 
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


// CPU Estructuras
typedef struct{
    int id;
    int estado;
    int pid;
    int socket_dispatch;
    int socket_interrupt;
    pthread_mutex_t mutex;
}t_cpu;

typedef enum{
    DISPONIBLE,
    EJECUTANDO,
}cpu_estado;

// I/O Estructuras

typedef struct{
    char *nombre;
    t_monitor *instancias_IO;
    t_monitor *procesos_esperando; 
}t_IO;

typedef struct{
    int socket;
    int proceso;
}t_IO_instancia;



# endif // KERNEL_TYPES_H