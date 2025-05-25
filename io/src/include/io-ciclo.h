#ifndef IO_CICLO_H_
#define IO_CICLO_H_

// Incluye las bibliotecas necesarias externas:
#include <utils/connection.h>
#include <utils/protocolo.h>
#include <commons/string.h>

// Incluye las bibliotecas internas necesarias:
#include "io-state.h"
#include "io-types.h"

void ciclo_de_io();
t_proceso* recibir_proceso();
void notificar_liberacion(t_proceso* proceso);
#endif /* IO_CICLO_H_ */