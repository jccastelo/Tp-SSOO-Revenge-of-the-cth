#include <check.h>
#include <math.h>


#include "../include/memoria-protocols.h"

void rcv_setup_to_process(int client_socket, int *id_process, int *tam_process, char **file_procces) {
    // Inicializamos las variables necesarias:
    int size; 
    int desplazamiento = 0;  
    void *buffer = recibir_buffer(&size, client_socket);  

    // Parseamos los primeros valores enteros del buffer, actualizando el desplazamiento en cada paso:
    parsear_int(buffer, &desplazamiento, id_process);
    parsear_int(buffer, &desplazamiento, tam_process); 

    // Parseamos las cadenas de caracteres contenidas en el buffer y las almacenamos en una lista:
    parsear_string(buffer, &desplazamiento, file_procces);
}


void parsear_int(void* buffer ,int* desplazamiento,int* ubicacion ){
    memcpy(ubicacion, buffer + *desplazamiento, sizeof(int));
    *desplazamiento += sizeof(int);
}
void parsear_string(void *buffer, int *desplazamiento, char **destino) {
    int longitud;

    memcpy(&longitud, buffer + *desplazamiento, sizeof(int));
    *desplazamiento += sizeof(int);

    *destino = malloc(longitud + 1);
    if (*destino == NULL) {
        perror("Error al asignar memoria para la cadena");
        exit(1);
    }

    memcpy(*destino, buffer + *desplazamiento, longitud);
    (*destino)[longitud] = '\0';

    *desplazamiento += longitud;
}
