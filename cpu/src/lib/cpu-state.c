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
t_entrada_tlb *tlb;
int timestamp_global;
int puntero_fifo = 0;

void inicializar_tlb() {
    tlb = malloc(sizeof(t_entrada_tlb) * config_cpu->ENTRADAS_TLB);
    for (int i = 0; i < config_cpu->ENTRADAS_TLB; i++) {
        tlb[i].libre = 1;
        tlb[i].bit_presencia = 0;
        tlb[i].bit_modificado = 0;
    };
}