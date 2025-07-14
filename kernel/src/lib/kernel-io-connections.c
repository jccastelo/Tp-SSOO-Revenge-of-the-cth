#include "../include/kernel-io-connections.h"

//pthread_mutex_t mutex_io = PTHREAD_MUTEX_INITIALIZER;

//CPu a Kernel
void gestionar_io(t_buffer *buffer)
{
    //pthread_mutex_lock(&mutex_io);

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
    desplazamiento += sizeof(int);

    //Busco el PC
    int pc_PID;
    memcpy(&pc_PID, buffer->stream + desplazamiento, sizeof(int));

    pthread_mutex_lock(&list_procesos->mutex);    
    t_pcb* process = list_get(list_procesos->cola, pid_a_io);
    pthread_mutex_unlock(&list_procesos->mutex);

    process->pc = pc_PID;

    log_info(logger,"## PID: %d - Bloqueado por IO: %s", process->pid,ioNombre);
    queue_process(process, BLOCKED);

    
    t_IO *ioBuscada = buscar_io(ioNombre);
    
    if (ioBuscada != NULL)
    {
        t_buffer *buffer_io = crear_buffer_io(milisegundos, pid_a_io);

        t_IO_instancia *instancia_io_libre = buscar_instancia_libre(ioBuscada);

        pthread_mutex_lock(&ioBuscada->procesos_esperando->mutex);
        int tamanio = list_size(ioBuscada->procesos_esperando->cola);
        pthread_mutex_unlock(&ioBuscada->procesos_esperando->mutex);

        if (tamanio == 0 && instancia_io_libre != NULL) {
            
            pthread_mutex_lock(&ioBuscada->procesos_esperando->mutex);
            list_add(ioBuscada->procesos_esperando->cola, buffer_io);
            pthread_mutex_unlock(&ioBuscada->procesos_esperando->mutex);
            
            int socket_io_libre = instancia_io_libre->socket;
            
            enviar_proceso_io(socket_io_libre);

            //pthread_mutex_unlock(&mutex_io);
        }
        else {   

            pthread_mutex_lock(&ioBuscada->procesos_esperando->mutex);
            list_add(ioBuscada->procesos_esperando->cola, buffer_io);
            pthread_mutex_unlock(&ioBuscada->procesos_esperando->mutex);

            //pthread_mutex_unlock(&mutex_io);
        }
    }
    else
    {
        pthread_mutex_lock(&list_procesos->mutex);
        t_pcb *process_to_delate = list_get(list_procesos->cola, pid_a_io);
        pthread_mutex_unlock(&list_procesos->mutex);

        //pthread_mutex_unlock(&mutex_io);
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
    pthread_mutex_lock(&ioBuscada->instancias_IO->mutex);
    int cant_instancias = list_size(ioBuscada->instancias_IO->cola);
    for (int j = 0; j < cant_instancias; j++) {
            
        t_IO_instancia *io_instancia = list_get(ioBuscada->instancias_IO->cola, j);
    
        if (io_instancia->proceso < 0 ) {
            
            io_instancia->proceso = 9999999;
            pthread_mutex_unlock(&ioBuscada->instancias_IO->mutex);
            return io_instancia;
        }
        
    }
        
    pthread_mutex_unlock(&ioBuscada->instancias_IO->mutex);
    return NULL;
}

t_IO *buscar_io(char *ioNombreBuscado)
{
    t_IO *encontrado = NULL;
    
    pthread_mutex_lock(&list_ios->mutex); // como estoy recorriendo una lista cuyo tamanio cambia en tiempo de ejecucion, hay que clavar mutex antes de iterar
    int tamanio = list_size(list_ios->cola);
    for (int i = 0; i < tamanio; i++) {   
        
        encontrado = list_get(list_ios->cola, i);

        if (strcmp(encontrado->nombre, ioNombreBuscado) == 0) {
            pthread_mutex_unlock(&list_ios->mutex);
            return encontrado;
        }
    }
    pthread_mutex_unlock(&list_ios->mutex);

    return NULL;
}


//IO a Kernel
int recibir_pid(t_buffer *buffer, int io_socket)
{
    int pid;
    memcpy(&pid, buffer->stream, sizeof(int));

    return pid;
}

void enviar_proceso_io(int io_socket)
{
    pthread_mutex_lock(&list_ios->mutex);
    int cant_ios = list_size(list_ios->cola);
    for (int i = 0; i < cant_ios; i++) {
        
        t_IO *ios = list_get(list_ios->cola, i);

        pthread_mutex_lock(&ios->instancias_IO->mutex);
        int cant_instancias = list_size(ios->instancias_IO->cola);
        for (int j = 0; j < cant_instancias; j++) {
            
            t_IO_instancia *io = list_get(ios->instancias_IO->cola, j);

            if (io->socket == io_socket) {
                
                pthread_mutex_lock(&ios->procesos_esperando->mutex);
                int cant_procesos_esperando = list_size(ios->procesos_esperando->cola);

                if(cant_procesos_esperando) {

                    t_buffer* pid_y_milisegundos = (t_buffer*)list_remove(ios->procesos_esperando->cola, 0);
                    pthread_mutex_unlock(&ios->procesos_esperando->mutex);

                    send(io_socket, &pid_y_milisegundos->size, sizeof(int), 0);
                    send(io_socket, pid_y_milisegundos->stream, pid_y_milisegundos->size, 0);
                    
                    memcpy(&(io->proceso), pid_y_milisegundos->stream, sizeof(int));
                    
                    free(pid_y_milisegundos);
                }

                pthread_mutex_unlock(&ios->procesos_esperando->mutex);
                pthread_mutex_unlock(&ios->instancias_IO->mutex);
                pthread_mutex_unlock(&list_ios->mutex);
                
                return;
            } 
        }

        pthread_mutex_unlock(&ios->instancias_IO->mutex);  
    }
    
    pthread_mutex_unlock(&list_ios->mutex);
    return;
}

// Carniero en busqueda de su fiambre

void eliminar_instancia(int io_socket)
{
    pthread_mutex_lock(&list_ios->mutex);
    int cant_ios = list_size(list_ios->cola);
    for (int i = 0; i < cant_ios; i++) {
        
        t_IO *ios = list_get(list_ios->cola, i);

        pthread_mutex_lock(&ios->instancias_IO->mutex);
        int cant_instancias = list_size(ios->instancias_IO->cola);
        for (int j = 0; j < cant_instancias; j++) {
            
            t_IO_instancia *io = list_get(ios->instancias_IO->cola, j);

            if (io->socket == io_socket) {
                
                list_remove(ios->instancias_IO->cola,j);
                carnicero_de_instancias_io(io);

                if (list_is_empty(ios->instancias_IO->cola)) {
                    
                    desencolarProcesosEsperando(ios);
                    log_error(logger, "llego el carnicero");
                    carnicero_de_io(ios);

                    list_remove(list_ios->cola,i); 
                }

                return;
            }
        }
        
        pthread_mutex_unlock(&ios->instancias_IO->mutex);   
    }
    
    pthread_mutex_unlock(&list_ios->mutex);

    return;
}

void desencolarProcesosEsperando(t_IO *ios_estructura) {
   
    int tamano_lista = list_size(ios_estructura->procesos_esperando->cola);
    int i = 0;
    while(tamano_lista > i && tamano_lista > 0)
    {   
        t_buffer *pid_milisegundos  = list_remove(ios_estructura->procesos_esperando->cola,0);
        
        if(i != tamano_lista - 1){
        }

        int pid_a_remover;
        memcpy(&pid_a_remover, pid_milisegundos->stream, sizeof(int));

        //log_info(logger, "PID A ELIMINAR: %d, ELEMENTOS RESTANTES: %d",pid_a_remover, tamano_lista > i);

        free(pid_milisegundos);

        pthread_mutex_lock(&list_procesos->mutex);
        t_pcb* proceso = list_get(list_procesos->cola, pid_a_remover);
        pthread_mutex_unlock(&list_procesos->mutex);

        queue_process(proceso, EXIT);
        
        i++;
    }
}

void recibir_io(t_buffer* buffer, int socket) {

    int desplazamiento = 0;
    char *ioNombre;

    // Copiamos el tamanio del nombre
    int tamanio_nombre = 0;
    memcpy(&tamanio_nombre, buffer->stream + desplazamiento, sizeof(int));
    desplazamiento += sizeof(int);

    // Copiamos el nombre de la IO
    ioNombre = malloc(tamanio_nombre + 1);
    memcpy(ioNombre, buffer->stream + desplazamiento, tamanio_nombre);
    ioNombre[tamanio_nombre] = '\0';
    desplazamiento += tamanio_nombre;

    //log_info(logger ,"Llego IO a kernel de nombre: %s",ioNombre);

    t_IO *ioBuscada = buscar_io(ioNombre);
    
    if (ioBuscada != NULL)
    {
        //Creo instancia y agrego a estructura ya existente

        t_IO_instancia *nueva_instancia_io= malloc(sizeof(t_IO_instancia));
        nueva_instancia_io->socket = socket;
        nueva_instancia_io->proceso = -1;

        pthread_mutex_lock(&ioBuscada->instancias_IO->mutex);
        list_add_in_index(ioBuscada->instancias_IO->cola,0,nueva_instancia_io);
        pthread_mutex_unlock(&ioBuscada->instancias_IO->mutex);

        log_debug(logger, "Llego una nueva INSTANCIA de IO de nombre %s Y SOCKET: %d ", ioBuscada->nombre,nueva_instancia_io->socket );
        enviar_proceso_io(nueva_instancia_io->socket);
    }
    else
    {
        //Creo estructura de 0, inicializando todo
        t_IO *ioNueva = malloc(sizeof(t_IO));
        ioNueva->nombre = ioNombre;

        ioNueva->instancias_IO = malloc(sizeof(t_monitor));
        pthread_mutex_init(&(ioNueva->instancias_IO->mutex), NULL);
        ioNueva->instancias_IO->cola =list_create();

        ioNueva->procesos_esperando= malloc(sizeof(t_monitor));
        pthread_mutex_init(&(ioNueva->procesos_esperando->mutex), NULL);
        ioNueva->procesos_esperando->cola =list_create();

        //Creo la primera instancia para meter en la estructura nueva
        t_IO_instancia *nueva_instancia_io= malloc(sizeof(t_IO_instancia));
        nueva_instancia_io->socket = socket;
        nueva_instancia_io->proceso = -1;

        pthread_mutex_lock(&ioNueva->instancias_IO->mutex);
        list_add(ioNueva->instancias_IO->cola,nueva_instancia_io);
        pthread_mutex_unlock(&ioNueva->instancias_IO->mutex);

        pthread_mutex_lock(&list_ios->mutex);
        list_add(list_ios->cola,ioNueva);
        pthread_mutex_unlock(&list_ios->mutex);

        log_debug(logger, "Llego una Nueva IO de nombre %s Y SOCKET: %d ", ioNueva->nombre,nueva_instancia_io->socket );
    }   
}

void actualizarIO_a_libre(int pid_desbloqueo) {

    pthread_mutex_lock(&list_ios->mutex); // Lock externo por si list_ios es compartida
    int cant_ios = list_size(list_ios->cola);
    for (int i = 0; i < cant_ios; i++) {

        t_IO *ios = list_get(list_ios->cola, i);

        pthread_mutex_lock(&ios->instancias_IO->mutex); // Lock sobre instancias_IO
        int cant_instancias = list_size(ios->instancias_IO->cola);
        for (int j = 0; j < cant_instancias; j++) {
            
            t_IO_instancia *io = list_get(ios->instancias_IO->cola, j);

            if (io->proceso == pid_desbloqueo) {
                io->proceso = -1;

                pthread_mutex_unlock(&ios->instancias_IO->mutex);
                pthread_mutex_unlock(&list_ios->mutex);
                return;
            }
        }
        
        pthread_mutex_unlock(&ios->instancias_IO->mutex);
    }

    pthread_mutex_unlock(&list_ios->mutex);
}