#include "include/kernel-cpus-connections.h"

void iniciar_cpu(t_buffer *buffer,int socket_cliente)
{
    t_cpu *cpu = cpu_init();

    int tamanio_cpu_id; 
    int desplazamiento = 0;

    //memcpy(&cpu_id, buffer->stream, sizeof(int)); SI SOLO VIENE UN CPU_ID SIN TAMANO

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
    new_cpu->socket_cpu= -1;

    return new_cpu;
}