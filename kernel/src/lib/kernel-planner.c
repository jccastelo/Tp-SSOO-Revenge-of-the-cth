#include "../include/kernel-planner.h"

void planner_init(){

    list_procesos = malloc(sizeof(t_monitor));
    pthread_mutex_init(&(list_procesos->mutex), NULL);
    list_procesos->cola = list_create();

    list_cpus = malloc(sizeof(t_monitor));
    pthread_mutex_init(&(list_cpus->mutex), NULL);
    list_cpus->cola = list_create();

    //Lista IOS
    list_ios = malloc(sizeof(t_monitor));
    pthread_mutex_init(&(list_ios->mutex), NULL);
    list_ios->cola =list_create();


    //Planner
    planner = malloc(sizeof(t_planner));

    planner->short_term = malloc(sizeof(t_short_term));
    planner->medium_term = malloc(sizeof(t_medium_term));
    planner->long_term = malloc(sizeof(t_long_term));
    planner->queue_EXECUTE = malloc(sizeof(t_monitor));
    
    pthread_mutex_init(&(planner->queue_EXECUTE->mutex), NULL);
    planner->queue_EXECUTE->cola = list_create();

    switch(get_algoritm(config_kernel->ALGORITMO_INGRESO_A_READY))
    {
    case FIFO:
        planner->long_term->algoritmo_planificador = queue_FIFO;
        planner->medium_term->algoritmo_planificador = queue_FIFO;
        break;
    
    case PMCP:
        planner->long_term->algoritmo_planificador = queue_PMCP;
        planner->medium_term->algoritmo_planificador = queue_PMCP;
        break;
    
    default:
        log_error(logger,"Mal ingresado el nombre del algoritmo planificador");
        abort();
    }

    switch(get_algoritm(config_kernel->ALGORITMO_CORTO_PLAZO))
    {
    case FIFO:
        planner->short_term->algoritmo_planificador = queue_FIFO;
        planner->short_term->algoritmo_desalojo = sin_desalojo;
        break;
    
    case SJFsD:
        planner->short_term->algoritmo_planificador = queue_SJF;
        planner->short_term->algoritmo_desalojo = sin_desalojo;
        break;

    case SJFcD:
        planner->short_term->algoritmo_planificador = queue_SJF;
        planner->short_term->algoritmo_desalojo = desalojo_SJF;
        break;
        
    default:
        log_error(logger,"Mal ingresado el nombre del algoritmo planificador");
        abort();
    }

    //SHORT TERM
    planner->short_term->queue_READY = malloc(sizeof(t_monitor));
    pthread_mutex_init(&(planner->short_term->queue_READY->mutex), NULL);
    planner->short_term->queue_READY->cola = list_create();

    //MEDIUM TERM
    planner->medium_term->queue_BLOCKED_SUSPENDED = malloc(sizeof(t_monitor));
    pthread_mutex_init(&(planner->medium_term->queue_BLOCKED_SUSPENDED->mutex), NULL);
    planner->medium_term->queue_BLOCKED_SUSPENDED->cola = list_create();

    planner->medium_term->queue_READY_SUSPENDED = malloc(sizeof(t_monitor));
    pthread_mutex_init(&(planner->medium_term->queue_READY_SUSPENDED->mutex), NULL);
    planner->medium_term->queue_READY_SUSPENDED->cola = list_create();

    //LONG TERM
    planner->long_term->queue_NEW = malloc(sizeof(t_monitor));
    pthread_mutex_init(&(planner->long_term->queue_NEW->mutex), NULL);
    planner->long_term->queue_NEW->cola = list_create();

    planner->long_term->queue_EXIT = malloc(sizeof(t_monitor));
    pthread_mutex_init(&(planner->long_term->queue_EXIT->mutex), NULL);
    planner->long_term->queue_EXIT->cola = list_create();

    planner->long_term->queue_BLOCKED = malloc(sizeof(t_monitor));
    pthread_mutex_init(&(planner->long_term->queue_BLOCKED->mutex), NULL);
    planner->long_term->queue_BLOCKED->cola = list_create();
    
    log_info(logger,"PLANIFICADOR INICIADO");
}   

// Funciones del planificador

