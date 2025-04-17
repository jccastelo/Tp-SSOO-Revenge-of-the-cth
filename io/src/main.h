#ifndef MAIN_H_
#define MAIN_H_

#include "include/utils.h"

void iniciar_logger();

void leer_configuraciones(char* , char*, char*);
    
int crear_conexion(char*, char*, char*); 
    
void escucha_peticiones_kernel();

#endif  /*MAIN_H_*/