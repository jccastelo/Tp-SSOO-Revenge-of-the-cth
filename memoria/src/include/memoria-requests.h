#ifndef MEMORIA_REQUESTS_H
#define MEMORIA_REQUESTS_H

// Incluimos las biblotecas internas:
#include "memoria-protocols.h"
#include "memoria-instructions.h"
#include "memoria-user-spaces.h"
#include "memoria-pages-tables.h"
#include "memoria-processes.h"

/**
 * Tipo de función para realizar operaciones personalizadas sobre la memoria de usuario
 * una vez resuelta la dirección física.
 * 
 * Esta función recibe:
 * - `client_socket`: Socket del cliente que solicita la operación, utilizado para enviar respuestas.
 * - `id_process`: Identificador del proceso que realiza el acceso a memoria.
 * - `frame`: Número de frame físico donde se realizará la operación.
 * - `extra_data`: Puntero a datos adicionales necesarios para la operación. 
 *                 Por ejemplo, puede ser un puntero a un entero que indique 
 *                 el tamaño de los datos a leer, o un puntero a un buffer que 
 *                 contenga los datos a escribir.
 * - `offset`: Desplazamiento dentro del frame, utilizado como punto de inicio
 *             para la operación (lectura o escritura).
 * 
 * Este tipo de función se utiliza como callback para definir operaciones 
 * específicas (lectura, escritura, etc.) que se ejecutan una vez que se 
 * resuelve la dirección física a partir de las tablas de páginas.
 */
typedef void (*t_execute_operation)(int client_socket, int id_process, int frame, void *extra_data, int offset);

/**
 * @brief Inicializa un proceso en el servidor en respuesta a una solicitud del kernel.
 * 
 * Esta función se utiliza para recibir una solicitud del kernel para la creación y inicialización 
 * de un nuevo proceso. Se encarga de configurar los parámetros necesarios, como la asignación 
 * de recursos y la preparación del entorno del proceso, para que este pueda comenzar a ejecutar 
 * tareas específicas según la solicitud recibida.
 * 
 * @param client_socket El socket a través del cual el servidor interactúa con el cliente,
 *                      permitiendo la transmisión de datos necesarios para la inicialización.
 */
void init_process(int client_socket);

/**
 * @brief Envía una instrucción de un proceso a solicitud del cliente (CPU).
 * 
 * Esta función coordina la recepción de los parámetros necesarios (ID del proceso y program counter)
 * y la posterior obtención y envío de la instrucción correspondiente desde la memoria. 
 * Se utiliza como parte del protocolo de comunicación entre el módulo de memoria y otros componentes, 
 * permitiendo que el cliente obtenga la instrucción que necesita ejecutar.
 * 
 * @param cliente_socket El socket a través del cual se recibe la solicitud y se envía la instrucción.
 */
void send_process_instruction(int cliente_socket);

/**
 * @brief Atiende una solicitud de acceso a las tablas de páginas por parte de un cliente.
 *
 * Esta función recibe desde el cliente (por ejemplo, la CPU) la dirección lógica de acceso,
 * junto con el identificador del proceso. Luego, obtiene las entradas de tabla de páginas
 * asociadas, resuelve el número de marco (frame) físico correspondiente y se lo envía de
 * vuelta al cliente.
 *
 * @param client_socket Socket del cliente que realiza la solicitud de traducción de dirección.
 */
void access_to_page_tables(int client_socket);

/**
 * @brief Ejecuta una operación de acceso a memoria en espacios de usuario.
 *
 * Esta función generaliza el manejo de accesos a memoria (lectura o escritura) 
 * en los espacios de usuario de un proceso. Recibe el socket del cliente, 
 * una función que define la operación a ejecutar (por ejemplo, `write_memory` o `read_memory`), 
 * y una función de parseo para interpretar datos adicionales si es necesario.
 *
 * Internamente, recibe una dirección física y datos asociados desde el cliente, 
 * obtiene el frame físico correspondiente y ejecuta la operación solicitada.
 *
 * @param cliente_socket Socket del cliente que realiza la solicitud.
 * @param execute_operation Función que define la operación a ejecutar sobre memoria.
 * @param parse_fn Función encargada de parsear los datos extra del buffer recibido.
 */
void operation_in_user_spaces(int cliente_socket, t_execute_operation execute_operation, parse_func_t parse_fn);

/**
 * @brief Ejecuta una operación de escritura en espacios de usuario.
 *
 * Esta función prepara una operación de escritura en memoria invocando 
 * `operation_in_user_spaces` con la función `write_memory` como operación principal
 * y `parsear_string` como función de parseo de los datos a escribir.
 *
 * @param client_socket Socket del cliente desde el cual se reciben los datos de escritura.
 */
void write_in_user_spaces(int client_socket);

/**
 * @brief Ejecuta una operación de lectura desde espacios de usuario.
 *
 * Esta función prepara una operación de lectura de memoria invocando 
 * `operation_in_user_spaces` con la función `read_memory` como operación principal 
 * y `noop_parse_entry` como función de parseo (ya que no se espera datos del cliente).
 *
 * @param client_socket Socket del cliente que solicita la lectura.
 */
void read_in_user_spaces(int client_socket);


#endif // MEMORIA_REQUESTS_H
