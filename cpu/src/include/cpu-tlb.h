#ifndef CPU_TLB_H
#define CPU_TLB_H

//Librerias externas
#include <utils/logger.h>
#include <commons/string.h>

//Librerias internas
#include "cpu-state.h"
#include "cpu-config.h"
#include "cpu-connect.h"

#include "include/cpu-tlb.h"

int buscar_frame_tlb(int pagina);

void actualizar_timestamp(int posicion);

void agregar_a_tlb(int pagina, int marco);

void agregar_en_entrada(int entrada, int pagina, int marco);

int elegir_victima_tlb();

t_algoritmo_tlb get_algoritmo(char* algoritmo_str);

#endif //CPU_TLB_H