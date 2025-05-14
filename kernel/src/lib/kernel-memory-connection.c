
#include "../include/kernel-memory-connection.h"

void kernel_memory_connection(void) {
    // Incializamos variables necesarias para la conexión:
    char *puerto_memoria = string_itoa(config_kernel->PUERTO_MEMORIA);

    setup_connection_with_server("MEMORIA", config_kernel->IP_MEMORIA, puerto_memoria, set_socket_memoria);
}

void set_socket_memoria(int socket) {
    // Guardamos el socket de la memoria en la variable global:
    socket_memoria = socket;

    // Enviamos el handshake a la memoria:
    generar_handshake(socket_memoria, "KERNEL a MEMORIA");
}



char* memoria_init_proc(t_pcb* process) {

    int tamanio_nombre_archivo = strlen(process->archivo) + 1;

    t_paquete* paquete = crear_paquete(INIT_PROC);
    agregar_a_paquete(paquete, &tamanio_nombre_archivo, sizeof(int));
    agregar_a_paquete(paquete, &process->archivo, strlen(process->archivo) + 1);
    agregar_a_paquete(paquete, &process->tamanio_proceso, sizeof(int));
    agregar_a_paquete(paquete, &process->pid, sizeof(int));
    enviar_paquete(paquete, socket_memoria);
    eliminar_paquete(paquete);
    
    pthread_t memoria_recive_proc;

    pthread_create(&memoria_recive_proc, NULL, (void*) kernel_wait_init_proc(process), NULL);

    void* retorno;
    
    pthread_join(memoria_recive_proc, &retorno);

    char* resultado = (char*)retorno;

    return resultado;
}

void* kernel_wait_init_proc(t_pcb* process)
{   
    void *resultado;

    recv(socket_memoria, &resultado, sizeof(int), MSG_WAITALL);
        
    return resultado;
}

int memory_delete_process(t_pcb *process_to_delate)
{
    pthread_t memoria_delate_proc; //Creo hilo para eliminar el proceso

    pthread_create(&memoria_delate_proc, NULL, (void*) kernel_wait_delate_proc(process_to_delate), NULL); //Le asigno la funcion de eliminacion

    void* retorno; // se supone que es entero
    pthread_join(memoria_delate_proc, &retorno); 
    int retorno_int = *(int*)retorno;

    return retorno_int;
}

void *kernel_wait_delate_proc(t_pcb *process_to_delate)
{
    t_paquete* paquete = crear_paquete(EXIT_Sys); //Creo paquete con syscall de salida

    agregar_a_paquete(paquete, &(process_to_delate->pid), sizeof(int)); //Pongo en el paquete el PID Del proceso a eliminar

    enviar_paquete(paquete, socket_memoria); //Envio el paquete
    
    eliminar_paquete(paquete);

    void* resultado;

    recv(socket_memoria, &resultado, sizeof(int), MSG_WAITALL); //ESpero una respuesta de OK
            
    return resultado;
}
