#ifndef KERNEL_H
#define KERNEL_H

// Incluye las bibliotecas necesarias externas:
#include <utils/logger.h>
#include <signal.h>

// Incluyo las bibliotecas necesarias internas:
#include "kernel-config.h"
#include "kernel-state.h"
#include "kernel-init-connection.h"
#include "kernel-servers.h"
#include "kernel-memory-connection.h"
#include "kernel-planner.h"
#include "kernel-free.h"

extern pthread_mutex_t mutex_control_kernel;
#endif // KERNEL_Hs