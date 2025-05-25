#include "../include/io-finalizacion.h"

void* cierre_de_io(){

    t_paquete* paquete = crear_paquete(FIN_CONEXION_DE_IO);
    enviar_paquete(paquete, socket_kernel);
    eliminar_paquete(paquete);
    return NULL;
}