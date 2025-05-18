
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

    //void* retorno;
/*
    pthread_t memoria_recive_proc;

    pthread_create(&memoria_recive_proc, NULL, kernel_wait_init_proc, (void*) process);

    pthread_join(memoria_recive_proc, &retorno);
*/  
    void *resultado = malloc(3); ;
    int resultado99 ; //test
    int tamanio_nombre_archivo = strlen(process->archivo) + 1;

    t_paquete* paquete = crear_paquete(INIT_PROC);
    agregar_a_paquete(paquete, &tamanio_nombre_archivo, sizeof(int));
    agregar_a_paquete(paquete, process->archivo, strlen(process->archivo) + 1);
    agregar_a_paquete(paquete, &process->tamanio_proceso, sizeof(int));
    agregar_a_paquete(paquete, &process->pid, sizeof(int));
    enviar_paquete(paquete, socket_memoria);
    int bytes_recibidos= recv(socket_memoria, &resultado99, sizeof(int), MSG_WAITALL);
    eliminar_paquete(paquete);


    if (bytes_recibidos <= 0) {
    perror("recv");
    log_error(logger, "No se pudo recibir respuesta de memoria para iniciar proceso");
    }

    if(resultado99 == 99)//test
    {log_info(logger,"MEmoria confirma espacio para iniciar proceso %d :", process->pid);
    
    
    strcpy(resultado, "OK");
    }//test
    

    
    //char* resultado = (char*)retorno;

    return resultado;
   
}


int memory_delete_process(t_pcb *process_to_delate)
{
    /*
    pthread_t memoria_delate_proc; //Creo hilo para eliminar el proceso

    pthread_create(&memoria_delate_proc, NULL,  kernel_wait_delate_proc(process_to_delate), NULL); //Le asigno la funcion de eliminacion

    void* retorno; // se supone que es entero
    pthread_join(memoria_delate_proc, &retorno);
    */
    t_paquete* paquete = crear_paquete(EXIT_Sys); //Creo paquete con syscall de salida

    agregar_a_paquete(paquete, &(process_to_delate->pid), sizeof(int)); //Pongo en el paquete el PID Del proceso a eliminar

    enviar_paquete(paquete, socket_memoria); //Envio el paquete
    
    eliminar_paquete(paquete);

    int resultado51;
    //void* resultado;
    log_info(logger, "Proceso en COnsultando salida");
    int bytes_recibidos=recv(socket_memoria, &resultado51, sizeof(int), MSG_WAITALL); //ESpero una respuesta de OK

     if (bytes_recibidos <= 0) {
    perror("recv");
    log_error(logger, "No se pudo recibir respuesta de memoria par eliminar proceso");
    }

    if(resultado51 == 51)//test
    {log_info(logger,"MEmoria confirma Eliminar proceso %d :", process_to_delate->pid);}


    //int retorno_int = *(int*)resultado;

    return resultado51;
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

void* kernel_wait_init_proc(void *args)
{   
    t_pcb* process = (t_pcb*) args;
    void *resultado = malloc(3); ;
    int resultado99 ; //test
    int tamanio_nombre_archivo = strlen(process->archivo) + 1;

    t_paquete* paquete = crear_paquete(INIT_PROC);
    agregar_a_paquete(paquete, &tamanio_nombre_archivo, sizeof(int));
    agregar_a_paquete(paquete, process->archivo, strlen(process->archivo) + 1);
    agregar_a_paquete(paquete, &process->tamanio_proceso, sizeof(int));
    agregar_a_paquete(paquete, &process->pid, sizeof(int));
    enviar_paquete(paquete, socket_memoria);
    recv(socket_memoria, &resultado99, sizeof(int), MSG_WAITALL);
    eliminar_paquete(paquete);

    
    


    if(resultado99 == 99)//test
    {log_info(logger,"MEmoria confirma espacio para iniciar proceso %d :", process->pid);
    
    
    strcpy(resultado, "OK");
    }//test
    
    return resultado;
}
