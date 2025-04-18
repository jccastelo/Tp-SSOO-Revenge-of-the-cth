#include "../include/kernel-servers.h"


void kernel_servers_oid(){
    while(1) {
        
        ptheread_t hilo_conexion;

    }
}

void kernel_server_oid_io() {
    while(1) {
        
        int cliente_socket = esperar_cliente("IO",kernel_servers->id_server_io);
        
        
        ptheread_t hilo_conexion;
        hilo_conexion = pthread_create(&hilo_conexion, NULL, (void*) kernel_peticiones_io, cliente_socket);

    }
}

void kernel_peticiones_io(void *args) {
    int cliente_io = (int) args;
    
    recibir_handshake(cliente_io);
    recibir_interfaz(cliente_io);  
}