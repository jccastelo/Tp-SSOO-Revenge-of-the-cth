#ifndef CPU_CONNECT_H_
#define CPU_CONNECT_H_

// Incluimos las bibliotecas externas necesarias:
#include <commons/string.h>
#include <utils/connection.h>
#include <utils/protocolo.h>

// Incluimos las bibliotecas internas necesarias:
#include "cpu-state.h"

/**
 * @brief Establece la conexión entre el CPU y el kernel.
 * 
 * Esta función se encarga de iniciar la conexión de la CPU con el kernel del sistema operativo, 
 * permitiendo la interacción entre ambos componentes. A través de esta conexión, el CPU podrá 
 * enviar y recibir instrucciones, así como notificaciones de eventos como interrupciones, 
 * finalización de ejecución de instrucciones, y más.
 * 
 * Generalmente, esta función se invoca durante la fase de inicialización del módulo de CPU.
 */
void cpu_connect();

/**
 * @brief Asigna el socket utilizado para la comunicación con el servidor de dispatch del kernel.
 * 
 * Esta función permite establecer y almacenar el descriptor de socket que será utilizado por la CPU 
 * para comunicarse con el componente de dispatch del kernel. Esta conexión es esencial para enviar 
 * y recibir instrucciones relacionadas con la planificación y ejecución de procesos.
 * 
 * @param socket Descriptor del socket a utilizar para la comunicación con el servidor de dispatch.
 */
void set_socket_kernel_dispatch(int socket);

/**
 * @brief Asigna el socket utilizado para la comunicación con el servidor de interrupciones del kernel.
 * 
 * Esta función establece el socket mediante el cual la CPU puede interactuar con el servidor de 
 * interrupciones del kernel. Esto permite a la CPU notificar al kernel sobre interrupciones 
 * importantes que pueden requerir un cambio de contexto u otras acciones inmediatas.
 * 
 * @param socket Descriptor del socket a utilizar para la comunicación con el servidor de interrupciones.
 */
void set_socket_kernel_interrupt(int socket);

/**
 * @brief Asigna el socket utilizado para la comunicación con el módulo de memoria.
 * 
 * Esta función define el socket a través del cual la CPU interactúa con el módulo de memoria del sistema. 
 * A través de esta conexión, la CPU puede realizar operaciones de lectura y escritura, además de gestionar 
 * accesos a páginas o segmentos según la lógica del sistema de memoria implementado.
 * 
 * @param socket Descriptor del socket que conecta la CPU con el módulo de memoria.
 */
void set_socket_memoria(int socket);

void enviar_id_cpu(int socket);

#endif // CPU_CONNECT_H_