#ifndef CONEXION_KERNEL_H_
#define CONEXION_KERNEL_H_

#include "utils.h"
#include <utils/protocolo.h>

int handshake(int);
void escucha_peticiones_kernel();

#endif  /*CONEXION_KERNEL_H_*/