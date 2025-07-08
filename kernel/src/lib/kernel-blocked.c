#include "../include/kernel-blocked.h"
t_pcb* procesoBuscado;

void bloquearProceso(t_pcb *process)
{   
    pthread_t blockearProceso;

    pthread_create(&blockearProceso, NULL, (void*) timer_de_blockeo,(void*) process);
   
    pthread_detach(blockearProceso);

}

void* timer_de_blockeo(void* arg)
{
    //COMPARAR METRICAS DE BLOCK PARA VER SI SIGUE AHI
    t_pcb* process = (t_pcb*) arg;

    int esperaDeBLockeo = config_kernel->TIEMPO_SUSPENSION;

    usleep(esperaDeBLockeo*1000);
    procesoBuscado = process;
    if(list_any_satisfy(planner->long_term->queue_BLOCKED->cola,condicionProcesoEnLista))
    {
        queue_process(process,BLOCKED_SUSPENDED);
    }

    return NULL;

}

bool condicionProcesoEnLista(void* elemento)
{
    t_pcb* pcb = (t_pcb*) elemento;
    return pcb->pid == procesoBuscado->pid;
}