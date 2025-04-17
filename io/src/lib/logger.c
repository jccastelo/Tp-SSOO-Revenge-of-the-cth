#include "../include/logger.h"
t_log* logger;

//POSIBLE MEJORA: char* nombre para el nombre del modulo
void iniciar_logger(void) {
    logger = log_create("../../io.log","LOGGER.io", true, LOG_LEVEL_INFO);

    if(logger == NULL){
        abort();
    }
    
}