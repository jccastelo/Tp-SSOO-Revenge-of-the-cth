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
    
    list_add(list_cpus,cpu);

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

    for(int i = 0; i< list_size(list_cpus) ; i++)
    {   
        t_cpu *cpu =list_get(list_cpus,i);
        socket_actual = cpu->socket_cpu;

        if(socket_actual == cpu_socket_buscado)
        {
            cpu->estado = estado_nuevo;
            return;
        }
    }
    
}