#ifndef MEMORIA_PROTOCOLS_H
#define MEMORIA_PROTOCOLS_H

// Incluyo las bibliotecas externas necesarias:
#include <utils/protocolo.h>
#include <utils/logger.h>

// Incluyo las bibliotecas internas necesarias:
#include "memoria-state.h"

/**
 * @brief Recibe y deserializa los datos necesarios para iniciar el procesamiento de un archivo enviado por un cliente.
 *
 * Esta función se encarga de recibir un buffer desde el socket del cliente, interpretar su contenido y extraer
 * tres elementos clave: el identificador del proceso, el tamaño del proceso y el archivo a procesar.
 * Los datos extraídos se almacenan en las variables provistas por el llamador.
 *
 * @param client_socket     Socket desde el cual se recibe el buffer con la información.
 * @param id_process        Puntero a un entero donde se almacenará el ID del proceso recibido.
 * @param tam_process       Puntero a un entero donde se almacenará el tamaño del proceso.
 * @param file_procces      Puntero a una cadena (char**) donde se almacenará el archivo recibido.
 *                          La memoria para la cadena se asigna dinámicamente dentro de la función.
 */
void rcv_setup_to_process(int client_socket, int *id_process, int *tam_process, char **file_procces);

/**
 * @brief Recibe un paquete con la instrucción solicitada por un consumidor (CPU).
 *
 * Esta función recibe un buffer desde el socket del cliente y extrae los valores enteros
 * correspondientes al ID del proceso y al contador de programa (program counter),
 * actualizando el desplazamiento en el buffer para la correcta lectura.
 *
 * @param client_socket Descriptor del socket desde donde se recibe la información.
 * @param id_process Identificador del proceso solicitado (se actualiza con el valor recibido).
 * @param program_counter Contador de programa solicitado (se actualiza con el valor recibido).
 */
void rcv_instruction_consumer(int client_socket, int *id_process, int *program_counter);

/**
 * @brief Envía una instrucción al consumidor (CPU) a través del socket.
 *
 * Esta función arma un paquete que contiene la instrucción solicitada, primero verificando
 * que la instrucción no sea NULL para evitar errores. Luego, agrega la instrucción al paquete,
 * la envía por el socket correspondiente y libera la memoria utilizada por el paquete.
 *
 * @param cliente_socket Descriptor del socket donde se enviará la instrucción.
 * @param id_process Identificador del proceso al que pertenece la instrucción.
 * @param program_counter Contador de programa correspondiente a la instrucción.
 * @param instruction Cadena de caracteres que representa la instrucción a enviar.
 */
void send_instruction_consumer(int cliente_socket, int id_process, int program_counter, char *instruction);

/**
 * @brief Recibe la finalización de un proceso desde un cliente a través de un socket.
 *
 * Esta función recibe un paquete enviado por un cliente, que indica que un proceso sera finalizado.
 * El paquete se recibe mediante el socket especificado y se interpreta para extraer el identificador
 * del proceso correspondiente. El ID se almacena en la dirección de memoria provista por el puntero
 * `id_process`. Luego de procesar el paquete, se libera la memoria asignada al buffer recibido.
 *
 * @param client_socket Descriptor del socket desde el cual se recibe el paquete.
 * @param id_process Puntero a una variable donde se almacenará el ID del proceso finalizado.
 */
void rcv_process_to_end(int client_socket, int *id_process);

