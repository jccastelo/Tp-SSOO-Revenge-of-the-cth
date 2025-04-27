#ifndef PROTOCOL_H
#define PROTOCOL_H

// Incluyo las bibliotecas externas necesarias:
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <utils/logger.h>
#include <string.h>

typedef enum {
    MENSAJE, 
    HANDSHAKE,
	CONTEXT_PROCESS,
	CPU_ID,
	INIT_PROC,
	DUMP_MEMORY,
	IO,
} op_code;

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

/**
 * @brief Crea un nuevo paquete con la operación indicada.
 * 
 * Esta función reserva memoria para un paquete y lo inicializa con el código de operación especificado.
 * 
 * @param operacion Código de operación a incluir en el paquete.
 * @return t_paquete* Puntero al paquete creado.
 */
t_paquete* crear_paquete(op_code operacion);

/**
 * @brief Inicializa el buffer del paquete.
 * 
 * Esta función reserva e inicializa el espacio necesario para el buffer del paquete, 
 * que luego podrá ser utilizado para agregar datos.
 * 
 * @param paquete Puntero al paquete cuyo buffer se quiere crear.
 */
void crear_buffer(t_paquete* paquete);

/**
 * @brief Agrega un valor serializado al buffer del paquete.
 * 
 * Esta función copia los datos apuntados por `valor` al buffer del paquete, expandiendo
 * el buffer si es necesario.
 * 
 * @param paquete Puntero al paquete al que se desea agregar el valor.
 * @param valor Puntero a los datos que se desean agregar.
 * @param tamanio Tamaño en bytes de los datos a agregar.
 */
void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio);

/**
 * @brief Envía un paquete a través de un socket.
 * 
 * Esta función serializa y envía todo el contenido del paquete (incluyendo la operación, 
 * el tamaño del buffer y los datos) al socket especificado.
 * 
 * @param paquete Puntero al paquete que se desea enviar.
 * @param socket_cliente Descriptor del socket por el cual se enviará el paquete.
 */
void enviar_paquete(t_paquete* paquete, int socket_cliente);

/**
 * @brief Libera la memoria asociada a un paquete.
 * 
 * Esta función libera tanto el buffer como la estructura del paquete.
 * 
 * @param paquete Puntero al paquete que se desea eliminar.
 */
void eliminar_paquete(t_paquete* paquete);

/**
 * @brief Recibe un buffer desde un socket.
 * 
 * Esta función recibe un entero que indica el tamaño del buffer, seguido del contenido del mismo.
 * 
 * @param size Puntero donde se almacenará el tamaño del buffer recibido.
 * @param socket_cliente Descriptor del socket desde el cual se recibe.
 * @return void* Puntero al buffer recibido (debe ser liberado por el receptor).
 */
void* recibir_buffer(int* size, int socket_cliente);

/**
 * @brief Recibe el código de operación enviado a través de un socket.
 * 
 * Esta función recibe un entero desde el socket que representa el `op_code` enviado por el emisor.
 * 
 * @param socket_cliente Descriptor del socket desde el cual se recibe el código de operación.
 * @return int Código de operación recibido.
 */
int recibir_operacion(int socket_cliente);

/**
 * @brief Serializa un paquete para su envío por red.
 * 
 * Esta función convierte la estructura del paquete (código de operación, tamaño del buffer y datos) 
 * en un bloque continuo de memoria listo para ser enviado.
 * 
 * @param paquete Puntero al paquete a serializar.
 * @param bytes Cantidad total de bytes que ocupará el paquete serializado (incluye encabezado y buffer).
 * @return void* Puntero al bloque de memoria serializado (debe ser liberado por el emisor).
 */
void* serializar_paquete(t_paquete* paquete, int bytes);

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
*/
void generar_handshake(int socket, char *server_name);
 
#endif /* PROTOCOL_H */
