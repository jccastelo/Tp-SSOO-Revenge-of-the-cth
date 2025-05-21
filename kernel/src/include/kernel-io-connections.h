#ifndef KERNEL_IO_CONNECTIONS_H
#define KERNEL_IO_CONNECTIONS_H

#include "kernel.h"
#include "kernel-types.h"
#include "kernel-syscalls.h"
#include "kernel-free.h"

void gestionar_io(t_buffer *buffer);
t_IO_instancia *buscar_instancia_libre(t_IO *ioBuscada);
t_IO *buscar_io(char *ioNombreBuscado);
int recibir_pid(int io_socket);
void enviar_proceso_io(int io_socket);
void eliminar_instancia(int io_socket);
t_buffer *crear_buffer_io(int milisegundos,int  pid_a_io);
#endif //KERNEL_IO_CONNECTIONS