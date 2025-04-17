#include "../include/logger.h"

//POSIBLE MEJORA: char* nombre para el nombre del modulo
t_log* iniciar_logger(void) {
    t_log* nuevo_logger = log_create("../../io.log","LOGGER.io", true, LOG_LEVEL_INFO);

    if(nuevo_logger == NULL){
        abort();
    }
    
}