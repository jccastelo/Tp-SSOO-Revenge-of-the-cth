#include "../include/kernel-blocked.h"


void bloquearProceso(t_pcb *process)
{   
    pthread_create(&process->hilo_block, NULL, timer_de_blockeo, (void*) process);
}

void* timer_de_blockeo(void* arg)
{
    //COMPARAR METRICAS DE BLOCK PARA VER SI SIGUE AHI
    t_pcb* process = (t_pcb*) arg;

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    int esperaDeBLockeo = config_kernel->TIEMPO_SUSPENSION;

    usleep(esperaDeBLockeo*1000);
    
    if (proceso_esta_en_lista(planner->long_term->queue_BLOCKED->cola, process)) {
        queue_process(process, BLOCKED_SUSPENDED);
    }

    return NULL;

}

bool proceso_esta_en_lista(t_list* lista, t_pcb* proceso) {

    for (int i = 0; i < list_size(lista); i++) {
        t_pcb* pcb = list_get(lista, i);
        
        if (pcb->pid == proceso->pid) {
            return true;
        }
    }
    return false;
}