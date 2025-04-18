#ifndef SOCKET_H_
#define SOCKET_H_

// Incluye las bibliotecas necesarias externas:
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <commons/log.h>
#include <utils/logger.h> 
#include <string.h>
#include<commons/string.h>
#include<commons/config.h>

/**
 * @brief Establece una conexión con un servidor remoto.
 * 
 * Crea un socket cliente y se conecta al servidor especificado por IP y puerto.
 * 
 * @param server_name Nombre lógico del servidor (solo para logs o mensajes).
 * @param ip Dirección IP del servidor al que se desea conectar.
 * @param puerto Puerto del servidor al que se desea conectar.
 * @return Devuelve el descriptor del socket conectado, o -1 en caso de error.
 */
int crear_conexion(const char *server_name, char *ip, char *puerto);

/**
 * @brief Inicia un servidor en la IP y puerto especificados.
 * 
 * Crea un socket, lo enlaza a la IP y puerto dados, y lo deja escuchando conexiones entrantes.
 * 
 * @param name Nombre lógico del servidor (solo para logs o mensajes).
 * @param ip IP en la cual el servidor escuchará conexiones (puede ser "0.0.0.0" para todas).
 * @param puerto Puerto en el cual el servidor escuchará conexiones.
 * @return Devuelve el descriptor del socket del servidor, o -1 en caso de error.
 */
int iniciar_servidor(const char *name, char *ip, char *puerto);

/**
 * @brief Espera la conexión de un cliente al socket servidor.
 * 
 * Bloquea la ejecución hasta que un cliente se conecta al servidor.
 * 
 * @param name Nombre lógico del servidor (solo para logs o mensajes).
 * @param socket_servidor Descriptor del socket del servidor previamente creado.
 * @return Devuelve el descriptor del socket del cliente aceptado, o -1 en caso de error.
 */
int esperar_cliente(const char *name, int socket_servidor);

#endif /* SOCKET_H_ */