/**
 * @brief Recibe y parsea un acceso a memoria física solicitado por un cliente a través de un socket.
 *
 * Esta función se encarga de recibir un paquete desde el socket especificado, el cual contiene
 * la información necesaria para acceder a una dirección física en memoria. El paquete incluye
 * el identificador del proceso que realiza el acceso, un arreglo con las entradas (índices) 
 * correspondientes a cada nivel de la tabla de páginas, y eventualmente datos adicionales que 
 * pueden ser parseados mediante una función personalizada.
 *
 * La función extrae y almacena el ID del proceso en la variable apuntada por `id_process`, 
 * y guarda las entradas por nivel en el arreglo apuntado por `physical_memory`.
 * Además, si se requiere procesar datos adicionales, se utiliza el puntero `extra_data` junto
 * con la función `parse_fn` proporcionada, permitiendo flexibilidad para distintos tipos 
 * de operaciones.
 *
 * @param client_socket Descriptor del socket desde el cual se recibe el paquete.
 * @param id_process Puntero a la variable donde se almacenará el ID del proceso recibido.
 * @param physical_memory Puntero al arreglo donde se guardarán las entradas por nivel parseadas.
 * @param extra_data Puntero a la estructura donde se almacenarán datos adicionales parseados.
 * @param parse_fn Función encargada de parsear los datos adicionales contenidos en el paquete.
 *
 * @return void
 */
void rcv_physical_memory_and_parse_memory_access(int client_socket, int *id_process, int *physical_memory, void* extra_data, parse_func_t parse_fn);

/**
 * Envía al cliente un paquete de respuesta que contiene el código de estado 
 * y, en caso exitoso, el contenido leído desde la memoria.
 *
 * Esta función crea un paquete utilizando el código de respuesta recibido. 
 * Si el estado es OK, también se agrega el buffer con el contenido leído (interpretado como string).
 * Finalmente, el paquete se envía al cliente y se elimina para liberar recursos.
 *
 * Parámetros:
 * - client_socket: Socket del cliente al cual se enviará el paquete.
 * - buffer: Puntero al contenido leído desde memoria (interpretado como string).
 * - response: Código de estado de la operación (por ejemplo, OK o ERROR).
 *
 * Notas:
 * - Si el contenido leído no es textual (binario), se recomienda adaptar la función
 *   para enviar datos binarios en lugar de strings.
 */

void send_read_content(int client_socket, char *buffer, int response);

/**
 * @brief Recibe y parsea las entradas por nivel para un acceso a memoria desde un socket.
 *
 * Esta función se encarga de recibir un paquete enviado por un cliente a través del socket 
 * especificado, el cual contiene el identificador del proceso que realiza la operación 
 * y una lista de índices que representan las entradas correspondientes a cada nivel de 
 * la tabla de páginas en un esquema de paginación multinivel.
 *
 * La función extrae y almacena el ID del proceso en la variable apuntada por `id_process`, 
 * y devuelve una lista dinámica (`t_list*`) que contiene las entradas por nivel necesarias 
 * para realizar la traducción de la dirección.
 *
 * @param client_socket Descriptor del socket desde el cual se recibe el paquete.
 * @param id_process Puntero a la variable donde se almacenará el ID del proceso recibido.
 *
 * @return t_list* Lista de enteros que representa las entradas por nivel a recorrer en 
 *                 la estructura de paginación.
 */
t_list *rcv_entries_per_levels(int client_socket, int *id_process);

/**
 * @brief Envía al cliente los valores de configuración actuales de la memoria.
 *
 * Esta función construye y envía un paquete al cliente a través del socket especificado,
 * conteniendo los parámetros principales de configuración de la memoria: tamaño de página,
 * cantidad de entradas por tabla y cantidad de niveles en el esquema de paginación multinivel.
 *
 * @param client_socket Descriptor del socket del cliente al cual se enviarán los datos.
 *
 * @details
 * El paquete enviado tiene el código de operación `MEMORY_CONFIG` e incluye:
 * - Tamaño de página (`TAM_PAGINA`)
 * - Cantidad de entradas por tabla de páginas (`ENTRADAS_POR_TABLA`)
 * - Cantidad de niveles de paginación (`CANTIDAD_NIVELES`)
 *
 * Esta información permite al cliente conocer la estructura y límites de la memoria
 * administrada por el sistema.
 */
void send_values_memory(int client_socket);

#endif // MEMORIA_PROTOCOLS_H