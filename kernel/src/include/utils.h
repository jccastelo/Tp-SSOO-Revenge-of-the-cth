#ifndef UTILS_H_
#define UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>

typedef struct{
    char* ip_memoria, algoritmo_corto_plazo, algoritmo_ingreso_a_ready, log_level;
    int puerto_memoria, puerto_escucha_dispatch, puerto_escucha_interrupt, puerto_escucha_io, alfa, tiempo_suspension;
}t_config_kernel;

#endif  /*UTILS_H_*/