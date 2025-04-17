#include "../include/logger.h"
t_log* logger;

//POSIBLE MEJORA: char* nombre para el nombre del modulo
void iniciar_logger(char* LOG_level) {
    logger = log_create("../../kernel.log","LOGGER.Kernel", true, LOG_level);

    if(logger == NULL)
        abort();
    
    log_info(logger, "Se ha inicializado el logger de Kernel");
}