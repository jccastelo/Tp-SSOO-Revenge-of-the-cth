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

/**
 * @brief Convierte un buffer crudo recibido de Memoria en un string.
 *
 * Crea una copia del contenido del buffer como cadena de caracteres.
 *
 * @param buffer Buffer recibido desde Memoria.
 * @return String con la instrucción.
 */
char* deserializar_instruccion(void* buffer);

#endif // CPU_COMUNICACION_MEMORIA_H_