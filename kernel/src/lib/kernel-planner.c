#include "../include/kernel-planner.h"

pthread_mutex_t mutex_traer = PTHREAD_MUTEX_INITIALIZER;

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
    
    log_debug(logger,"PLANIFICADOR INICIADO");
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

    else{log_error(logger,"BOOOM");
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
    free(new_buffer);
}

void traer_proceso_a_MP(){
    pthread_mutex_lock(&mutex_traer);

    while(!list_is_empty(planner->medium_term->queue_READY_SUSPENDED->cola)){
        
        pthread_mutex_lock(&planner->medium_term->queue_READY_SUSPENDED->mutex);
        t_pcb *procesoASolicitar = list_get(planner->medium_term->queue_READY_SUSPENDED->cola,0);
        

        if(solicitar_a_memoria(desuspender_proceso,procesoASolicitar))
        {
            
            pthread_mutex_unlock(&planner->medium_term->queue_READY_SUSPENDED->mutex);

            queue_process(procesoASolicitar, READY);
            
        } else { 
            pthread_mutex_unlock(&planner->medium_term->queue_READY_SUSPENDED->mutex);
            break; }

    }

    while(!list_is_empty(planner->long_term->queue_NEW->cola)){ 

        pthread_mutex_lock(&planner->long_term->queue_NEW->mutex);
        t_pcb *procesoASolicitar = list_get(planner->long_term->queue_NEW->cola,0);
        

        if(solicitar_a_memoria(memoria_init_proc,procesoASolicitar))
        {
           
            pthread_mutex_unlock(&planner->long_term->queue_NEW->mutex);
            queue_process(procesoASolicitar, READY);

        } else { 
            pthread_mutex_unlock(&planner->long_term->queue_NEW->mutex);
            break; }
    }
    log_debug(logger,"NO rompi acaAAAAAAA");
    pthread_mutex_unlock(&mutex_traer);
    return;
}

void mandar_procesos_a_execute()
{   
    pthread_mutex_lock(&mutex_traer);
    while(!list_is_empty(planner->short_term->queue_READY->cola))
    {
         pthread_mutex_lock(&planner->short_term->queue_READY->mutex);
         t_pcb* procesoPrimero = list_get(planner->short_term->queue_READY->cola,0);
         
        pthread_mutex_lock(&mutex_cpu); 

        t_cpu* cpu_disponible= buscar_cpu_disponible();
        if(cpu_disponible != NULL)
        {
            set_cpu(cpu_disponible->socket_dispatch,EJECUTANDO,procesoPrimero->pid);
        }
        
        pthread_mutex_unlock(&mutex_cpu); 

        if(cpu_disponible != NULL)
        {
            
            pthread_mutex_unlock(&planner->short_term->queue_READY->mutex);

            queue_process(procesoPrimero, EXECUTE);
        } else { 
            pthread_mutex_unlock(&planner->short_term->queue_READY->mutex);
            break; }
    }
    pthread_mutex_unlock(&mutex_traer);
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
        int tamProcesoIt = procesoIterado->tamanio_proceso;

        if(tamano_proceso_nuevo < tamProcesoIt)//Soy menor o igual que el que esta aca?
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
    
    //log_warning(logger, "A");
    t_cpu* cpu = cpu_mayor_rafaga();
    //log_warning(logger, "B");

    if(cpu == NULL)
    {
        log_error(logger,"CPU NULL");
        return;
    }

    if(cpu->estado == DISPONIBLE) {
        log_debug(logger, "Se libero una CPU en medio del desalojo");
        return;
    }
    
    //log_warning(logger, "C");


    t_pcb* proceso_cpu = NULL;

    //log_warning(logger, "D");

    if(cpu->pid >=0 && cpu->pid < 400){
        pthread_mutex_lock(&list_procesos->mutex);    
        //log_warning(logger, "E");
        proceso_cpu = list_get(list_procesos->cola, cpu->pid);
        //log_warning(logger, "F");
        pthread_mutex_unlock(&list_procesos->mutex);
    }
    else{log_error(logger, "SE QUISO ACCEDER A PID NEGATIVO");
        return;}

    //log_warning(logger, "G");

    int64_t tiempo = temporal_gettime(proceso_cpu->estimaciones_SJF->rafagaReal);
    int64_t restante = max(proceso_cpu->estimaciones_SJF->rafagaEstimada - tiempo,(int64_t)0);

    if (restante > primer_proceso->estimaciones_SJF->rafagaEstimada) {
        //log_warning(logger, "H");
        desalojar_proceso(cpu);
        //log_warning(logger, "I");
        log_debug(logger, "DESALOJO PAPUI");
        return;
    }

    return;
}

