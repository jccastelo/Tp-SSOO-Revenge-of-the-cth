
#include "../include/kernel-memory-connection.h"

pthread_mutex_t mutex_memoria = PTHREAD_MUTEX_INITIALIZER;

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

int enviar_pid_memoria(t_pcb* proceso, int codigo_operacion) {
    
    t_paquete* paquete = crear_paquete(codigo_operacion); // Creo paquete con codigo de operacion

    agregar_a_paquete(paquete, &(proceso->pid), sizeof(int)); // Pongo en el paquete el PID Del proceso

    enviar_paquete(paquete, socket_memoria); //Envio el paquete
    
    eliminar_paquete(paquete);

    int resultado;

    //log_info(logger, "Proceso en Consultando: %i", codigo_operacion);
    int bytes_recibidos = recv(socket_memoria, &resultado, sizeof(int), MSG_WAITALL); // Espero una respuesta de OK

    if (bytes_recibidos <= 0) {
        perror("recv");
        log_error(logger, "No se pudo recibir respuesta de memoria para %i proceso", codigo_operacion);
    }

    //if(resultado == 1)
    //{log_info(logger,"Memoria confirma %i %i:", codigo_operacion, proceso->pid);}

    return resultado;
}

int solicitar_a_memoria(int (*operacion)(t_pcb* proceso), t_pcb* proceso_a_enviar) {
    pthread_mutex_lock(&mutex_memoria);

    kernel_memory_connection();

    int resultado;

    resultado = operacion(proceso_a_enviar); // Usa el socket global de set_socket_memoria

    close(socket_memoria);

    pthread_mutex_unlock(&mutex_memoria);
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

    return enviar_pid_memoria(proceso, DUMP_MEMORY);
}

int memoria_delete_process(t_pcb* proceso) {
    
    return enviar_pid_memoria(proceso, EXIT_SYS);
}

int suspender_proceso(t_pcb* proceso){ 

    return enviar_pid_memoria(proceso, SUSPENDER);
}

int desuspender_proceso(t_pcb* proceso){

    return enviar_pid_memoria(proceso, DESUSPENDER);
}

//faltaría finalizar proceso