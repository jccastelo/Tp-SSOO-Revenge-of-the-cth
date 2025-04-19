#ifndef CPU_CONNECT_H_
#define CPU_CONNECT_H_

// Incluimos las bibliotecas externas necesarias:
#include <commons/string.h>
#include <utils/connection.h>
#include <utils/protocolo.h>

// Incluimos las bibliotecas internas necesarias:
#include "cpu-state.h"

void cpu_connect();
void set_socket_kernel_dispatch(int socket);
void set_socket_kernel_interrupt(int socket);

#endif // CPU_CONNECT_H_