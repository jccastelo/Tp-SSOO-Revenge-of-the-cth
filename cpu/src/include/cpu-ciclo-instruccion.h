#ifndef CPU_CICLO_INSTRUCCION_H
#define CPU_CICLO_INSTRUCCION_H

#include <utils/logger.h>
#include <utils/protocolo.h>
#include <commons/string.h>

#include "cpu-types.h"
#include "cpu-state.h"
#include "cpu-comunicacion-memoria.h"
#include "cpu-comunicacion-kernel.h"

#include "../include/cpu-ciclo-instruccion.h"


void realizar_ciclo_de_instruccion();
char* fetch_instruction();
t_instruccion* decode(char* instruccion);
void excecute(t_instruccion* instruccion);
bool es_syscall_que_frena(t_tipo_instruccion tipo);
bool check_interrupt();
t_tipo_instruccion mapeo_string_tipo(char* tipo_instruccion);

#endif // CPU_COMUNICACION_MEMORIA_H_