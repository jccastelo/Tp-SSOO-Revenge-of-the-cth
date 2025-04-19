#ifndef  MEMORIA_SERVERS_LISTENING_H
#define  MEMORIA_SERVERS_LISTENING_H

// Incluyo las bibliotecas externas necesarias:
#include <utils/logger.h>
#include <utils/socket.h>
#include <utils/protocolo.h>
#include <utils/connection-strategy.h>
#include <utils/server.h>

// Incluyo las bibliotecas internas necesarias:
#include "memoria-state.h"
#include "memoria-handlers.h"

void memoria_servers_listening();

#endif //MEMORIA_SERVERS_LISTENING_H