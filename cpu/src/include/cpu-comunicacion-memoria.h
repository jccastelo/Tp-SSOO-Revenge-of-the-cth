#ifndef CPU_COMUNICACION_MEMORIA_H
#define CPU_COMUNICACION_MEMORIA_H


#include <utils/logger.h>
#include <utils/protocolo.h>

#include "cpu-types.h"
#include "cpu-state.h"

/**
 * @brief Solicita a Memoria la instrucción correspondiente al Program Counter actual.
 *
 * Construye un paquete con el `pc` y `pid` del proceso y lo envía a Memoria.
 */
void conseguir_siguiente_instruccion();

/**
 * @brief Espera y recibe una instrucción desde Memoria.
 *
 * Recibe un buffer con la instrucción solicitada previamente y la convierte a string.
 *
 * @return La instrucción como string terminada en NULL (debe liberarse luego).
 */
char* devolver_instruccion_a_ejecutar();

int pedir_marco_a_memoria(t_traduccion *traduccion);

char* conseguir_contenido_frame(int frame);

void escribir_pagina_en_memoria(int frame, char* contenido);

char* leer_en_memoria_desde(int dir_fisica, int tamanio);

void escribir_en_memoria_desde(int dir_fisica, char* contenido);

#endif // CPU_COMUNICACION_MEMORIA_H_