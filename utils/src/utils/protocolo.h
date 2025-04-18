#ifndef PROTOCOL_H
#define PROTOCOL_H

// Incluyo las bibliotecas externas necesarias:
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <utils/logger.h>

typedef enum {
    MENSAJE, 
    HANDSHAKE,
} op_code;

/**
* @brief Recibe un handshake en un socket y responde si es válido.
* 
* Esta función espera recibir un entero a través del socket. Si el valor recibido
* es 1 (valor esperado para un handshake válido), responde con 0 (OK). En cualquier 
* otro caso, responde con -1 (error).
* 
* @param socket Descriptor del socket desde donde se recibe el handshake.
*/
void recibir_handshake(int socket);

/**
* @brief Genera un handshake hacia un servidor, enviando la señal de inicio y esperando confirmación.
* 
* Esta función envía una solicitud de handshake al servidor especificado mediante el socket.
* Si el servidor responde correctamente (con 0), se loguea un mensaje de éxito. En caso de error,
* se loguea y se termina el proceso.
* 
* @param socket Descriptor del socket hacia el servidor.
* @param server_name Nombre lógico del servidor (para mensajes de log).
* @param ip IP del servidor (solo informativo).
* @param puerto Puerto del servidor (solo informativo).
*/
void generar_handshake(int socket, char *server_name, char *ip, char *puerto);
 
#endif /* PROTOCOL_H */
