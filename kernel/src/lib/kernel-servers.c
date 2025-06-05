#include "../include/kernel-servers.h"

void kernel_servers_listening() {
    // Inicializamos los hilos de los servidores:
    pthread_t kernel_server_io;
    pthread_t kernel_server_dispatcher;
    pthread_t kernel_server_interrupt;

    // Creamos los hilos para cada servidor:
    pthread_create(&kernel_server_io, NULL, (void*) kernel_server_io_listening, NULL);
    pthread_create(&kernel_server_dispatcher, NULL, (void*) kernel_server_dispatcher_listening, NULL);
    pthread_create(&kernel_server_interrupt, NULL, (void*) kernel_server_interrupt_listening, NULL);

    pthread_detach(kernel_server_io);
    pthread_detach(kernel_server_dispatcher);
    pthread_detach(kernel_server_interrupt);
}

void* kernel_server_io_listening(void* _) {
    int server_socket = kernel_servers->id_server_io;

    server_with_thread(
        server_socket, 
        "Kernel IO", 
        conection_strategy_once,
        kernel_server_io_handler);

    return NULL;
}

void* kernel_server_dispatcher_listening(void* _) {
    int server_socket = kernel_servers->id_server_dispatch;

    server_with_thread(
        server_socket, 
        "Kernel DISPATCH", 
        conection_strategy_persistence,
        kernel_server_dispatch_handler);
}

void* kernel_server_interrupt_listening(void* _) {
    int server_socket = kernel_servers->id_server_interrupt;

    server_with_thread(
        server_socket, 
        "Kernel INTERRUPT", 
        conection_strategy_persistence, //estrategia persistance?
        kernel_server_interrupt_handler);

    return NULL;
}