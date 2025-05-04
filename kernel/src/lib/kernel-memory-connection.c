
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

void memoria_init_proc(t_pcb* process) {
     t_paquete* paquete = crear_paquete(INIT_PROC);
     agregar_a_paquete(paquete, process->archivo, strlen(process->archivo) + 1);
     agregar_a_paquete(paquete, &process->tamanio_proceso, sizeof(int));
     enviar_paquete(paquete, socket_memoria);

    pthread_t memoria_init_proc;

    pthread_create(&memoria_init_proc, NULL, (void*) kernel_wait_init_proc(process), NULL);

    pthread_join(memoria_init_proc,NULL);

}

void* kernel_wait_init_proc(t_pcb* process)
{   
    int32_t resultado;
	int32_t result_Ok = 0;
	int32_t result_Sin_Espacio = -1;

    if( planner->long_term->algoritmo_planificador == queue_FIFO )
    {
        while(resultado == result_Sin_Espacio)
        {
            recv(socket_memoria, &resultado, sizeof(int32_t), MSG_WAITALL);
            //sd
            if(resultado == result_Ok)
            {
                queue_process(process,READY);
            }
        }
    }
    
    else if ( planner->long_term->algoritmo_planificador == queue_FIFO ) //PROCESO MAS CHICO PRIMERO
    {   
        recv(socket, &resultado, sizeof(int32_t), MSG_WAITALL);
    }

}
