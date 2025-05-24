#include "../include/kernel-io-connections.h"

//CPu a Kernel
void gestionar_io(t_buffer *buffer)
{
    int desplazamiento = 0;
    char *ioNombre;
    int milisegundos;

    // Copiamos el tamanio del nombre
    int tamanio_nombre = 0;
    memcpy(&tamanio_nombre, buffer->stream + desplazamiento, sizeof(int));
    desplazamiento += sizeof(int);

    // Copiamos el nombre de la IO
    ioNombre = malloc(tamanio_nombre + 1);
    memcpy(ioNombre, buffer->stream + desplazamiento, tamanio_nombre);
    ioNombre[tamanio_nombre] = '\0';
    desplazamiento += tamanio_nombre;

    // Copiamos el TIEMPO DE IO
    memcpy(&milisegundos, buffer->stream + desplazamiento, sizeof(int));
    desplazamiento += sizeof(int);

    
    int pid_a_io; // Solo necesito el pid del proces

    memcpy(&pid_a_io, buffer->stream + desplazamiento, sizeof(int));

    t_pcb* process = list_get(list_procesos->queue_ESTADO,pid_a_io);
    queue_process(process, BLOCKED);


    t_IO *ioBuscada = buscar_io(ioNombre);
    
    if (ioBuscada != NULL)
    {
        t_buffer *buffer_io = crear_buffer_io(milisegundos, pid_a_io);
        t_IO_instancia *instancia_io_libre = buscar_instancia_libre(ioBuscada);

        if (list_size(ioBuscada->procesos_esperando->queue_ESTADO) == 0 && instancia_io_libre != NULL)
        {
            pthread_mutex_lock(&ioBuscada->procesos_esperando->mutex);
            list_add(ioBuscada->procesos_esperando->queue_ESTADO, buffer_io);
            pthread_mutex_unlock(&ioBuscada->procesos_esperando->mutex);
            
            int socket_io_libre = instancia_io_libre->socket;
            enviar_proceso_io(socket_io_libre);
        }
        else
        {
            pthread_mutex_lock(&ioBuscada->procesos_esperando->mutex);
            list_add(ioBuscada->procesos_esperando->queue_ESTADO, buffer_io);
            pthread_mutex_unlock(&ioBuscada->procesos_esperando->mutex);
        }
    }
    else
    {
        t_pcb *process_to_delate = list_get(list_procesos->queue_ESTADO, pid_a_io); // Obtengo el proceso a eliminar de la lista global

        queue_process(process_to_delate, EXIT);
    }
}

t_buffer *crear_buffer_io(int milisegundos,int  pid_a_io)
{
    //INICIO BUFFER
    t_buffer* new_buffer = malloc(sizeof(t_buffer));
    new_buffer->size = 0;
    new_buffer->stream = NULL;

    int total_size = sizeof(int) + sizeof(int);

    new_buffer->stream = malloc(total_size);
    new_buffer->size = total_size;

    int desplazamiento = 0;

    // Copio el pid del proceso
    memcpy(new_buffer->stream + desplazamiento, &pid_a_io ,sizeof(int));
    desplazamiento += sizeof(int);

    // Copiar los milisegundos
    memcpy(new_buffer->stream + desplazamiento, &milisegundos ,sizeof(int));
    desplazamiento += sizeof(int);

    return new_buffer;
}

t_IO_instancia *buscar_instancia_libre(t_IO *ioBuscada)
{
    for (int j = 0; j < list_size(ioBuscada->instancias_IO->queue_ESTADO); j++)
        {
            t_IO_instancia *io_instancia = list_get(ioBuscada->instancias_IO->queue_ESTADO, j);

            if (io_instancia->proceso < 0 )
            {
                return io_instancia;
            }
        }

    return NULL;
}

t_IO *buscar_io(char *ioNombreBuscado)
{
    t_IO *encontrado = NULL;

    for (int i = 0; i < list_size(list_ios->queue_ESTADO); i++)
    {
         encontrado  = list_get(list_ios->queue_ESTADO, i);

        if (strcmp(encontrado->nombre, ioNombreBuscado) == 0)
        {
            return encontrado;
        }
    }

    return NULL;
}


//IO a Kernel
int recibir_pid(int io_socket)
{
    int pid;
    recv(io_socket, &pid, sizeof(int), MSG_WAITALL);
    return pid;
}

