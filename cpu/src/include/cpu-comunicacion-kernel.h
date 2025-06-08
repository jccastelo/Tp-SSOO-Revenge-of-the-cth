#ifndef CPU_COMUNICACION_KERNEL_H
#define CPU_COMUNICACION_KERNEL_H


#include <utils/logger.h>
#include <utils/protocolo.h>

#include "cpu-types.h"
#include "cpu-state.h"

/* -------------------- Recepción de contexto -------------------- */

/**
 * @brief Recibe un contexto del Kernel a través del socket de dispatch.
 *
 * Espera un paquete con el código de operación PROCESS_CONTEXT y deserializa el contenido en `contexto`.
 */
void recibir_contexto_de_kernel();

/**
 * @brief Deserializa el contexto recibido desde el Kernel.
 *
 * Asigna memoria al contexto global y copia los campos `pid` y `pc` desde el buffer.
 *
 * @param buffer Buffer recibido desde el Kernel.
 */
void deserializar_contexto(t_buffer *buffer);


/* -------------------- Manejo de interrupciones -------------------- */

/**
 * @brief Verifica si se recibió una interrupción desde el Kernel.
 *
 * Realiza un `recv` no bloqueante sobre el socket de interrupciones y analiza el código de operación.
 *
 * @return true Si se recibió una interrupción válida.
 * @return false Si no se recibió interrupción.
 */
bool recibir_interrupciones();

/**
 * @brief Envía el contexto actual al Kernel ante un desalojo por interrupción.
 *
 * Arma un paquete con el contexto y lo envía usando el socket de dispatch.
 */
void enviar_contexto_desalojo();


/* -------------------- Implementación de syscalls -------------------- */

/**
 * @brief Syscall de tipo IO.
 *
 * Envía al Kernel el dispositivo y tiempo de bloqueo, junto con el contexto actual.
 *
 * @param dispositivo Nombre del dispositivo.
 * @param tiempo Duración de la operación IO.
 */
void syscall_io(char* dispositivo, int tiempo);

/**
 * @brief Syscall INIT_PROC para iniciar un nuevo proceso.
 *
 * Informa al Kernel el archivo de instrucciones y tamaño del proceso, junto con el contexto.
 * 
 * No frena el ciclo de instrucciones.
 *
 * @param archivo Ruta del archivo de instrucciones.
 * @param tamanio Tamaño del proceso en memoria.
 */
void syscall_init_proc(char* archivo, int tamanio);

/**
 * @brief Syscall DUMP_MEMORY para solicitar un volcado del estado de memoria.
 *
 * Envía al Kernel el contexto del proceso solicitante.
 */
void syscall_dump_memory();

/**
 * @brief Syscall EXIT para indicar la finalización del proceso.
 *
 * Informa al Kernel el contexto del proceso que termina su ejecución.
 */
void syscall_exit();

/**
 * @brief Agrega el contexto actual al paquete especificado.
 *
 * Inserta `pid` y `pc` en el paquete.
 *
 * @param paquete Paquete al que se le agregará el contexto.
 */
void agregar_contexto_al_paquete(t_paquete* paquete);


#endif // CPU_CICLO_INSTRUCCION_H_