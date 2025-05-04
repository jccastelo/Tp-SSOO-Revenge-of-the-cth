#include "../include/memoria-requests.h"

void init_process (int client_socket) {
    // Inicializamos las variables necesarias para el proceso:
    int id_process;
    int tam_process;
    char *file_procces;
    
    // Llamamos a la función que recibe y configura los valores para el proceso:
    rcv_setup_to_process(client_socket, &id_process, &tam_process, &file_procces);

    // To Do: Deberiamos de validar, si memoria tiene espacio para el proceso

    // Cargar las instrucciones en la estructura administrativa: instrucciones por proceso
    loading_process_instructions(id_process, file_procces);
}