void enviar_proceso_io(int io_socket)
{

    for (int i = 0; i < list_size(list_ios->queue_ESTADO); i++)
    {
        t_IO *ios = list_get(list_ios->queue_ESTADO, i);

        for (int j = 0; j < list_size(ios->instancias_IO->queue_ESTADO); j++)
        {
            t_IO_instancia *io = list_get(ios->instancias_IO->queue_ESTADO, j);

            if (io->socket == io_socket)
            {
                if(!list_is_empty(ios->procesos_esperando)){

                pthread_mutex_lock(&ios->procesos_esperando->mutex);
                int pid = list_remove(ios->procesos_esperando->queue_ESTADO, 0);
                pthread_mutex_unlock(&ios->procesos_esperando->mutex);

                send(io_socket, &pid, sizeof(int), 0);
                }

                return;
            }
        }
    }
}

// Carniero en busqueda de su fiambre

void eliminar_instancia(int io_socket)
{

    for (int i = 0; i < list_size(list_ios->queue_ESTADO); i++)
    {
        t_IO *ios = list_get(list_ios->queue_ESTADO, i);

        for (int j = 0; j < list_size(ios->instancias_IO->queue_ESTADO); j++)
        {
            t_IO_instancia *io = list_get(ios->instancias_IO->queue_ESTADO, j);

            if (io->socket == io_socket)
            {

                pthread_mutex_lock(&ios->procesos_esperando->mutex);
                int pid = list_remove(ios->procesos_esperando->queue_ESTADO, 0);
                pthread_mutex_unlock(&ios->procesos_esperando->mutex);

                carnicero_de_instancias_io(io);

                if (list_size(ios->instancias_IO->queue_ESTADO) == 0)
                {

                    carnicero_de_io(ios);
                }

                return;
            }
        }
    }
}

void recibir_io(int socket)
{
    int tamanio_nombre;
    recv(socket, &tamanio_nombre, sizeof(int), MSG_WAITALL);

    char *nombreIO = malloc(tamanio_nombre + 1);

    recv(socket, nombreIO, tamanio_nombre, MSG_WAITALL);
    nombreIO[tamanio_nombre] = '\0';


    t_IO *ioBuscada = buscar_io(nombreIO);

    if (ioBuscada != NULL)
    {
        //Creo instancia y agrego a estructura ya existente

        t_IO_instancia *nueva_instancia_io= malloc(sizeof(t_IO_instancia));
        nueva_instancia_io->socket = socket;
        nueva_instancia_io->proceso = -1;

        pthread_mutex_lock(&ioBuscada->instancias_IO->mutex);
        list_add(ioBuscada->instancias_IO->queue_ESTADO,nueva_instancia_io);
        pthread_mutex_unlock(&ioBuscada->instancias_IO->mutex);

        log_info(logger, "Llego una nueva INSTANCIA de IO de nombre %s Y SOCKET: %d ", ioBuscada->nombre,nueva_instancia_io->socket );
    }
    else
    {
        //Creo estructura de  0, inicializando todo
        t_IO *ioNueva = malloc(sizeof(t_IO));
        ioNueva->nombre = nombreIO;

        ioNueva->instancias_IO = malloc(sizeof(t_monitor));
        pthread_mutex_init(&(ioNueva->instancias_IO->mutex), NULL);
        ioNueva->instancias_IO->queue_ESTADO =list_create();

        ioNueva->procesos_esperando= malloc(sizeof(t_monitor));
        pthread_mutex_init(&(ioNueva->procesos_esperando->mutex), NULL);
        ioNueva->procesos_esperando->queue_ESTADO =list_create();

        //Creo la primera instancia para meter en la estructura nueva
        t_IO_instancia *nueva_instancia_io= malloc(sizeof(t_IO_instancia));
        nueva_instancia_io->socket = socket;
        nueva_instancia_io->proceso = -1;

        pthread_mutex_lock(&ioNueva->instancias_IO->mutex);
        list_add(ioNueva->instancias_IO->queue_ESTADO,nueva_instancia_io);
        pthread_mutex_unlock(&ioNueva->instancias_IO->mutex);

        pthread_mutex_lock(&list_ios->mutex);
        list_add(list_ios->queue_ESTADO,ioNueva);
        pthread_mutex_unlock(&list_ios->mutex);

        log_info(logger, "Llego una Nueva IO de nombre %s Y SOCKET: %d ", ioNueva->nombre,nueva_instancia_io->socket );
    }

}