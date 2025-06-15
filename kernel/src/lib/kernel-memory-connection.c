
#include "../include/kernel-memory-connection.h"

void kernel_memory_connection(void) {
    // Incializamos variables necesarias para la conexión:
    char *puerto_memoria = string_itoa(config_kernel->PUERTO_MEMORIA);

    setup_connection_with_server("MEMORIA", config_kernel->IP_MEMORIA, puerto_memoria, set_socket_memoria);
}

void set_socket_memoria(int socket) {
    // Guardamos el socket de la memoria en la variable global (lo recibimos de setup connection with server)
    socket_memoria = socket;

    // Enviamos el handshake a la memoria:
    generar_handshake(socket_memoria, "KERNEL a MEMORIA");
}



int solicitar_a_memoria(int (*operacion)(t_pcb* proceso), t_pcb* proceso_a_enviar) {

    kernel_memory_connection();

    int resultado;

    resultado = operacion(proceso_a_enviar); // Usa el socket global de set_socket_memoria

    close(socket_memoria);

    return resultado;
}

// Operaciones

int memoria_init_proc(t_pcb* proceso) {
    
    int tamanio_nombre_archivo = strlen(proceso->archivo) + 1;

    t_paquete* paquete = crear_paquete(INIT_PROC);

    agregar_a_paquete(paquete, &proceso->pid, sizeof(int));
    agregar_a_paquete(paquete, &proceso->tamanio_proceso, sizeof(int));
    agregar_a_paquete(paquete, &tamanio_nombre_archivo, sizeof(int));
    agregar_a_paquete(paquete, proceso->archivo, strlen(proceso->archivo) + 1);
    
    int respuestaMemoria;
    enviar_paquete(paquete, socket_memoria);
    recv(socket_memoria, &respuestaMemoria, sizeof(int), MSG_WAITALL);

    eliminar_paquete(paquete);

    return respuestaMemoria;
   
}

int avisar_dump_memory(t_pcb* proceso){

    t_paquete* paquete = crear_paquete(DUMP_MEMORY); //Creo paquete con syscall Dump

    agregar_a_paquete(paquete, &(proceso->pid), sizeof(int)); //Pongo en el paquete el PID Del proceso a eliminar

    enviar_paquete(paquete, socket_memoria); //Envio el paquete
    
    eliminar_paquete(paquete);

    int resultado;

    log_info(logger, "Proceso en COnsultando DUMP");
    int bytes_recibidos = recv(socket_memoria, &resultado, sizeof(int), MSG_WAITALL); //Espero una respuesta de OK

     if (bytes_recibidos <= 0) {
    perror("recv");
    log_error(logger, "No se pudo recibir respuesta de memoria parA DUMP MEMORY proceso");
    }

    if(resultado == 1)
    {log_info(logger,"MEmoria confirma DUMP MEMORY %i:", proceso->pid);}

    return resultado;

}

int memoria_delete_process(t_pcb* proceso)
{
    t_paquete* paquete = crear_paquete(EXIT_SYS); //Creo paquete con syscall de salida

    agregar_a_paquete(paquete, &(proceso->pid), sizeof(int)); //Pongo en el paquete el PID Del proceso a eliminar

    enviar_paquete(paquete, socket_memoria); //Envio el paquete
    
    eliminar_paquete(paquete);

    int resultado;

    log_info(logger, "Proceso en COnsultando salida");
    int bytes_recibidos = recv(socket_memoria, &resultado, sizeof(int), MSG_WAITALL); //Espero una respuesta de OK

     if (bytes_recibidos <= 0) {
    perror("recv");
    log_error(logger, "No se pudo recibir respuesta de memoria par eliminar proceso");
    }

    if(resultado == 1)
    {log_info(logger,"MEmoria confirma Eliminar proceso %d :", proceso->pid);}

    return resultado;
}