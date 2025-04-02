# IIC2333 (Sistemas operativos y Redes): Tarea 0

Repositorio para la Tarea 0 de Sistemas operativos y Redes (IIC2333)

## Integrantes
* Nicolás Ulloa (S1)
* Cristóbal Albornoz (S1)

## Información General
### Compilado y ejecución
Para compilar la tarea se debe abrir una terminal en el directorio raíz (donde se encuentra el Makefile) y ejecutar el comando *`make`*

Luego, la tarea se ejecuta con *`./dccAdmin [{time_max}]`*, donde se podrá interactuar con la shell según lo pedido en el enunciado.

*`time_max`* es opcional. Indica la cantidad máxima de segundos que un proceso puede demorarse antes de ser terminado. En su ausencia, los procesos tienen tiempo ilimitado para ejecutarse 

### Estructura de la tarea
El archivo principal de la tarea es *`main.C`*. En él, se definieron las siguientes funciones:
- *`start`*
- *`info`*
- *`liberar_proceso`*
- *`agregar_proceso`*
- *`timeout`*
- *`quit`*
- *`actualizar_proceso`*
- *`modificar_signal`*
- *`modificar_exit_code`*
-  *`buscar_proceso`*

Además, se definió la estructura *`Proceso`* que consiste en una lista ligada, donde se almcena la información de cada proceso que se crea y ejecuta.

## Funcionalidades Implementadas
✅: Completo
🟨: En proceso de implementación
🟥: Aún no implementado

* 🟥`start`
    * (Descripción)
* 🟥`info`
    * (Descripción)
* 🟥`timeout`
    * (Descripción)
* 🟥`quit`
    * (Descripción)

## Syscalls utilizadas
- `fork()`: Crea un nuevo proceso duplicando el proceso actual.
- `wait()`: Suspende la ejecución del proceso actual (padre) hasta que un proceso hijo termine.
- `waitpid()`: Espera la terminación de un proceso hijo específico. Permite mayor control que `wait()`.
- `execv()`: Reemplaza el proceso actual con un nuevo programa, pasando losargumentos en un array.  

## Observaciones

## Referencias
Para la elaboración de la tarea, nos basamos en las siguientes fuentes:

- 