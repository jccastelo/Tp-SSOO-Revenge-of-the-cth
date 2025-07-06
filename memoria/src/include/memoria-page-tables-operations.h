#ifndef MEMORIA_PAGE_TABLES_OPERATIONS_H_
#define MEMORIA_PAGE_TABLES_OPERATIONS_H__ 

#include <commons/collections/list.h>
#include <commons/collections/dictionary.h>
#include <stdbool.h>
#include <logger.h> 

#include "memoria-pages-tables.h"

typedef void (*accion_sobre_nodo_t)(t_list* nodo);
#endif // MEMORIA_PAGE_TABLES_OPERATIONS_H_