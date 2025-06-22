#include "include/cpu-tlb.h"

int buscar_frame_tlb(int pagina) {
    for (int i=0; i < config_cpu->ENTRADAS_TLB; i++) {
        if (tlb[i].libre != 1 && tlb[i].pagina == pagina) {
            actualizar_timestamp(i);
            return tlb[i].marco; // TODO LOG TLBHIT ACA?
        }
    }
    return -1; //MISMO LOG PERO CON MISS
}

void actualizar_timestamp(int posicion) {
    timestamp_global++; 
    tlb[posicion].timestamp = timestamp_global;
}

void agregar_a_tlb(int pagina, int marco) {
    int entrada_libre = -1;
    for (int i = 0; i < config_cpu->ENTRADAS_TLB; i++) {
        if (tlb[i].libre) {
            entrada_libre = i;
            break;
        }
    }

    if (entrada_libre == -1) {
        entrada_libre = elegir_victima_tlb();
    }

    agregar_en_entrada(entrada_libre, pagina, marco);
}

void agregar_en_entrada(int entrada, int pagina, int marco) {
    tlb[entrada].pagina = pagina;
    tlb[entrada].marco = marco;
    tlb[entrada].bit_modificado = 0;
    tlb[entrada].bit_presencia = 1;
    tlb[entrada].libre = 0;
    actualizar_timestamp(entrada);
}

int elegir_victima_tlb() {
    int victima = -1;

    switch (get_algoritmo(config_cpu->REEMPLAZO_TLB))
    {
    case FIFO:
        victima = puntero_fifo;
        puntero_fifo = (puntero_fifo + 1) % config_cpu->ENTRADAS_TLB;
        break;
    case LRU:
        int menor = tlb[0].timestamp;
        victima = 0;
        for (int i=1; i < config_cpu->ENTRADAS_TLB; i++) {
            if (tlb[i].timestamp < menor) {
                menor = tlb[i].timestamp;
                victima = i;
            }
        }
    default:
        //TODO ERROR
        break;
    }

    return victima;
}

t_algoritmo_tlb get_algoritmo(char* algoritmo_str) {
    if (string_equals_ignore_case(algoritmo_str, "FIFO")) return FIFO;
    else if (string_equals_ignore_case(algoritmo_str, "LRU")) return LRU;
    else return -1;
}