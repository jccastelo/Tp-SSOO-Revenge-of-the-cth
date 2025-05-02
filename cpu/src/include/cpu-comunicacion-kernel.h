#ifndef CPU_COMUNICACION_KERNEL_H
#define CPU_COMUNICACION_KERNEL_H


#include <utils/logger.h>
#include <utils/protocolo.h>

#include "cpu-types.h"
#include "cpu-state.h"

#include "../include/cpu-comunicacion-kernel.h"

void recibir_contexto_de_kernel();
void deserializar_contexto(void* buffer);
bool recibir_interrupciones();
void enviar_contexto_desalojo();

void syscall_io(char* dispositivo, int tiempo);
void syscall_init_proc(char* archivo, int tamanio);
void syscall_dump_memory();
void syscall_exit();
void agregar_contexto_al_paquete(t_paquete* paquete);


#endif // CPU_CICLO_INSTRUCCION_H_