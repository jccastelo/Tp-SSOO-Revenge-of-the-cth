#include "../include/kernel-planner.h"

void planner_init(){

    list_procesos = malloc(sizeof(t_monitor));
    pthread_mutex_init(&(list_procesos->mutex), NULL);
    list_procesos->queue_ESTADO = list_create();

    list_ios = malloc(sizeof(t_monitor));
    pthread_mutex_init(list_ios->mutex);
    list_ios->queue_ESTADO =list_create();

    list_cpus = malloc(sizeof(t_monitor));
    pthread_mutex_init(&(list_cpus->mutex), NULL);
    list_cpus->queue_ESTADO = list_create();

    planner = malloc(sizeof(t_planner));

    planner->short_term = malloc(sizeof(t_short_term));
    planner->medium_term = malloc(sizeof(t_medium_term));
    planner->long_term = malloc(sizeof(t_long_term));
    planner->queue_EXECUTE = malloc(sizeof(t_monitor));
    
    pthread_mutex_init(&(planner->queue_EXECUTE->mutex), NULL);
    planner->queue_EXECUTE->queue_ESTADO = list_create();

    switch(get_algoritm(config_kernel->ALGORITMO_INGRESO_A_READY))
    {
    case FIFO:
        planner->long_term->algoritmo_planificador = queue_FIFO;
        break;
    
    case PMCP:
        //planner->long_term->algoritmoPlanificador_long = queue_PMCP();
        break;
    
    default:
        log_info(logger,"Mal ingresado el nombre del algoritmo planificador");
        abort();
    }

    planner->medium_term->algoritmo_planificador = queue_FIFO;

    switch(get_algoritm(config_kernel->ALGORITMO_CORTO_PLAZO))
    {
    case FIFO:
        planner->short_term->algoritmo_planificador = queue_FIFO;
        break;
    
    case SJFsD:
        //planner->long_term->algoritmo_planificador = queue_SJFsD;
        break;

    case SJFcD:
        //planner->long_term->algoritmo_planificador = queue_SJFcD;
        break;
        
    default:
        log_info(logger,"Mal ingresado el nombre del algoritmo planificador");
        abort();
    }

    //SHORT TERM
    planner->short_term->queue_READY = malloc(sizeof(t_monitor));
    pthread_mutex_init(&(planner->short_term->queue_READY->mutex), NULL);
    planner->short_term->queue_READY->queue_ESTADO = list_create();

    //MEDIUM TERM
    planner->medium_term->queue_BLOCKED_SUSPENDED = malloc(sizeof(t_monitor));
    pthread_mutex_init(&(planner->medium_term->queue_BLOCKED_SUSPENDED->mutex), NULL);
    planner->medium_term->queue_BLOCKED_SUSPENDED->queue_ESTADO = list_create();

    planner->medium_term->queue_READY_SUSPENDED = malloc(sizeof(t_monitor));
    pthread_mutex_init(&(planner->medium_term->queue_READY_SUSPENDED->mutex), NULL);
    planner->medium_term->queue_READY_SUSPENDED->queue_ESTADO = list_create();

    //LONG TERM
    planner->long_term->queue_NEW = malloc(sizeof(t_monitor));
    pthread_mutex_init(&(planner->long_term->queue_NEW->mutex), NULL);
    planner->long_term->queue_NEW->queue_ESTADO = list_create();

    planner->long_term->queue_EXIT = malloc(sizeof(t_monitor));
    pthread_mutex_init(&(planner->long_term->queue_EXIT->mutex), NULL);
    planner->long_term->queue_EXIT->queue_ESTADO = list_create();

    planner->long_term->queue_BLOCKED = malloc(sizeof(t_monitor));
    pthread_mutex_init(&(planner->long_term->queue_BLOCKED->mutex), NULL);
    planner->long_term->queue_BLOCKED->queue_ESTADO = list_create();
    

    
   

    
   
    

   

    log_info(logger,"Planificador listo. Esperando para iniciar...");

    getchar();
    
    log_info(logger,"PLANIFICADOR INICIADO");
}   

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

void traer_proceso_a_MP(){
            
    char* respuestaMemoria;

    while(list_size(planner->medium_term->queue_READY_SUSPENDED->queue_ESTADO) > 0){

        respuestaMemoria = memoria_init_proc(list_get(planner->medium_term->queue_READY_SUSPENDED->queue_ESTADO,0));
        // SE USA LA MISMA FUNCION PARA INCIAR PROCESOS EN MP QUE PARA TRAERLOS DE SWAP???
        
        if(strcmp(respuestaMemoria, "OK"))
        {
            queue_process(list_get(planner->medium_term->queue_READY_SUSPENDED->queue_ESTADO,0), READY);
        } else { break; }
    }

    while(list_size(planner->long_term->queue_NEW->queue_ESTADO) > 0){ 

        respuestaMemoria = memoria_init_proc(list_get(planner->long_term->queue_NEW->queue_ESTADO,0));

        if(!strcmp(respuestaMemoria, "OK"))
        {
            queue_process(list_get(planner->long_term->queue_NEW->queue_ESTADO,0), READY);
        } else { break; }
    }
}

void queue_FIFO(t_pcb *process, t_list *lista)
{
    list_add(lista, process);
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