#ifndef MEMORIA_TYPES_H_
#define MEMORIA_TYPES_H_

typedef struct {
    int PUERTO_ESCUCHA;
    int TAM_MEMORIA;
    int TAM_PAGINA;
    int ENTRADAS_POR_TABLA;
    int CANTIDAD_NIVELES;
    int RETARDO_MEMORIA;
    char *PATH_SWAPFILE;
    char *PATH_INSTRUCCIONES;
    int RETARDO_SWAP;
    char* LOG_LEVEL;
    char *DUMP_PATH;
} t_config_memoria;

typedef struct {
    int id_server_escucha_memoria;
} t_memoria_servers;

#endif // MEMORIA_TYPES_H_