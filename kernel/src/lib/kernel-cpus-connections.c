#include "include/kernel-cpus-connections.h"

void iniciar_cpu(t_buffer *buffer,int socket_cliente, int dispatch_o_interrupt)
{
    int id;
    memcpy(&id, buffer->stream, sizeof(int));

    t_cpu* cpu = buscar_cpu_con_id(id);

    if(cpu != NULL){

        if(dispatch_o_interrupt == 1) { // 1 dispatch - 0 interrupt
            memcpy(&cpu->socket_dispatch, &socket_cliente, sizeof(int));
        } else {
            memcpy(&cpu->socket_interrupt, &socket_cliente, sizeof(int));            
        }

    } else {
        
        t_cpu *cpu = cpu_init();

        cpu->id = id;

        if(dispatch_o_interrupt == 1) {
            memcpy(&cpu->socket_dispatch, &socket_cliente, sizeof(int));
        } else {
            memcpy(&cpu->socket_interrupt, &socket_cliente, sizeof(int));            
        }

        pthread_mutex_lock(&list_cpus->mutex);
        list_add(list_cpus->cola, cpu);
        pthread_mutex_unlock(&list_cpus->mutex);
    }
}

t_cpu *cpu_init(){
    t_cpu *new_cpu = malloc(sizeof(t_cpu));

    new_cpu ->id = -1;
    new_cpu->estado = DISPONIBLE;
    new_cpu->pid = -1;
    new_cpu->socket_interrupt = -1;
    new_cpu->socket_dispatch = -1;

    return new_cpu;
}

void set_cpu(int cpu_socket_buscado,int estado_nuevo,int pid_ejecutando)
{
    int socket_actual = -1;

    pthread_mutex_lock(&list_cpus->mutex);
    int cantidad_cpus = list_size(list_cpus->cola);
    pthread_mutex_unlock(&list_cpus->mutex);
    for(int i = 0; i < cantidad_cpus ; i++)
    {   
        pthread_mutex_lock(&list_cpus->mutex);
        t_cpu *cpu =list_get(list_cpus->cola,i);
        pthread_mutex_unlock(&list_cpus->mutex);

        socket_actual = cpu->socket_dispatch;

        if(socket_actual == cpu_socket_buscado)
        {
            cpu->estado = estado_nuevo;
            cpu->pid =pid_ejecutando;
            return;
        }
    }
}

t_cpu* buscar_cpu_disponible(){

    pthread_mutex_lock(&list_cpus->mutex);
    int cantidad_cpus = list_size(list_cpus->cola);
    pthread_mutex_unlock(&list_cpus->mutex);
    for(int i = 0; i < cantidad_cpus ; i++)
    {   
        pthread_mutex_lock(&list_cpus->mutex);
        t_cpu *cpu =list_get(list_cpus->cola,i);
        pthread_mutex_unlock(&list_cpus->mutex);

        if(cpu->estado == DISPONIBLE)
        {
            return cpu;
        }
    }

    return NULL;
}

t_cpu* buscar_cpu_con_id(int id){
    
    pthread_mutex_lock(&list_cpus->mutex);
    int cantidad_cpus = list_size(list_cpus->cola);
    pthread_mutex_unlock(&list_cpus->mutex);
    for(int i = 0; i < cantidad_cpus ; i++)
    {   
        pthread_mutex_lock(&list_cpus->mutex);
        t_cpu *cpu =list_get(list_cpus->cola,i);
        pthread_mutex_unlock(&list_cpus->mutex);

        if(cpu->id == id)
        {
            return cpu;
        }
    }

    return NULL;
}

void enviar_proceso_cpu(int cpu_socket, t_pcb* process){
    
    t_paquete* paquete = crear_paquete(CONTEXT_PROCESS); 

    agregar_a_paquete(paquete, &process->pid, sizeof(int));
    agregar_a_paquete(paquete, &process->pc, sizeof(int));

    log_info(logger," SE ENVIO PID  %d,  CON PC %d", process->pid,process->pc);
    enviar_paquete(paquete, cpu_socket);

    eliminar_paquete(paquete);

    set_cpu(cpu_socket, EJECUTANDO,process->pid);
}

void desalojar_proceso(t_cpu* cpu){
    
    t_paquete* paquete = crear_paquete(INTERRUPT); // NO SE SI ESTE CODIGO DE OPERACION ESTA BIEN
    
    enviar_paquete(paquete, cpu->socket_interrupt);
}

t_pcb* recibir_proceso(t_buffer* buffer){
    
    int pid_recibido;
    memcpy(&pid_recibido, buffer->stream, sizeof(int));

    int pc_recibido;
    memcpy(&pc_recibido, buffer->stream + sizeof(int), sizeof(int));
    
    free(buffer->stream);
    free(buffer);

    pthread_mutex_lock(&list_procesos->mutex);
    t_pcb *proceso_buscado = list_get(list_procesos->cola,pid_recibido);
    pthread_mutex_unlock(&list_procesos->mutex);

    proceso_buscado->pc = pc_recibido;

    return proceso_buscado;
}