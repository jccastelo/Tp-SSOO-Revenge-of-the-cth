#include "include/kernel-cpus-connections.h"

void iniciar_cpu(t_buffer *buffer,int socket_cliente)
{
    t_cpu *cpu = cpu_init();

    int tamanio_cpu_id; 
    int desplazamiento = 0;

    memcpy(&tamanio_cpu_id, buffer->stream + desplazamiento, sizeof(int));
    desplazamiento += sizeof(int);

    memcpy(&cpu->id, buffer->stream + desplazamiento, tamanio_cpu_id);

    cpu->socket_cpu = socket_cliente;
    
    log_info(logger, "Llego cpu. ID: %d ",cpu->id);
    list_add(list_cpus->queue_ESTADO,cpu);

}

t_cpu *cpu_init(){
    t_cpu *new_cpu = malloc(sizeof(t_cpu));

    new_cpu ->id= -1;
    new_cpu->estado= DISPONIBLE;
    new_cpu->pid= -1;
    new_cpu->socket_cpu= -1;

    return new_cpu;
}

void set_cpu(int cpu_socket_buscado,int estado_nuevo)
{
    int socket_actual = -1;

    for(int i = 0; i< list_size(list_cpus->queue_ESTADO) ; i++)
    {   
        t_cpu *cpu =list_get(list_cpus->queue_ESTADO,i);
        socket_actual = cpu->socket_cpu;

        if(socket_actual == cpu_socket_buscado)
        {
            cpu->estado = estado_nuevo;
            return;
        }
    }
}

t_cpu* buscar_cpu_disponible(){

    for(int i = 0; i< list_size(list_cpus->queue_ESTADO) ; i++)
    {   
        t_cpu* cpu =list_get(list_cpus->queue_ESTADO,i);

        if(cpu->estado == DISPONIBLE)
        {
            log_info(logger,"CPu asignada a proceso");
            return cpu;
        }
    }

    return NULL;
}

void enviar_proceso_cpu(int cpu_socket, t_pcb* process){
    
    t_paquete* paquete = crear_paquete(CONTEXT_PROCESS); // QUE VERGA LE PONGO
    crear_buffer(paquete);

    agregar_a_paquete(paquete, &process->pid, sizeof(int));
    agregar_a_paquete(paquete, &process->pc, sizeof(int));

    enviar_paquete(paquete, cpu_socket);

    eliminar_paquete(paquete);

    set_cpu(cpu_socket,EJECUTANDO);
}

t_pcb* recibir_proceso(t_buffer* buffer){
    
    int pid_recibido;
    memcpy(&pid_recibido, buffer->stream, sizeof(int));
    
    free(buffer->stream);
    free(buffer);

    for(int i = 0; i < list_size(planner->queue_EXECUTE->queue_ESTADO); i++){
        
        t_pcb* process = list_get(planner->queue_EXECUTE->queue_ESTADO, i);
        if(process->pid == pid_recibido)
        {
            return process;
        }
    }

    return NULL;
}