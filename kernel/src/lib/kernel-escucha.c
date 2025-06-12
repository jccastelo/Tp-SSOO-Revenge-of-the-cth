#include "include/kernel-escucha.h"

void kernel_server_io_handler(int io_socket, int operation, const char *server_name) {

    t_buffer* new_buffer = malloc(sizeof(t_buffer));
    new_buffer->size = 0;
    new_buffer->stream = NULL;

    if((operation != HANDSHAKE))
    {
      new_buffer->stream = recibir_buffer(&new_buffer->size, io_socket);
    }

    if (operation == HANDSHAKE) {
        log_info(logger,"LLego op handssake");
        recibir_handshake(io_socket);
        log_info(logger,"Coonexion IO lista");
    }

    switch(operation){
        case IDENTIFICAR_IO:
            recibir_io(new_buffer, io_socket);
        break;
        case DESBLOQUEO_IO:
            // REcibo el pid (no hace falta paquete)
            int pid_desbloqueo = recibir_pid(new_buffer, io_socket);
            // Reviso la cola de bloqueados de esa IO, si hay alguno lo mando
            enviar_proceso_io(io_socket);
            // Con el pid busco el proceso y lo mando a READY
            t_pcb *process = list_get(list_procesos->cola, pid_desbloqueo);
            queue_process(process, READY);
        break;
        case FIN_CONEXION_DE_IO:
            // Recibo el pid (no hace falta paquete), puede no haber EN ESE CASO IO MANDA -1
            int pid_fin = recibir_pid(new_buffer, io_socket);
        
            //Si habia pid lo mando a exit
            if(pid_fin >= 0){
                t_pcb *process = list_get(list_procesos->cola, pid_fin);
                queue_process(process, EXIT);
            }

            // Borramos la instancia y si era la ultima instancia borramos la estructura general
            eliminar_instancia(io_socket);
        break;
    }
    free(new_buffer);
}

void kernel_server_interrupt_handler(int cpu_socket, int operation, const char *server_name) {
   
    t_buffer* new_buffer = malloc(sizeof(t_buffer));
    new_buffer->size = 0;
    new_buffer->stream = NULL;

    if((operation != HANDSHAKE))
        {
            new_buffer->stream = recibir_buffer(&new_buffer->size, cpu_socket);
        }

      if (operation == HANDSHAKE) {
        log_info(logger,"LLego op handssake");
        recibir_handshake(cpu_socket);
        log_info(logger,"Coonexion interrupt lista");
    } else {
        log_error(logger, "Operación no válida para el servidor INTERRUPT: %d", operation);}

    
    free(new_buffer);
    return;
}

void kernel_server_dispatch_handler(int cpu_socket, int operation, const char *server_name) {

    log_info(logger, "LLEGO OPERACION A KERNEL DISPATCH %d", operation);

    t_buffer* new_buffer = malloc(sizeof(t_buffer));
    new_buffer->size = 0;
    new_buffer->stream = NULL;

    if((operation != HANDSHAKE)) 
        new_buffer->stream = recibir_buffer(&new_buffer->size, cpu_socket);

    if (operation == HANDSHAKE) {
        log_info(logger,"LLego op handssake");
        recibir_handshake(cpu_socket);
        log_info(logger,"Coonexion disptach lista ");
        return;
    }

    
    switch(operation) {
        case CPU_ID:
            log_info(logger, "Llego cpu para identificarse" );
            iniciar_cpu(new_buffer,cpu_socket);
            log_info(logger,"Se recibio la ID de la CPU desde el server %s",server_name);
            break;
        case INIT_PROC:
            recibir_y_crear_proceso(new_buffer);
            log_info(logger,"Se recibio la syscall INIC_PROC desde el server %s",server_name);
            break;
        case DUMP_MEMORY:
            t_pcb* process = recibir_proceso(new_buffer);
            temporal_stop(process->estimaciones_SJF->rafagaReal);

            log_info(logger,"Se recibio la syscall DUMP MEMORY desde el server %s",server_name);
            set_cpu(cpu_socket, DISPONIBLE);
            queue_process(process, BLOCKED);

            if(avisar_dump_memory(process->pid) == 61)
                queue_process(process, READY);
            else
                queue_process(process, EXIT);
            break;
        case IO:
            log_info(logger,"Se recibio la syscall IO desde el server %s",server_name);
            set_cpu(cpu_socket, DISPONIBLE);

            //SE REALIZA EL BLOC ACA, PORQUE EL PID ESTA EN EL BUFFER
            gestionar_io(new_buffer);
            break;
        case EXIT_SYS:
            delate_process(new_buffer);
            set_cpu(cpu_socket, DISPONIBLE);
            log_info(logger,"Se recibio la syscall EXIT_Sys desde el server %s",server_name);
            break;
        default:
            log_error(logger, "Operación no válida para el servidor INTERRUPT: %d", operation);
            break;
    }

    free(new_buffer);
    
    return;
}