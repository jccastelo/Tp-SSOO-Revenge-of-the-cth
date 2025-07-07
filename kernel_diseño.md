# **Modulo KERNEL**

> *"Diseñado con mucho amor y pasión por Pablo Kernel Gabarini y Tadeo Kernel Beast IO Destroyer"*

Este documento tiene como objetivo describir el diseño y funcionamiento del módulo ``Kernel``, detallando tanto su funcionamiento como sus responsabilidades dentro del sistema. A continuación, se enumeran los aspectos que se desarrollarán en las siguientes secciones:

- Conexiones
- Gestion de solicitudes
- Planificador
- Cambios de estado

# Conexiones

### Arquitectura del Sistema
![Diagrama - Arquitectura del Sistema](image-1.png)
El modulo ``Kernel`` funciona como servidor de ``IO`` y ``CPU``, pudiendo gestionar multiples instancias de dichos modulos en simultaneo, a través de conexiones permanentes. A su vez es cliente de ``Memoria`` creando conexiones efimeras para cada solicitud.

### Kernel como servidor de ``IO`` y ``CPU``

El modulo ``Kernel`` cuenta con el archivo ``kernel-init-connection.c`` dedicado a iniciar su estado como servidor, utilizando las funciones del directorio ``utils``. 
Los servidores creados son los siguientes:
- **DISPATCH** → Servidor para el puerto dispatch con CPU
- **INTERRUPT** → Servidor para el puerto interrupt con CPU
- **IO** → Servidor para las conexiones con IO

> *A REVISAR: Actualmente por alguna razón al inicializar los servidores está configurado para que se levanten utilizando la IP_Memoria del config*

Cada uno de los hilos para la gestión de las conexiones se crean en ``kernel-servers.c``, teniendo las siguientes características:
- Estrategia ``persistance`` → La conexión no se cierra
- Atiende de a un cliente a la vez → ``server_clients`` atiende a un cliente por vez, de forma secuencial.


### Kernel como cliente de ``Memoria``
Para las conexiones con memoria el modulo ``Kernel`` cuenta con el archivo ``kernel-memory-connection.c`` donde se gestiona la creacion de conexiones y las diferentes solicitudes que puede realizar.
Para la creacion de conexiones existen las siguientes funciones:
- **``kernel_memory_conection(void)``** → Crea la conexión con memoria y establece el socket en la variable global ``socket_memoria``  
- **``set_socket_memoria(int socket)``** → Se utiliza como parametro de ``setup_conection_with_server(...)`` (funcion de utils) dentro de ``kernel_memory_conection`` para establecer la conexión.

Logramos que la conexión sea efimera creando el siguiente wrapper:
```c
int solicitar_a_memoria(int (*operacion)(t_pcb* proceso), t_pcb* proceso_a_enviar) {

    kernel_memory_connection();

    int resultado;

    resultado = operacion(proceso_a_enviar); // Usa el socket global de set_socket_memoria

    close(socket_memoria);

    return resultado;
}
```

Las solicitudes realizadas a memoria son las siguientes enviando los datos en ese orden:

- **``memoria_init_proc``** ⇒ PID → Tamaño del proceso → Tamaño de la ruta → Ruta
- **``avisar_dump_memory``** ⇒ PID
- **``memoria_delete_proc``** ⇒ PID
- **``suspender_proceso``** ⇒ PID
- **``desuspender_proceso``** ⇒ PID

## Gestión de Solicitudes

Para la gestión de solicitudes existe el archivo ``kernel-escucha.c`` donde para cada cliente existe un handler con los codigos de operacion respectivos a ese módulo, y realizando las rutinas propias de dicha operación.

> *A REVISAR como está hecho ahora, podemos tratar de realizar una operación enviada por una CPU sin identificar*

## Planificador

En ``kernel-planner.c`` esta toda la lógica relacionada al planificador menos los cambios de estado. Las funciones destacables son:
- **``traer_procesos_a_MP()``** → utilizada para traer procesos a memoria principal al swapear procesos o matarlos. Primero trae de ready suspended y despues de new.
- **``mandar_proceso_a_execute()``** → al salir procesos de CPU, se llama a esta función que envía procesos de ready a execute mientras haya CPU's disponibles.

## Cambios de Estado

En ``kernel-cambios-de-estado.c`` está toda la lógica relacionada a las rutinas del ``kernel`` cuando un proceso realiza un cambio de estado. Actualizando las métricas y actualizando el PCB en cada cambio.