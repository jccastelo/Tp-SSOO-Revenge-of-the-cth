# Revenge of The Cth

Trabajo práctico de la materia Sistemas Operativos en la Universidad Tecnológica Nacional - Facultad Regional Buenos Aires (UTN-FRBA), realizado en el primer cuatrimestre del año 2025.

Este repositorio contiene un proyecto que simula el funcionamiento de un sistema operativo, contando con los módulos kernel, CPU, memoria + swap y I/O

---

## Integrantes del equipo

| Apellido       | Nombre    | User de Github                                                     | Módulo      |
|----------------|-----------|--------------------------------------------------------------------|-------------|
| Castelo        | Juan Cruz | [jccastelo](https://github.com/jccastelo)                          | CPU         |
| Vazquez Cañoto | Tadeo     | [Tadeo-VC](https://github.com/Tadeo-VC)                            | Kernel, I/O |
| Gabarini       | Pablo     | [PGabarini ](https://github.com/PGabarini)                         | Kernel      |
| Encinaz Sayago | Federico  | [FedericoEncinazSayago](https://github.com/FedericoEncinazSayago)  | Memoria     |
| Silva          | Dante     | [cuso03](https://github.com/cuso03)                                | Memoria     |

---

## Funciones y características

### Visión general

- Se simula un sistema operativo modular compuesto por **Kernel**, **CPU**, **Memoria + Swap**, **I/O** y una librería común **Utils**.
- Los módulos se comunican entre sí mediante **sockets** y un **protocolo propio** de mensajes definido.
- Soporta ejecución de procesos definidos por pseudocódigo, con manejo de **estados**, **planificación**, **memoria virtual**, **TLB**, **caché** y **operaciones de entrada/salida** temporizadas.

### Kernel

- Núcleo del sistema: coordina la ejecución de procesos y la interacción entre CPU, Memoria e I/O.
- Implementa planificación de **largo**, **mediano** y **corto plazo**, manteniendo colas para los estados clásicos de un proceso: `NEW`, `READY`, `EXEC`, `BLOCKED`, `BLOCKED_SUSPENDED`, `READY_SUSPENDED`, `EXIT`.
- Soporta múltiples algoritmos de **planificación de corto plazo** (configurables por archivo):
  - **FIFO**
  - **SJF sin desalojo (SJFsD)**
  - **SJF con desalojo (SJFcD)**
  - **PMCP** (algoritmo propio del TP basado en prioridades/criterios de memoria/CPU)
- Administra:
  - **CPUs conectadas**, detectando cuáles están disponibles y despachando procesos a ejecutar.
  - **Dispositivos de I/O**, manteniendo colas de procesos bloqueados por cada dispositivo e instancias activas.
  - **Relación con Memoria**, solicitando creación, suspensión, desuspensión y finalización de procesos.
- Registra **métricas de planificación** (tiempos en cada estado, tiempos de espera/ejecución) y genera logs detallados de cambios de estado y decisiones del planificador.

### CPU

- Simula uno o varios procesadores que ejecutan las instrucciones de los procesos.
- Implementa el **ciclo de instrucción** clásico: *fetch → decode → execute*.
- Se conecta al Kernel por dos canales:
  - **Dispatch**: recepción y envío de contextos de ejecución.
  - **Interrupt**: recepción de señales de desalojo.
- Se comunica con Memoria para obtener **instrucciones** y **datos**, utilizando:
  - **MMU** para traducir direcciones lógicas a físicas.
  - **TLB** para acelerar la traducción de páginas a marcos.
  - **Caché de páginas** para acelerar lecturas/escrituras de datos.
- Soporta un conjunto de **instrucciones** de pseudocódigo (NO_OP, READ, WRITE, GOTO, IO, INIT_PROC, DUMP_MEMORY, EXIT, entre otras) y, ante syscalls bloqueantes o interrupciones, devuelve el contexto al Kernel.
- Implementa algoritmos de reemplazo:
  - En **TLB**: `FIFO` y `LRU`.
  - En **caché** de datos: variantes basadas en `CLOCK`, con bits de uso y modificado.

### Memoria + Swap

- Simula la **memoria principal** del sistema y un archivo de **swap**.
- Gestiona el espacio de direcciones de cada proceso mediante **paginación**, manteniendo tablas de páginas y asignando **marcos** en memoria.
- Atiende pedidos de:
  - **Kernel**: creación (`INIT_PROC`), suspensión, desuspensión y finalización de procesos.
  - **CPU**: lectura de instrucciones y acceso a datos (lecturas/escrituras en marcos).
- Implementa **swap in / swap out** de páginas y/o procesos completos hacia/desde un archivo de swap, respetando retardos configurables para simular latencia de disco.
- Mantiene **métricas** de acceso (cantidad de escrituras a memoria, operaciones de swap, etc.) y permite realizar *dumps* del estado de la memoria según configuración.

### I/O

- Cada instancia de I/O simula un **dispositivo de entrada/salida** independiente (por ejemplo, discos, terminales, etc.).
- Se conecta al Kernel, desde donde recibe:
  - El **PID del proceso** a bloquear.
  - El **tiempo de operación** a simular.
- Para cada pedido:
  - Simula el tiempo de bloqueo mediante retardos configurables.
  - Al finalizar, notifica al Kernel para que el proceso pase de **BLOCKED** a **READY** (o al estado correspondiente según si está o no en memoria principal).
- Permite ejecutar múltiples dispositivos I/O en paralelo, cada uno con su propia configuración.

### Utils

- Librería común utilizada por todos los módulos del proyecto.
- Proporciona:
  - Manejo de **sockets** y conexiones.
  - Implementación del **protocolo de mensajes** entre módulos (serialización/deserialización de paquetes, códigos de operación, buffers, etc.).
  - Utilidades de **logging**, configuración y estructuras auxiliares.
- Facilita que cada módulo se enfoque en su lógica específica de sistema operativo, reutilizando una base común de infraestructura.

---

## Dependencias

Para poder compilar y ejecutar el proyecto, es necesario tener instalada la
biblioteca [so-commons-library] de la cátedra:

```bash
git clone https://github.com/sisoputnfrba/so-commons-library
cd so-commons-library
make debug
make install
```

---

## Compilación y ejecución

Cada módulo del proyecto se compila de forma independiente a través de un
archivo `makefile`. Para compilar un módulo, es necesario ejecutar el comando
`make` desde la carpeta correspondiente.

El ejecutable resultante de la compilación se guardará en la carpeta `bin` del
módulo. Ejemplo:

```sh
cd kernel
make
./bin/kernel
```

El orden de ejecución de los módulos es el siguiente:
1. Memoria
2. Kernel
3. CPU (1 o más instancias)
4. I/O (1 o más instancias)

---

## Importar desde Visual Studio Code

Para importar el workspace, debemos abrir el archivo `tp.code-workspace` desde
la interfaz o ejecutando el siguiente comando desde la carpeta raíz del
repositorio:

```bash
code tp.code-workspace
```

---

## Despliegue del trabajo

Para desplegar el proyecto en una máquina Ubuntu Server, podemos utilizar el
script [so-deploy] de la cátedra:

```bash
git clone https://github.com/sisoputnfrba/so-deploy.git
cd so-deploy
./deploy.sh -r=release -p=utils -p=kernel -p=cpu -p=memoria -p=io "tp-{año}-{cuatri}-{grupo}"
```

El mismo se encargará de instalar las Commons, clonar el repositorio del grupo
y compilar el proyecto en la máquina remota.

> [!NOTE]
> Ante cualquier duda, pueden consultar la documentación en el repositorio de
> [so-deploy], o utilizar el comando `./deploy.sh --help`.