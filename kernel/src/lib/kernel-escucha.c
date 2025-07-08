#include "include/kernel-escucha.h"

void kernel_server_io_handler(int io_socket, int operation, const char *server_name) {

    if (operation == HANDSHAKE) {
        
        recibir_handshake(io_socket);
        log_info(logger,"Conexion IO lista");
        return;
    }

    t_buffer* new_buffer = malloc(sizeof(t_buffer));
    new_buffer->size = 0;
    new_buffer->stream = NULL;

    if((operation != HANDSHAKE))
    {
      new_buffer->stream = recibir_buffer(&new_buffer->size, io_socket);
    }

    switch(operation){
        case IDENTIFICAR_IO:
            log_info(logger,"IO A identificarse");
            recibir_io(new_buffer, io_socket);
        break;
        case DESBLOQUEO_IO:
            // REcibo el pid (no hace falta paquete)
            int pid_desbloqueo = recibir_pid(new_buffer, io_socket);

            actualizarIO_a_libre(pid_desbloqueo);

            // Reviso la cola de bloqueados de esa IO, si hay alguno lo mando
            enviar_proceso_io(io_socket);

            // Con el pid busco el proceso 
            t_pcb *process = list_get(list_procesos->cola, pid_desbloqueo);

            //Si esta en block va a ready, sino a readysuspended
            if(process->queue_ESTADO_ACTUAL->cola == planner->long_term->queue_BLOCKED->cola) 
            {   log_info(logger,"## PID: %d finalizó IO y pasa a READY",process->pid);
                queue_process(process, READY);}

            else {log_info(logger,"## PID: %d finalizó IO y pasa a READY_SUSPENDED",process->pid);
                queue_process(process, READY_SUSPENDED); }
        break;
        case FIN_CONEXION_DE_IO:
            log_info(logger,"Llego IO A desconectarse");
            int pid_fin = recibir_pid(new_buffer, io_socket);
        
            //Si habia pid lo mando a exit
            if(pid_fin >= 0){
                t_pcb *process = list_get(list_procesos->cola, pid_fin);
                queue_process(process, EXIT);
            }

            // Borramos la instancia y si era la ultima instancia borramos la estructura general
            // Si habia procesos esperando, van a exit
            eliminar_instancia(io_socket);
            log_info(logger,"IO desconectada");
        break;
        default:
            log_error(logger, "Operación no válida para el servidor IO: %d", operation);
            break;
    }
    free(new_buffer);
    return;
}

void kernel_server_interrupt_handler(int cpu_socket, int operation, const char *server_name) {
   
    if (operation == HANDSHAKE) {
        
        recibir_handshake(cpu_socket);
        log_info(logger,"Conexion interrupt lista");
        return;
    }

    t_buffer* new_buffer = malloc(sizeof(t_buffer));
    new_buffer->size = 0;
    new_buffer->stream = NULL;

    if((operation != HANDSHAKE))
        {
            new_buffer->stream = recibir_buffer(&new_buffer->size, cpu_socket);
        }

    switch(operation) {
        case CPU_ID:
            log_info(logger, "Llego cpu para identificarse");
            iniciar_cpu(new_buffer,cpu_socket, 0);
            log_info(logger,"Se recibio la ID de la CPU desde el server %s",server_name);
            break;
        default:
            log_error(logger, "Operación no válida para el servidor HANDLER: %d", operation);
            break;
    }

    free(new_buffer);
    return;
}

void kernel_server_dispatch_handler(int cpu_socket, int operation, const char *server_name) {


    if (operation == HANDSHAKE) {
        
        recibir_handshake(cpu_socket);
        log_info(logger,"Coonexion disptach lista ");
        return;
    }

    t_buffer* new_buffer = malloc(sizeof(t_buffer));
    new_buffer->size = 0;
    new_buffer->stream = NULL;

    if((operation != HANDSHAKE)) 
        new_buffer->stream = recibir_buffer(&new_buffer->size, cpu_socket);
    
    switch(operation) {
        case CPU_ID:
            //log_info(logger, "Llego cpu para identificarse" );
            //log_info(logger, "CPU SOCKET %d ", cpu_socket);
            iniciar_cpu(new_buffer,cpu_socket, 1);
            log_info(logger,"Se recibio la ID de la CPU desde el server %s",server_name);
            break;
        case CONTEXTO_DESALOJO:
            t_pcb* proceso = recibir_proceso(new_buffer);
            temporal_stop(proceso->estimaciones_SJF->rafagaReal);

            // No hay que voler a estimar, unicamente hay que restarle el tiempo que estuvo en CPU
            proceso->estimaciones_SJF->rafagaEstimada -= temporal_gettime(proceso->estimaciones_SJF->rafagaReal);

            queue_process(proceso, READY);

            set_cpu(cpu_socket, DISPONIBLE); // Por consigna, la cpu tiene que quedarse esperando
            mandar_procesos_a_execute();
        break;
        case INIT_PROC:
            log_info(logger,"Se recibio la syscall INIC_PROC desde el server %s",server_name);
            recibir_y_crear_proceso(new_buffer);
            break;
        case DUMP_MEMORY:
            t_pcb* process = recibir_proceso(new_buffer);
            temporal_stop(process->estimaciones_SJF->rafagaReal);

            log_info(logger,"Se recibio la syscall DUMP MEMORY desde el server %s",server_name);
            set_cpu(cpu_socket, DISPONIBLE);
            queue_process(process, BLOCKED);
            mandar_procesos_a_execute();

            if(solicitar_a_memoria(avisar_dump_memory, process))
                queue_process(process, READY);
            else
                queue_process(process, EXIT);
            break;
        case IO:
            log_info(logger,"Se recibio la syscall IO desde el server %s",server_name);
            set_cpu(cpu_socket, DISPONIBLE);
            gestionar_io(new_buffer);
            mandar_procesos_a_execute();
            break;
        case EXIT_SYS:
            delate_process(new_buffer);
            set_cpu(cpu_socket, DISPONIBLE);
            log_info(logger,"Se recibio la syscall EXIT_Sys desde el server %s",server_name);
            mandar_procesos_a_execute();
            break;
        default:
            log_error(logger, "Operación no válida para el servidor HANDLER: %d", operation);
            break;
    }

    free(new_buffer);
    
    return;
}