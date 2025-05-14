#include "include/kernel-escucha.h"

void kernel_server_io_handler(int client_socket, int operation, const char *server_name) {
    if (operation == HANDSHAKE) {
        recibir_handshake(client_socket);
    } else 
        log_error(logger, "Operación no válida para el servidor IO: %d", operation);
    
    return;
}

void kernel_server_interrupt_handler(int client_socket, int operation, const char *server_name) {
    if (operation == HANDSHAKE) {
        recibir_handshake(client_socket);
    } else 
        log_error(logger, "Operación no válida para el servidor Interrupt: %d", operation);
    
    return;
}

void kernel_server_dispatch_handler(int cpu_socket, int operation, const char *server_name) {

    t_buffer* new_buffer = malloc(sizeof(t_buffer));
    new_buffer->size = 0;
    new_buffer->stream = NULL;

    new_buffer->stream = recibir_buffer(&new_buffer->size, cpu_socket);

    switch(operation)
    {
    // Operaciones
    case HANDSHAKE:
        recibir_handshake(cpu_socket);
        break;

    case CPU_ID:
        iniciar_cpu(new_buffer,cpu_socket);
        log_info(logger,"Se recibio la ID de la CPU desde el server %s",server_name);
        break;

    // Syscalls
    case INIT_PROC:
        recibir_y_crear_proceso(new_buffer);
        log_info(logger,"Se recibio la syscall INIC_PROC desde el server %s",server_name);
        break;

    case DUMP_MEMORY:
        t_pcb* process = recibir_proceso(new_buffer);
        
        queue_process(process, BLOCKED);

        set_cpu(cpu_socket, DISPONIBLE);

        //avisar_dump_memory(process->pid);
       
        enviar_proceso_cpu(cpu_socket, list_get(planner->short_term->queue_READY->queue_ESTADO, 0)); 
        

        log_info(logger,"Se recibio la syscall INIC_PROC desde el server %s",server_name);
        free(process);
        break;

    case IO:
        process = recibir_proceso(new_buffer);
        queue_process(process, BLOCKED);


        // como se acaba de liberar una CPU, enviamos el primer proceso de READY a EXECUTE 
        enviar_proceso_cpu(cpu_socket, list_get(planner->short_term->queue_READY->queue_ESTADO, 0));
         
       


        log_info(logger,"Se recibio la syscall IO desde el server %s",server_name);
        free(process);
        break;

    case EXIT_Sys:
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
