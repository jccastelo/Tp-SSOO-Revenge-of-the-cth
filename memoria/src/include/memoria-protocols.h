#ifndef MEMORIA_PROTOCOLS_H
#define MEMORIA_PROTOCOLS_H

// Incluyo las bibliotecas externas necesarias:
#include <utils/protocolo.h>
#include <utils/logger.h>

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

#endif // MEMORIA_PROTOCOLS_H