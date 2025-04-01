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
1. *`start`*
2. *`info`*
3. *`liberar_proceso`*
4. *`agregar_proceso`*
5. *`timeout`*
6. *`quit`*
7. *`actualizar_proceso`*
8. *`modificar_signal`*
9. *`modificar_exit_code`*
10. *`buscar_proceso`*

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

## Observaciones