#ifndef MMU_H
#define MMU_H

//Librerias externas


//Librerias internas
#include "cpu-state.h"
#include "cpu-types.h"

t_traduccion* traducir_direccion_logica(int dir_logica);

void get_data_memoria();

int obtener_nro_pagina(int dir_logica);

void obtener_entradas_por_nivel(int *entradas, int nro_pagina);

int obtener_desplazamiento(int dir_logica);

int potencia(int base, int exponente);

#endif //MMU_H