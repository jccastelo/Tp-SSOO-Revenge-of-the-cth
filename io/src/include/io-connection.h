#ifndef IO_CONNECTION_H_
#define IO_CONNECTION_H_

// Incluye las bibliotecas necesarias externas:
#include <utils/connection.h>
#include <utils/protocolo.h>
#include <commons/string.h>

// Incluye las bibliotecas internas necesarias:
#include "io-state.h"
void identificarse_como_io();
/**
 * @brief Establece una conexión con un servidor y ejecuta una función callback con el socket del cliente.
 * 
 * Esta función utiliza la configuración previamente definida (como el nombre lógico del servidor,
 * la IP, el puerto y la función callback asociada) para intentar establecer una conexión con un servidor.
 * Una vez conectado, se invoca la función callback con el socket obtenido. Si la conexión falla,
 * se registra un mensaje de error y se finaliza la ejecución del programa.
 * 
 * Es útil para inicializar conexiones con componentes del sistema como parte de la etapa de configuración.
 */
void io_connect(char* nombre);

/**
 * @brief Asigna el socket conectado del kernel para su uso posterior en el sistema.
 * 
 * Esta función se utiliza para guardar el socket que representa la conexión activa con el kernel.
 * Permite que otros módulos del sistema accedan al socket cuando sea necesario comunicarse con el kernel.
 * 
 * @param socket Descriptor del socket conectado al kernel.
 */
void set_socket_kernel(int socket);

void enviar_nombre_kernel(char* nombre);

#endif /* IO_CONNECTION_H_ */