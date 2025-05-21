#include  "../include/kernel-io-connections.h"

void gestionar_io(t_buffer *buffer,int socket)
{
    int desplazamiento = 0;
    char *ioNombre;
    int milisegundos;

    // Copiamos el tamanio del nombre
    int tamanio_nombre=0;
    memcpy(&tamanio_nombre, buffer->stream+ desplazamiento, sizeof(int));
    desplazamiento += sizeof(int);

    // Copiamos el nombre de la IO
    ioNombre= malloc(tamanio_nombre + 1);
    memcpy(ioNombre, buffer->stream+  desplazamiento, tamanio_nombre);
    ioNombre[tamanio_nombre] = '\0'; 
    desplazamiento+=tamanio_nombre;
    
    // Copiamos el TIEMPO DE IO
    memcpy(&milisegundos, buffer->stream+desplazamiento, sizeof(int));
    desplazamiento += sizeof(int);

    int tamanio_pid; 
    int pid_a_io; //Solo necesito el pid del proces

    memcpy(&tamanio_pid, buffer->stream + desplazamiento, sizeof(int));
    desplazamiento += sizeof(int);

    memcpy(&pid_a_io, buffer->stream + desplazamiento, tamanio_pid);

    t_IO *ioBuscada = existe_io(ioNombre);

    if(ioBuscada != NULL)
    {
        if(list_size(ioBuscada->procesos_esperando->queue_ESTADO) == 0)
        {
        
        }
        else{
            pthread_mutex_lock(&ioBuscada->procesos_esperando->queue_ESTADO);
            list_add(ioBuscada->procesos_esperando->queue_ESTADO,pid_a_io);
            pthread_mutex_unlock(&ioBuscada->procesos_esperando->queue_ESTADO);
        }
    }
    else
    {
        t_pcb *process_to_delate = list_get(list_procesos->queue_ESTADO, pid_a_io); //Obtengo el proceso a eliminar de la lista global

        queue_process(process_to_delate,EXIT);
        
    }

}

t_IO *existe_io(char *ioNombreBuscado)
{
    for(int i = 0; i< list_size(list_ios->queue_ESTADO) ; i++)
    {   
        t_IO* io = list_get(list_ios->queue_ESTADO,i);

        if(strcmp(io->nombre,ioNombreBuscado)==0)
        {
            return io;
        }
    }

    return NULL;
}

int recibir_pid(int io_socket){
    int pid;
    recv(io_socket, &pid, sizeof(int), MSG_WAITALL);
    return pid;
}

void enviar_proceso_io(int io_socket){
    
    for(int i = 0; i < list_size(list_ios->queue_ESTADO); i++){
        t_IO* ios = list_get(list_ios->queue_ESTADO, i);
        
        for(int j = 0; j < list_size(ios->instancias_IO); j++){
            t_IO_instancia* io = list_get(ios->instancias_IO, j);

            if(io->socket == io_socket){
                pthread_mutex_lock(&ios->procesos_esperando->mutex);
                int pid = list_remove(ios->procesos_esperando, 0);
                pthread_mutex_unlock(&ios->procesos_esperando->mutex);
                
                send(io_socket, &pid, sizeof(int), NULL);
                return;
            }
        }
    }
}

void eliminar_instancia(int io_socket){
    
    for(int i = 0; i < list_size(list_ios->queue_ESTADO); i++){
        t_IO* ios = list_get(list_ios->queue_ESTADO, i);
        
        for(int j = 0; j < list_size(ios->instancias_IO); j++){
            t_IO_instancia* io = list_get(ios->instancias_IO, j);

            if(io->socket == io_socket){
                
                pthread_mutex_lock(&ios->procesos_esperando->mutex);
                int pid = list_remove(ios->procesos_esperando, 0);
                pthread_mutex_unlock(&ios->procesos_esperando->mutex);
                
                carnicero_de_instancias(io);

                if(list_size(ios->instancias_IO) == 0){

                    carnicero_de_io(ios);
                } 

                return;
            }
        }
    }    
}

