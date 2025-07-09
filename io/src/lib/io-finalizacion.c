#include "../include/io-finalizacion.h"

void cierre_de_io(){

    t_paquete* paquete = crear_paquete(FIN_CONEXION_DE_IO);
    agregar_a_paquete(paquete, &pid_proceso_Actual, sizeof(int));
    enviar_paquete(paquete, socket_kernel);
    eliminar_paquete(paquete);
    exit(EXIT_SUCCESS);
}