t_cpu* cpu_mayor_rafaga() {
    
    pthread_mutex_lock(&mutex_cpu);
    //log_warning(logger, "J");
    pthread_mutex_lock(&list_cpus->mutex);
    //log_warning(logger, "K");

    pthread_mutex_lock(&list_procesos->mutex);
    int cantidad_procesos = list_size(list_procesos->cola);
    pthread_mutex_unlock(&list_procesos->mutex);

    t_cpu* cpu_buscada=NULL;
    
    //log_warning(logger, "L");
    int tamanio = list_size(list_cpus->cola);
    

    //log_warning(logger, "M");
    cpu_buscada = list_get(list_cpus->cola, 0);
    

    //log_warning(logger, "N");
    if(cpu_buscada->estado == DISPONIBLE){     
        pthread_mutex_unlock(&list_cpus->mutex);
        pthread_mutex_unlock(&mutex_cpu);
        //log_warning(logger, "O");
        return cpu_buscada;
    }

    for(int i = 1; i < tamanio; i++) {
        
        //log_warning(logger, "P");
        t_cpu* cpu_i = list_get(list_cpus->cola, i);

        if(cpu_i ==NULL)
        {
             pthread_mutex_unlock(&list_cpus->mutex);
            pthread_mutex_unlock(&mutex_cpu);
            log_error(logger, "CPU NULL TOMADA");
            return NULL;
        }
        
        //log_warning(logger, "Q");
        if(cpu_i->estado == DISPONIBLE){    
            pthread_mutex_unlock(&list_cpus->mutex);
            pthread_mutex_unlock(&mutex_cpu);
            //log_warning(logger, "R");
            return cpu_i;
        }
        
        //log_warning(logger, "S");
        t_pcb* proceso_a = NULL;
        if(cpu_buscada->pid >= 0 && cpu_buscada->pid < cantidad_procesos){
            log_warning(logger, "T");
            proceso_a = list_get(list_procesos->cola, cpu_buscada->pid);
        
        }
        else{log_error(logger, "SE QUISO ACCEDER A PID NEGATIVO");}

        //log_warning(logger, "U");
        t_pcb* proceso_b = NULL;
        if(cpu_i->pid >= 0 && cpu_i->pid < cantidad_procesos){
            log_warning(logger, "V");
            proceso_b = list_get(list_procesos->cola, cpu_i->pid);
        }
        else{log_error(logger, "SE QUISO ACCEDER A PID NEGATIVO");}
       
        //log_warning(logger, "W");

        if(proceso_a ==NULL || proceso_b == NULL){
            pthread_mutex_unlock(&list_cpus->mutex);
            pthread_mutex_unlock(&mutex_cpu);
            log_error(logger,"ALGUN PROCESO NULL");
            return NULL; }

        if(proceso_a->pid < 0 || proceso_a->pid > cantidad_procesos
         || proceso_b->pid > cantidad_procesos || proceso_b->pid <0 ){
            pthread_mutex_unlock(&list_cpus->mutex);
            pthread_mutex_unlock(&mutex_cpu);
            log_error(logger,"PID: %d", proceso_a->pid );
            log_error(logger,"PID: %d", proceso_b->pid );
            log_error(logger,"ALGUN PROCESO con PID INVALIDO");
            return NULL; 
        }

        //log_error(logger,"PID: %d", proceso_a->pid );
        //log_error(logger," Son  %"PRId64,proceso_a->estimaciones_SJF->rafagaEstimada );

        //log_error(logger,"PID: %d", proceso_b->pid );
        //log_error(logger,"Son  %"PRId64,proceso_b->estimaciones_SJF->rafagaEstimada );

        if(proceso_a->estimaciones_SJF->rafagaEstimada < proceso_b->estimaciones_SJF->rafagaEstimada 
            && cpu_i->pid >= 0 && cpu_i->pid < cantidad_procesos && cpu_buscada->pid < cantidad_procesos && cpu_buscada->pid >=0 ) {

                //log_warning(logger, "X");
                cpu_buscada = cpu_i;
            }
        
    }
    //log_warning(logger, "Y");
    pthread_mutex_unlock(&list_cpus->mutex);
    //log_warning(logger, "Z");
    pthread_mutex_unlock(&mutex_cpu);
    return cpu_buscada;
}
//void* cpu_mayor_rafaga(void* unaCPU, void* otraCPU) {
//    
//    t_cpu* cpu_a = (t_cpu*) unaCPU;
//    t_cpu* cpu_b = (t_cpu*) otraCPU;
//   
//    if(cpu_a->estado == DISPONIBLE){
//        return unaCPU;
//    }
//
//    if(cpu_b->estado == DISPONIBLE){
//        return otraCPU;
//    }
//
//    pthread_mutex_lock(&list_procesos->mutex);
//    t_pcb* proceso_a = list_get(list_procesos->cola, cpu_a->pid);
//    t_pcb* proceso_b = list_get(list_procesos->cola, cpu_b->pid);
//    pthread_mutex_unlock(&list_procesos->mutex);
//
//    return (void*) proceso_a->estimaciones_SJF->rafagaEstimada <= (void*) proceso_b->estimaciones_SJF->rafagaEstimada ? (void*) proceso_a->estimaciones_SJF->rafagaEstimada : (void*) proceso_b->estimaciones_SJF->rafagaEstimada;
//}
