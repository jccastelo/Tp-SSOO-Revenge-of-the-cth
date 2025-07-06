#ifndef SWAP_FUNTIONS_H_
#define SWAP_FUNTIONS_H_ 

#include <commons/collections/list.h>
#include <commons/collections/dictionary.h>
#include <stdbool.h>
#include <logger.h> 

#include "memoria-pages-tables.h" 

typedef struct {
    int nro_pagina;     
    int offset_swap;    
} swap_entry_t;
t_dictionary* diccionario_swap_metadata; 
FILE* archivo_swap = NULL; 


#endif // SWAP_FUNTIONS_H_