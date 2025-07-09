#include "../include/kernel-blocked.h"


void bloquearProceso(t_pcb *process)
{   
    pthread_create(&process->hilo_block, NULL, timer_de_blockeo, (void*) process);
}

void limpiar_hilo_activo(void* arg) {
  t_pcb* process = (t_pcb*) arg;
  process->hilo_activo = false;
}

void liberar_mutex(void* arg) {
    pthread_mutex_t* mutex = (pthread_mutex_t*) arg;
    pthread_mutex_unlock(mutex);
}

void* timer_de_blockeo(void* arg)
{
    
    t_pcb* process = (t_pcb*) arg;

    process->hilo_activo =true;

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    pthread_cleanup_push(limpiar_hilo_activo, (void*)process);

    int esperaDeBLockeo = config_kernel->TIEMPO_SUSPENSION;

    
    usleep(esperaDeBLockeo*1000);
    pthread_mutex_lock(&process->mutex_estado);
    
    pthread_cleanup_push(liberar_mutex, (void*)&process->mutex_estado);

    if (proceso_esta_en_lista(planner->long_term->queue_BLOCKED->cola, process)) {
        queue_process(process, BLOCKED_SUSPENDED);
        
    }
    
    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);
   
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