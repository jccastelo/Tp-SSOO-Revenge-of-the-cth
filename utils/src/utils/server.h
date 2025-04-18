#ifndef SERVER_H_
#define SERVER_H_

// Incluye las bibliotecas necesarias externas:
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h>

// Incluye las bibliotecas internas necesarias:
#include "socket.h"
#include "logger.h"
#include "connection-strategy.h"

/**
 * @brief Atiende múltiples clientes en un bucle, usando una estrategia de manejo definida.
 * 
 * Acepta conexiones entrantes en el socket del servidor y delega el manejo de cada cliente
 * a una función de estrategia configurable. Esto permite aplicar distintos enfoques para
 * manejar clientes, como atención secuencial o concurrente mediante hilos.
 * 
 * @param server_socket Descriptor del socket del servidor.
 * @param server_name Nombre lógico del servidor (usado para logs o mensajes).
 * @param connection_strategy Función que define cómo manejar cada cliente (ej: con hilos).
 * @param server_strategy Estrategia que define cómo gestionar la conexión del servidor (manejo concurrente o secuencial).
 * @param client_handler Función encargada de atender a cada cliente de forma específica.
 */
void server_clients(
    int server_socket,
    const char* server_name,
    void (*connection_strategy)(void *args),
    void (*server_strategy)(int, const char* server_name, void (*connection_strategy)(void *args), void (*client_handler)(int, int, char *)),
    void (*client_handler)(int client_socket, int operation, char *server_name)
);

/**
 * @brief Aplica una estrategia multihilo para manejar conexiones entrantes.
 * 
 * Por cada cliente conectado, se crea un nuevo hilo que ejecuta la estrategia
 * de atención configurada. Ideal para servidores que requieren manejar múltiples
 * clientes en paralelo.
 * 
 * @param client_socket Descriptor del socket del cliente.
 * @param server_name Nombre lógico del servidor (usado para logs o contexto).
 * @param connection_strategy Estrategia específica de conexión a ejecutar dentro del hilo.
 * @param client_handler Función encargada de atender al cliente.
 */
void server_strategy_thread(
    int client_socket,
    const char* server_name,
    void (*connection_strategy)(void *args),
    void (*client_handler)(int client_socket, int operation, char *server_name)
);

/**
 * @brief Aplica una estrategia multihilo para manejar conexiones entrantes.
 * 
 * Por cada cliente conectado, se crea un nuevo hilo que ejecuta la estrategia
 * de atención configurada. Ideal para servidores que requieren manejar múltiples
 * clientes en paralelo.
 * 
 * @param client_socket Descriptor del socket del cliente.
 * @param server_name Nombre lógico del servidor (usado para logs o contexto).
 * @param connection_strategy Estrategia específica de conexión a ejecutar dentro del hilo.
 * @param client_handler Función encargada de atender al cliente.
 */
void server_strategy_thread(
    int client_socket,
    const char* server_name,
    void (*connection_strategy)(void *args),
    void (*client_handler)(int client_socket, int operation, char *server_name)
);

/**
 * @brief Maneja conexiones entrantes en el hilo principal de forma secuencial.
 * 
 * Atiende al cliente directamente, sin crear un nuevo hilo, ejecutando la estrategia
 * de atención en el mismo contexto de ejecución. Útil para implementaciones simples
 * o cuando no se requiere concurrencia.
 * 
 * @param client_socket Descriptor del socket del cliente.
 * @param server_name Nombre lógico del servidor (usado para logs o contexto).
 * @param connection_strategy Estrategia de atención a ejecutar para el cliente.
 * @param client_handler Función encargada de atender al cliente.
 */
void server_strategy_unique_thread(
    int client_socket,
    const char* server_name,
    void (*connection_strategy)(void *args),
    void (*client_handler)(int client_socket, int operation, char *server_name)
);

/**
 * @brief Inicia un servidor secuencial que atiende un cliente a la vez en el hilo principal.
 * 
 * Esta función configura el servidor para manejar las conexiones entrantes de forma secuencial, es decir,
 * cada vez que un cliente se conecta, el servidor lo atiende en el mismo hilo, uno a la vez. 
 * Este enfoque es ideal para entornos donde la concurrencia no es necesaria, o para mantener una lógica 
 * sencilla y sin complicaciones relacionadas con el manejo de múltiples hilos.
 * 
 * @param server_socket Descriptor del socket del servidor, que escucha las conexiones entrantes.
 * @param server_name Nombre lógico del servidor, usado para mensajes de registro o información de contexto.
 * @param connection_strategy Estrategia de conexión, una función que define cómo manejar la conexión del cliente (sin concurrencia).
 * @param client_handler Función que maneja las operaciones específicas que se deben realizar para atender al cliente.
 */
void server_with_thread(
    int server_socket, 
    const char* server_name, 
    void (*connection_strategy)(void *args),
    void (*client_handler)(int client_socket, int operation, char *server_name)
);

/**
 * @brief Inicia un servidor concurrente que maneja múltiples clientes utilizando hilos.
 * 
 * En lugar de atender a los clientes de manera secuencial, esta función crea un nuevo hilo para cada cliente
 * que se conecta, permitiendo que varios clientes sean atendidos simultáneamente. Este enfoque es útil en situaciones
 * donde la concurrencia es necesaria para mejorar el rendimiento o cuando se deben manejar múltiples conexiones
 * al mismo tiempo, como en servidores de alta demanda.
 * 
 * @param server_socket Descriptor del socket del servidor, que escucha las conexiones entrantes.
 * @param server_name Nombre lógico del servidor, utilizado para logs o mensajes informativos.
 * @param connection_strategy Estrategia de conexión, que define cómo manejar la conexión del cliente utilizando hilos.
 * @param client_handler Función que contiene la lógica para atender al cliente, ejecutada en un hilo separado para cada cliente.
 */
void server_with_unique_thread(
    int server_socket, 
    const char* server_name, 
    void (*connection_strategy)(void *args),
    void (*client_handler)(int client_socket, int operation, char *server_name)
);

#endif /* SERVER_H_ */