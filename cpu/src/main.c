#include "include/cpu.h"


int main(int argc, char* argv[]) {
    // Inicializamos un logger:
    logger = log_create("cpu.log", "CPU", true, LOG_LEVEL_INFO);

    // Inicializamos la configuración del kernel y nos conectamos a él:
    cpu_config_init();
    

    char *puerto_dispatch = string_itoa(config_cpu->PUERTO_KERNEL_DISPATCH);
    char *puerto_interrupt = string_itoa(config_cpu->PUERTO_KERNEL_INTERRUPT);

    int opcion;
    do {
        mostrar_menu();
        scanf("%d", &opcion);

        switch ((op_code)opcion) {

            case HANDSHAKE:
                printf("Opción: HANDSHAKE seleccionada.\n");
                setup_connection_with_server("Kernel DISPATCH", config_cpu->IP_KERNEL, puerto_dispatch, set_socket_kernel_dispatch);
                setup_connection_with_server("Kernel INTERRUPT", config_cpu->IP_KERNEL, puerto_interrupt, set_socket_kernel_interrupt);
                break;

            case CPU_ID:
                printf("Opción: CPU_ID seleccionada.\n");
                setup_connection_with_server("Kernel DISPATCH", config_cpu->IP_KERNEL, puerto_dispatch, identificar_cpu_A_kernel);
                break;

            case INIT_PROC:
                printf("Opción: INIT_PROC seleccionada.\n");
                break;

            case DUMP_MEMORY:
                printf("Opción: DUMP_MEMORY seleccionada.\n");
                break;

            case IO:
                printf("Opción: IO seleccionada.\n");
                break;

            case EXIT_Sys:
                printf("ELimianr proceso  SYSCALL EXIT.\n");
                break;

            default:
                printf("Opción inválida. Intente de nuevo.\n");
        }

        printf("\n");

    } while (opcion != EXIT_Sys);

    
    
    return 0;
}
void mostrar_menu() {
    printf("===== MENÚ OPCIONES =====\n");
    printf("Presione %d para HANDSHAKE\n", HANDSHAKE);
    printf("Presione %d para CPU_ID\n", CPU_ID);
    printf("Presione %d para INIT_PROC\n", INIT_PROC);
    printf("Presione %d para DUMP_MEMORY\n", DUMP_MEMORY);
    printf("Presione %d para IO\n", IO);
    printf("Presione %d para EXIT_Sys\n", EXIT_Sys);
    printf("==========================\n");
    printf("Seleccione una opción: ");
}