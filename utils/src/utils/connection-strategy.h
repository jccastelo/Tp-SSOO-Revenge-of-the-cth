#ifndef CONNECTION_STRATEGY_H_
#define CONNECTION_STRATEGY_H_

// Incluye las bibliotecas necesarias externas:
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <commons/log.h>
#include <pthread.h>

// Incluye las bibliotecas internas necesarias:
#include "socket.h"
#include "logger.h"
#include "connection-strategy.h"
#include "protocolo.h"
#include "connection.h"

typedef struct {
    int client_socket;
    const char* server_name;
    void (*client_handler)(int client_socket, int operation, const char *server_name);
} client_args_t;

/**
 * @brief Mantiene la conexión con el cliente activa y procesa múltiples operaciones.
 * 
 * Esta estrategia implementa un bucle persistente de atención al cliente. A partir de una conexión ya aceptada,
 * recibe sucesivamente distintas operaciones desde el cliente, ejecutando para cada una la función lógica
 * encargada de su procesamiento. La conexión se mantiene abierta hasta que ocurre un error de recepción
 * o se detecta un cierre por parte del cliente.
 * 
 * Este enfoque es ideal para protocolos de comunicación orientados a sesiones prolongadas o donde se espera
 * un flujo de operaciones continuas por parte del mismo cliente.
 * 
 * @param args Puntero a una estructura `client_args_t` que contiene el socket del cliente, 
 * el nombre del servidor y el handler lógico que procesa cada operación.
 */
void conection_strategy_persistence(void * args);

/**
 * @brief Maneja una única operación del cliente y cierra la conexión.
 * 
 * Esta estrategia se utiliza para modelos de atención puntual, donde se establece una conexión
 * con el cliente, se recibe una única operación, se procesa mediante el handler provisto
 * y luego se cierra la conexión.
 * 
 * Es útil en arquitecturas simples o cuando cada petición del cliente es independiente
 * y no se requiere mantener el estado entre múltiples solicitudes.
 * 
 * @param args Puntero a una estructura `client_args_t` que contiene el socket del cliente, 
 * el nombre del servidor y el handler lógico que procesa la operación.
 */
void conection_strategy_once(void * args);


#endif /* CONNECTION_STRATEGY_H_ */