int get_algoritm(char *algoritm_name)
{
    if(!strcmp("FIFO",algoritm_name))
    {return FIFO;}

    else if(!strcmp("PMCP",algoritm_name))
    {return PMCP;}

    else if(!strcmp("SJFsD",algoritm_name))
    {return SJFsD;}

    else if(!strcmp("SJFcD",algoritm_name))
    {return SJFcD;}

    else{log_info(logger,"BOOOM");
    return -1;}

}

void init_fist_process(char *archivo_pseudocodigo,int Tamanio_proc){

    //INICIO BUFFER
    t_buffer* new_buffer = malloc(sizeof(t_buffer));
    new_buffer->size = 0;
    new_buffer->stream = NULL;

    // Inicializamos los datos del buffer
    int tamanio_nombre_archivo= strlen(archivo_pseudocodigo);
    int total_size = sizeof(int) + tamanio_nombre_archivo + sizeof(int); // tamaño + mensaje + Tamanio_prco

    new_buffer->stream = malloc(total_size);
    new_buffer->size = total_size;

    int desplazamiento = 0;

    // Copiar el tamaño del archivo_pseudocodigo
    memcpy(new_buffer->stream + desplazamiento, &tamanio_nombre_archivo, sizeof(int));
    desplazamiento += sizeof(int);

    // Copiar el mensaje
    memcpy(new_buffer->stream + desplazamiento, archivo_pseudocodigo, tamanio_nombre_archivo);
    desplazamiento += tamanio_nombre_archivo;

    // Copiar el Tamanio_proc
    memcpy(new_buffer->stream + desplazamiento, &Tamanio_proc, sizeof(int));
    desplazamiento += sizeof(int);

    //Llamada de la syscall INIC_PROC
    recibir_y_crear_proceso(new_buffer);
}

void traer_proceso_a_MP(){


    while(!list_is_empty(planner->medium_term->queue_READY_SUSPENDED->cola)){
     
        if(solicitar_a_memoria(desuspender_proceso, list_get(planner->medium_term->queue_READY_SUSPENDED->cola,0)))
        {
            pthread_mutex_lock(&planner->medium_term->queue_READY_SUSPENDED->mutex);
            t_pcb* procesoAux = list_remove(planner->medium_term->queue_READY_SUSPENDED->cola,0);
            pthread_mutex_unlock(&planner->medium_term->queue_READY_SUSPENDED->mutex);

            queue_process(procesoAux, READY);
            
        } else { break; }

    }

    while(!list_is_empty(planner->long_term->queue_NEW->cola)){ 

        if(solicitar_a_memoria(memoria_init_proc, list_get(planner->long_term->queue_NEW->cola,0)))
        {
            pthread_mutex_lock(&planner->long_term->queue_NEW->mutex);
            t_pcb* procesoAux = list_remove(planner->long_term->queue_NEW->cola,0);
            pthread_mutex_unlock(&planner->long_term->queue_NEW->mutex);

            queue_process(procesoAux, READY);
        } else { break; }
    }

    return;
}

void mandar_procesos_a_execute()
{
    while(!list_is_empty(planner->short_term->queue_READY->cola))
    {
        if(buscar_cpu_disponible() != NULL)
        {
            pthread_mutex_lock(&planner->short_term->queue_READY->mutex);
            t_pcb* procesoAux = list_remove(planner->short_term->queue_READY->cola,0);
            pthread_mutex_unlock(&planner->short_term->queue_READY->mutex);

            queue_process(procesoAux, EXECUTE);
        } else { break; }
    }

    return;
}

//

// queue_X

void queue_FIFO(t_pcb *process, t_list *lista)
{
    list_add(lista, process);
}

void queue_PMCP(t_pcb *process, t_list *lista)
{
    int tamano_proceso_nuevo = process->tamanio_proceso;

            
    if( list_size(lista)==0)//Lista vacia?
    {
        list_add(lista,process);
        return;
    }

    int i =0;
    int tamano_Lista_Foto = list_size(lista);

    while (i < tamano_Lista_Foto) 
    {

        t_pcb *procesoIterado = list_get(lista,i);
        int tamProcesoIt = procesoIterado -> tamanio_proceso;

        if(tamano_proceso_nuevo <= tamProcesoIt)//Soy menor o igual que el que esta aca?
        {
            list_add_in_index(lista,i,process);
            return;
        }

        i++;
    }

    list_add(lista,process); //AL final si no entr en ningun lado
}


