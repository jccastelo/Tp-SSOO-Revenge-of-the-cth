#ifndef MEMORIA_SERVERS_INIT_H
#define MEMORIA_SERVERS_INIT_H


// Incluye las bibliotecas necesarias externas:
#include <utils/logger.h>
#include <utils/socket.h>
#include <utils/connection.h>
#include <commons/string.h>

// Incluyo las bibliotecas necesarias internas:
#include "memoria-state.h"

void memoria_servers_init(void);
void set_escucha_id (int id_server);
void set_escucha_id (int id_server);

#endif //MEMORIA_SERVERS_INIT_H