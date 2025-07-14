#ifndef KERNEL_IO_CONNECTIONS_H
#define KERNEL_IO_CONNECTIONS_H

#include "kernel.h"
#include "kernel-types.h"
#include "kernel-syscalls.h"
#include "kernel-free.h"

void gestionar_io(t_buffer *buffer);
t_IO_instancia *buscar_instancia_libre(t_IO *ioBuscada);
t_IO *buscar_io(char *ioNombreBuscado);
int recibir_pid(t_buffer *buffer, int io_socket);
void enviar_proceso_io(int io_socket);
void eliminar_instancia(int io_socket);
t_buffer *crear_buffer_io(int milisegundos,int  pid_a_io);
void recibir_io(t_buffer *buffer, int io_socket);
void desencolarProcesosEsperando(t_IO *ios_estructura);
void actualizarIO_a_libre(int pid_desbloqueo);

//extern pthread_mutex_t mutex_io;

#endif //KERNEL_IO_CONNECTIONS