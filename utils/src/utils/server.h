#ifndef SERVER_H_
#define SERVER_H_

// Incluye las bibliotecas internas externas:
#include "socket.h"
#include "logger.h"
#include "connection-strategy.h"

/**
 * @brief Atiende múltiples clientes en un bucle, usando una estrategia de manejo definida.
 * 
 * Acepta conexiones entrantes en el socket del servidor y delega el manejo de cada cliente
 * a la función de estrategia provista. Esta función permite utilizar distintos enfoques 
 * para manejar las conexiones, como hilos por cliente o manejo secuencial.
 * 
 * @param server_socket Descriptor del socket del servidor.
 * @param server_name Nombre lógico del servidor (usado para logs o mensajes).
 * @param connection_strategy Función que define cómo manejar cada cliente (ej: con hilos).
 * @param client_handler Función que contiene la lógica específica para atender un cliente.
 */
void server_clients(int server_socket, const char* server_name, void (*connection_strategy)(int, const char* server_name, void (*client_handler)(void*)), void (*client_handler)(void*));

/**
 * @brief Maneja una conexión de cliente en un nuevo hilo.
 * 
 * Crea un nuevo hilo para atender al cliente usando el handler proporcionado. 
 * Es útil para permitir múltiples conexiones concurrentes.
 * 
 * @param client_socket Descriptor del socket del cliente.
 * @param server_name Nombre lógico del servidor (usado para logs o contexto).
 * @param client_handler Función encargada de atender al cliente dentro del hilo.
 */
void serve_client_with_thread(int client_socket, const char* server_name, void (*client_handler)(void*));

/**
 * @brief Maneja una conexión de cliente en el hilo actual.
 * 
 * Atiende al cliente directamente sin crear un nuevo hilo, utilizando el handler dado.
 * Es útil para servidores secuenciales o cuando no se requiere concurrencia.
 * 
 * @param client_socket Descriptor del socket del cliente.
 * @param server_name Nombre lógico del servidor (usado para logs o contexto).
 * @param client_handler Función encargada de atender al cliente.
 */
void serve_client_with_unique_thread(int client_socket, const char* server_name, void (*client_handler)(void*));
