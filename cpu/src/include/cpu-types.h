#ifndef CPU_TYPES_H_
#define CPU_TYPES_H_

#include <commons/temporal.h>

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
    int pid;
    int pc;
} t_contexto;

typedef enum {
    INSTR_NOOP,
    INSTR_WRITE,
    INSTR_READ,
    INSTR_GOTO,
    INSTR_IO,
    INSTR_INIT_PROC,
    INSTR_DUMP_MEMORY,
    INSTR_EXIT
} t_tipo_instruccion;

typedef struct {
    t_tipo_instruccion tipo;
    char** argv;
    int argc;
} t_instruccion;

typedef struct {
    int nro_pagina;
    int *entradas;
    int desplazamiento;
} t_traduccion;

typedef struct {
    int pagina;
    int marco;
    int timestamp;
    int bit_presencia;
    int bit_modificado;
    int libre;
} t_entrada_tlb;

typedef enum {
    FIFO,
    LRU
} t_algoritmo_tlb;

typedef struct {
    int pagina;
    char* contenido;
    int bit_uso;
    int bit_modificado;
    int libre;
} t_entrada_cache;

typedef enum {
    CLOCK,
    CLOCK_M
} t_algoritmo_cache;


#endif // CPU_TYPES_H_ 