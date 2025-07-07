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
#include <commons/string.h>
#include <commons/collections/list.h>

typedef enum {
    MENSAJE, 
    HANDSHAKE,
	CONTEXT_PROCESS,
	INIT_PROC,
	CPU_ID,
	DUMP_MEMORY,
	IO,
	EXIT_SYS,
	SUSPENDER,
	DESUSPENDER,
	IDENTIFICAR_IO,
	DESBLOQUEO_IO,
	FIN_CONEXION_DE_IO,
	INTERRUPT,
	CONTEXTO_DESALOJO,
	GET_INSTRUCTION,
	RETURN_INSTRUCCION,
	MEMORY_CONFIG,
	GET_FRAME,
	RETURN_FRAME,
	GET_CONTENT,
	RETURN_CONTENT,
	WRITE_MEM,
	READ_MEM
} op_code;

typedef enum {
	OK = 2,
	ERROR = -1,
	HANDSHAKE_OK = 1,
	HANDSHAKE_ERROR = -2
} response_code;

typedef struct {
	int size;
	void* stream;
} t_buffer;

typedef struct {
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

/**
 * Tipo de función para parsear datos desde un buffer.
 * 
 * Esta función recibe:
 * - `buffer`: Puntero al buffer de datos a parsear.
 * - `offset`: Puntero al desplazamiento actual dentro del buffer; se actualiza
 *             conforme se avanzan bytes al parsear.
 * - `dest`: Puntero al destino donde se almacenará el dato parseado.
 * 
 * Se utiliza como callback para interpretar distintos tipos de datos desde un
 * buffer de forma flexible y genérica.
 */
typedef void (*parse_func_t)(void *buffer, int *offset, void *dest);

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
 * @brief Agrega una cadena de caracteres a un paquete, incluyendo primero su longitud.
 *
 * Esta función toma una cadena y su tamaño en bytes, y la agrega al buffer del paquete.
 * Primero se agrega la longitud de la cadena (como un entero), y luego el contenido de la cadena.
 *
 * @param paquete Puntero al paquete al que se le desea agregar la cadena.
 * @param cadena Puntero a la cadena de caracteres que se desea agregar.
 * @param tamanio Tamaño en bytes de la cadena a agregar (puede ser menor o igual a string_length(cadena)).
 *
 * @note La función utiliza realloc para expandir dinámicamente el buffer del paquete.
 *       Se recomienda verificar que el paquete esté correctamente inicializado antes de llamar a esta función.
 */
void agregar_a_paquete_string(t_paquete* paquete, char* cadena, int tamanio);

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

/**
 * @brief Parsea un entero desde un buffer en una posición determinada.
 *
 * Esta función copia un valor entero (`int`) desde el buffer en la posición indicada
 * por el desplazamiento, lo almacena en la variable de destino y actualiza el desplazamiento.
 *
 * @param buffer Puntero al buffer que contiene los datos.
 * @param desplazamiento Puntero al desplazamiento actual dentro del buffer (en bytes). Se actualiza tras la lectura.
 * @param destino Puntero a la variable donde se almacenará el valor entero parseado.
 */
void parsear_int(void* buffer, int* desplazamiento, void* destino);

/**
 * @brief Parsea una cadena de caracteres desde un buffer.
 *
 * Esta función lee un entero inicial que representa la longitud de la cadena,
 * luego copia esa cantidad de caracteres desde el buffer, reserva memoria para el string
 * e incluye el caracter nulo al final (`'\0'`). También actualiza el desplazamiento.
 *
 * @param buffer Puntero al buffer que contiene los datos.
 * @param desplazamiento Puntero al desplazamiento actual dentro del buffer (en bytes). Se actualiza tras la lectura.
 * @param destino Puntero donde se almacenará el puntero a la cadena parseada (se reserva memoria internamente).
 */
void parsear_string(void* buffer, int* desplazamiento, void* destino);

/**
 * @brief Parsea una cantidad fija de enteros desde un buffer y los almacena en una lista.
 *
 * Esta función utiliza un desplazamiento (`offset`) dentro de un buffer binario para
 * extraer `size` enteros consecutivos. Cada entero es leído mediante la función `parsear_int`,
 * almacenado dinámicamente y agregado a una lista.
 *
 * @param buffer Puntero al buffer que contiene los datos.
 * @param desplazamiento Puntero al desplazamiento actual dentro del buffer (en bytes). Se actualiza durante el parseo.
 * @param size Cantidad de enteros a parsear desde el buffer.
 * @return Una lista (`t_list*`) que contiene punteros a enteros parseados.
 */
t_list *parsear_ints(void *buffer, int *desplazamiento, int size);

/**
 * @brief Parsea múltiples valores desde un buffer y los agrega a una lista.
 *
 * Esta función permite parsear elementos genéricos (por ejemplo, enteros o cadenas)
 * desde un buffer binario, utilizando una función de parseo provista. Cada elemento
 * es almacenado dinámicamente y agregado a la lista destino. El proceso se repite
 * hasta que el desplazamiento alcance el tamaño total del buffer.
 *
 * @param list Lista en la que se agregarán los elementos parseados.
 * @param parse_func Función que se usará para parsear cada elemento desde el buffer.
 * @param buffer Puntero al buffer de entrada.
 * @param offset Puntero al desplazamiento actual dentro del buffer (en bytes). Se actualiza durante el parseo.
 * @param buffer_size Tamaño total del buffer (en bytes).
 * @param elem_size Tamaño en bytes de cada elemento a parsear (usado para `malloc`).
 */
void parse_values_and_add_to_list(t_list *list, parse_func_t parse_func, void *buffer, int *offset, int buffer_size, size_t elem_size);

/**
 * @brief Función de parseo vacía que no realiza ninguna acción sobre el buffer.
 *
 * Esta función cumple con la firma requerida para funciones de parseo (`t_parse_fn`),
 * pero no ejecuta ninguna operación. Es útil como valor por defecto cuando no se 
 * requiere procesar datos adicionales desde el buffer.
 *
 * Se espera que esta función sea pasada como argumento a funciones que aceptan un 
 * parser genérico, permitiendo omitir condicionales internos mediante una interfaz uniforme.
 *
 * @param buffer Puntero al buffer de entrada (no utilizado).
 * @param desplazamiento Puntero al desplazamiento actual dentro del buffer (no modificado).
 * @param extra_data Puntero a datos adicionales (no utilizado).
 */
void noop_parse_entry(void *buffer, int *desplazamiento, void *extra_data);
 
#endif /* PROTOCOL_H */
