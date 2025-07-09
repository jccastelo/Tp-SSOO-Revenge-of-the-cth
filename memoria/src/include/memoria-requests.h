#ifndef MEMORIA_REQUESTS_H
#define MEMORIA_REQUESTS_H

// Incluimos las biblotecas internas:
#include "memoria-protocols.h"
#include "memoria-instructions.h"
#include "memoria-user-spaces.h"
#include "memoria-pages-tables.h"
#include "memoria-processes.h"
#include "swap-funtions.h"
#include "memoria-processes.h"
#include "memoria-page-tables-operations.h"

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
 * @brief Suspende un proceso, guardando su contenido de memoria principal en el archivo de swap.
 *
 * Se escriben todas las páginas del proceso en el archivo `swap.bin`, se libera la memoria real 
 * (bitarray y tabla de páginas), y se guarda la metadata de swap en un diccionario para permitir
 * su futura restauración.
 *
 * @param client_socket Socket del cliente que solicita la suspensión (usado para recibir el PID).
 */
void suspend_process(int client_socket);

/**
 * @brief Restaura un proceso suspendido, trayendo sus páginas desde el archivo de swap a memoria real.
 *
 * Se verifica si hay suficiente memoria disponible, se leen los contenidos del swap en marcos libres,
 * se reconstruye la tabla de páginas y se actualiza el diccionario global.
 *
 * @param client_socket Socket del cliente que solicita la reanudación (usado para recibir el PID).
 */
void remove_suspend_process(int client_socket);
void dump_process(int client_socket);
/**
 * @brief Finaliza un proceso, liberando todos sus recursos tanto en memoria real como en swap.
 *
 * Se eliminan las entradas de la tabla de páginas del proceso, se liberan los marcos que ocupaba,
 * y se destruye la metadata de swap en caso de que el proceso estuviera suspendido.
 *
 * @param client_socket Socket del cliente que solicita la finalización (usado para recibir el PID).
 */
void finish_process(int client_socket); 




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

/**
 * @brief Finaliza un proceso solicitado por el cliente y envía la respuesta correspondiente.
 *
 * Esta función recibe, a través del socket especificado, el identificador del proceso que 
 * se desea finalizar. Una vez recibido, registra en el log la intención de finalizar dicho proceso.
 *
 * Actualmente, la lógica para verificar si el proceso ya ha finalizado está pendiente 
 * de implementación (comentada en el código). En el futuro, se podrá verificar el estado 
 * real del proceso antes de enviar la respuesta.
 *
 * Finalmente, se envía una respuesta al cliente indicando el estado de la operación 
 * (por ejemplo, OK o ERROR), permitiéndole conocer si la solicitud fue aceptada correctamente.
 *
 * @param client_socket Descriptor del socket desde el cual se recibe el ID del proceso
 *                      y al cual se envía la respuesta.
 *
 * @details
 * - Llama internamente a `rcv_process_to_end` para recibir el ID del proceso.
 * - Loguea la acción para facilitar el seguimiento y depuración.
 * - La verificación del estado del proceso aún está marcada como "To Do".
 */

void finish_process(int client_socket);

#endif // MEMORIA_REQUESTS_H