void queue_SJF(t_pcb *process, t_list *lista) {
    
    if(process->estimaciones_SJF->rafagaEstimada == 0) {

        process->estimaciones_SJF->ultimaEstimacion = process->estimaciones_SJF->ultimaEstimacion * (1-config_kernel->ALFA) + process->estimaciones_SJF->rafagaTotalReal * config_kernel->ALFA;
        process->estimaciones_SJF->rafagaEstimada = process->estimaciones_SJF->ultimaEstimacion; 
        process->estimaciones_SJF->rafagaTotalReal = 0;
    }

    int tamanio_actual = list_size(lista);
    if(tamanio_actual == 0) //Lista vacia?
    {
        list_add(lista,process);
        return;
    }

    for(int i = 0; i < tamanio_actual; i++) {
        t_pcb* proceso_a_desplazar = list_get(lista,i);

        if(process->estimaciones_SJF->rafagaEstimada < proceso_a_desplazar->estimaciones_SJF->rafagaEstimada) {
            list_add_in_index(lista,i,process);
            return;
        } 
    }

    list_add(lista,process); //Al final si no entr en ningun lado
    return;
}

void actualizar_rafagas_sjf(t_pcb* proceso){

    int64_t tiempoEnCPU = temporal_gettime(proceso->estimaciones_SJF->rafagaReal);
    proceso->estimaciones_SJF->rafagaEstimada = max((int64_t) 0, proceso->estimaciones_SJF->rafagaEstimada - tiempoEnCPU);
    proceso->estimaciones_SJF->rafagaTotalReal += tiempoEnCPU;
}

int64_t max(int64_t a, int64_t b) {
    return (a > b) ? a : b;
}

// desalojo_X

void sin_desalojo(t_pcb* primer_proceso) {} // null patern nos permite simplificar el codigo en cambios-de-estado eliminando la verificacion de algoritmo_desalojo!=NULL

void desalojo_SJF(t_pcb* primer_proceso) {
    
    pthread_mutex_lock(&list_cpus->mutex);
    t_cpu* cpu = list_get_maximum(list_cpus->cola, cpu_mayor_rafaga);
    pthread_mutex_unlock(&list_cpus->mutex);

    pthread_mutex_lock(&list_procesos->mutex);    
    t_pcb* proceso_cpu = list_get(list_procesos->cola, cpu->pid);
    pthread_mutex_unlock(&list_procesos->mutex);

    int64_t tiempo = temporal_gettime(proceso_cpu->estimaciones_SJF->rafagaReal);
    int64_t restante = max(proceso_cpu->estimaciones_SJF->rafagaEstimada - tiempo,(int64_t)0);

    log_warning(logger, "TIEMPO CPU: %"PRId64, tiempo);
    log_warning(logger, "RAFAGA RESTANTE: %" PRId64, proceso_cpu->estimaciones_SJF->rafagaEstimada);
    log_warning(logger, "RAFAGA NUEVA   : %" PRId64, primer_proceso->estimaciones_SJF->rafagaEstimada);

    if (restante > primer_proceso->estimaciones_SJF->rafagaEstimada) {
        desalojar_proceso(cpu);
        return;
    }

    return;
}

void* cpu_mayor_rafaga(void* unaCPU, void* otraCPU) {
    
    t_cpu* cpu_a = (t_cpu*) unaCPU;
    t_cpu* cpu_b = (t_cpu*) otraCPU;
   
    pthread_mutex_lock(&list_procesos->mutex);
    t_pcb* proceso_a = list_get(list_procesos->cola, cpu_a->pid);
    t_pcb* proceso_b = list_get(list_procesos->cola, cpu_b->pid);
    pthread_mutex_unlock(&list_procesos->mutex);
    
    return (void*) proceso_a->estimaciones_SJF->rafagaEstimada <= (void*) proceso_b->estimaciones_SJF->rafagaEstimada ? (void*) proceso_a->estimaciones_SJF->rafagaEstimada : (void*) proceso_b->estimaciones_SJF->rafagaEstimada;
}
