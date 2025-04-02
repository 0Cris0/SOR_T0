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
- *`agregar_proceso`*: Se encarga de añadir un proceso a la lista ligada.
-  *`buscar_proceso`*: Retorna el puntero de un proceso buscado según su pid.
- *`modificar_exit_code`*: Recibe un puntero de procesos y un nuevo exit-code (int). Actualiza el exit code del proceso.
- *`modificar_signal`*: Actualiza el valor de signal de algún proceso según un int entregado.
- *`actualizar_proceso`*: Modifica el exit code y el valor de señal de un proceso (es una combinación de las dos funciones anteriores).
- *`liberar_proceso`*: Se encarga de liberar procesos.
- *`start`*: Recibe el path de un ejecutable (y la lista de procesos) junto con los argumentos que necesite. Inicia el proceso correspondiente al ejecutable.
- *`info`*: Recibe la lista de procesos y entrega la información correspondiente a cada uno de los procesos. 
- *`timeout`*
- *`quit`*: Función que termina el programa principal y se encarga de matar a los procesos aún activos.



Además, se definió la estructura *`Proceso`* que consiste en una lista ligada, donde se almcena la información de cada proceso que se crea y ejecuta.

## Funcionalidades Implementadas
Se implementaron los siguientes comandos:

* ✅:`start`
    * Comando que recibe una ruta de un ejecutable y los argumentos que necesite. Lo ejecuta mediante un nuevo proceso.
* ✅:`info`
    * Comando que entrega la información (imprime en consola) de todos los programas que fueron ejecutados y se encuentran ejecutando.
*  ✅:`timeout`
    * Recibe el argumento < time > y termina todos los procesos que se estén ejecutando en el programa. Si un proceso no finaliza dentro del tiempo < time >, se envía la señal SIGTERM
* ✅:`quit`
    * Termina el programa principal dccadmin y ciera los procesos que aún estén ejecutandose. Se muestran las estadísticas de los procesos ejecutados segun lo pedido.
    * Consideración: Al momento de ejecutar Ctrl+C si bien si activa el sistema de quit por alguna razón que no alcanzamos a indagar se cierran automáticamente los procesos sin activar la función de cambio de signal (creo). Esto no ocurre cuando uno hace 'quit' normal.
* ✅:`paralelismo y no bloqueo`
    * Según nuestros tests no se generan bloqueos y los procesos se pueden correr de manera paralela
* ❌:`time_max`
    * No se pudo implementar time_max

## Syscalls utilizadas
- `fork()`: Crea un nuevo proceso duplicando el proceso actual.
- `wait()`: Suspende la ejecución del proceso actual (padre) hasta que un proceso hijo termine.
- `waitpid()`: Espera la terminación de un proceso hijo específico. Permite mayor control que `wait()`.
- `execv()`: Reemplaza el proceso actual con un nuevo programa, pasando losargumentos en un array.  

## Referencias
Para la elaboración de la tarea, nos basamos en las siguientes fuentes:

- https://www.quora.com/How-do-I-call-delay-in-C-programming para que el programa espere según la equivalencia de segundos a ciclos de clock.