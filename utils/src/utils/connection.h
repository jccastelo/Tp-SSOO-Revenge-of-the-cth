#ifndef CONNECTION_H_
#define CONNECTION_H_

// Incluye las bibliotecas externas:
#include <errno.h>
#include <string.h>
#include <unistd.h>

// Incluye las bibliotecas externas:
#include "logger.h"
#include "socket.h"

/**
 * @brief Inicializa un servidor y ejecuta una función callback con su ID.
 * 
 * Esta función crea un servidor utilizando los parámetros proporcionados (nombre, IP y puerto),
 * y, si la creación es exitosa, ejecuta la función callback pasándole el ID del servidor.
 * En caso de fallo, se loggea el error y se finaliza la ejecución.
 * 
 * @param server_name Nombre lógico del servidor (para logging).
 * @param ip IP en la cual el servidor escuchará conexiones.
 * @param puerto Puerto en el cual el servidor escuchará conexiones.
 * @param callback Función que se ejecutará tras la creación exitosa del servidor. 
 *                 Recibe como argumento el ID del servidor.
 */
void setup_server(char *server_name, char *ip, char *puerto, void (*callback)(int id_server));

/**
 * @brief Establece una conexión con un servidor y ejecuta una función callback con el socket del cliente.
 * 
 * Esta función intenta crear una conexión con el servidor especificado por su nombre, IP y puerto.
 * Si la conexión es exitosa, se ejecuta la función callback pasándole el socket del cliente.
 * En caso de error, se loggea el error y se finaliza la ejecución.
 * 
 * @param server_name Nombre lógico del servidor (para logging).
 * @param ip Dirección IP del servidor al que se desea conectar.
 * @param puerto Puerto del servidor al que se desea conectar.
 * @param callback Función que se ejecutará tras la conexión exitosa. 
 *                 Recibe como argumento el socket del cliente.
 */
void setup_connection_with_server(char *server_name, char *ip, char *puerto, void (*callback)(int socket_client));

/**
 * @brief Valida el estado de conexión con un cliente.
 * 
 * Esta función verifica si un cliente sigue conectado al servidor sin bloquear la ejecución.
 * Utiliza `recv` con las banderas `MSG_PEEK` y `MSG_DONTWAIT` para inspeccionar la conexión
 * de manera no bloqueante. 
 * 
 * Si detecta que el cliente se ha desconectado, registra un error en el log
 * y actualiza la variable `execute_server` para detener la ejecución del servidor.
 * 
 * @param execute_server Puntero a una variable de control que indica si el servidor debe seguir ejecutándose.
 * @param client_socket Socket del cliente cuya conexión se desea validar.
 */
 void connection_validate(int *execute_server, int client_socket);


# endif /* CONNECTION_H_ */