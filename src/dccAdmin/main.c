
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include "../input_manager/manager.h"
// TODO: Entender qué hace eso











// Agregar exit code y sigint
typedef struct proceso {
  // Para lista ligada (LL)
  struct proceso* siguiente;
  struct proceso* anterior;
  // Información del proceso
  pid_t pid;
  char nombre[256];
  time_t tiempo_inicio;
  int exit_code;
  int signal;
} Proceso;






// Podría ser el nombre del proceso un atributo de proceso?


void agregar_proceso(struct proceso* procesos, char name[256], pid_t pid){
  if(procesos == NULL){
    // Testear que se cumpla
    strcpy(procesos->nombre, name);
    printf(">>>> testeando nombre %s v/s copiado %s\n", name, procesos->nombre);
    procesos->anterior = NULL;
    procesos->siguiente = NULL;
    procesos->exit_code = -1;
    procesos->pid = pid;
    procesos->tiempo_inicio = time(NULL);
    //procesos->tiempo_final = 0;
  }
  else{
    struct proceso* nuevo_proceso = calloc(1, sizeof(struct proceso));
    strcpy(nuevo_proceso->nombre, name);
    printf(">>>> testeando nombre %s v/s copiado %s\n", name, nuevo_proceso->nombre);
    nuevo_proceso->anterior = NULL;
    nuevo_proceso->siguiente = NULL;
    nuevo_proceso->exit_code = -1;
    nuevo_proceso->pid = pid;
    nuevo_proceso->tiempo_inicio = time(NULL);
   // nuevo_proceso->tiempo_final = 0; 
    
    struct proceso* proceso_actual = procesos;
    while(proceso_actual->siguiente != NULL){
      proceso_actual = proceso_actual->siguiente;
    }
    nuevo_proceso->anterior = proceso_actual;
    proceso_actual->siguiente = nuevo_proceso;
  }
}

/* Yo creo que se va a tener que hacer función para ver lo de los terminos
 de procesos y sus exit_code
 Tal vez podría recorrer la cosa buscando el proceso y de ahí actualizar
 
 O mejor, voy a crear función buscar proceso por PID, que retorne un struct
  proceso* cosa que de ahí lo pueda modificar
 */

 void liberar_procesos(struct proceso* procesos){
  struct proceso* proceso_actual = procesos;
  struct proceso* proceso_a_liberar;
  printf("== Liberando procesos\n");
  while(proceso_actual != NULL){
    proceso_a_liberar = proceso_actual;
    proceso_actual = proceso_actual->siguiente;
    printf("=== Liberando proceso con PID = {%d}\n", proceso_a_liberar->pid);
    free(proceso_a_liberar);
  }
 }






 void start(const char *executable_path, char *const argv[], struct proceso* procesos){
  /* Tomar ruta del executable y sus argumentos y los ejecuta
   con nu nuevo proceso
  Caso 1: Executable no existe
    Indicar error al usuario
  Caso 2: Executable existe
    Hacer fork para crear nuevo proceso desde la shell
    Hacer execve?? para poder aplicar el código del executable al proceso

    No debo congelar la shell
    */

  pid_t pid = fork();
  if(pid < 0){
    perror("[Error]: Ha sucedido un error con fork()");
  }
  // Agregar en los casos de abajo manera de agregar a la LL de procesos iniciados por dccAdmin
  else if (pid == 0) {
    printf("Soy el hijo con PID %d\n", getpid());
    printf("Soy el hijo con PID %d\n", getpid());
    // Intentar ejecutar execve y ver si tira error
    printf("Ejecutando excecve\n");
    execve(executable_path, argv, NULL);
    perror("[Error]: Ha ocurrido un error al ejecutar execv, puede que el ejecutable no exista\n");
  } else if (pid > 0) {
    printf("Soy el padre con PID %d y mi hijo tiene PID %d\n", getpid(), pid);
    int status;
    pid_t resultado = waitpid(pid, &status, WNOHANG);
    if(resultado == 0){
      printf("Padre ha encontrado al hijo aún en ejecución, esperando...\n");
    } else {
      printf("Padre, hijo ha terminado\n");
    }
    
  }

}

Proceso* puntero_procesos = NULL;


void info(){
  time_t tiempo_actual;
  // Se asume que puntero_procesos es una variable global
  Proceso* actual = puntero_procesos;  

  printf("***  INFORMACIÓN DE PROCESOS *****\n");
  // Recorrer la lista mientras el puntero 'actual' no sea NULL
  while (actual != NULL) {
      // Imprimir la información del proceso actual
      printf("Nombre: %s\n", actual->nombre);
      printf("Información del proceso:\n");
      printf("PID: %d\n", actual->pid);
      tiempo_actual = time(NULL);
      printf("Tiempo de ejecución: %ld segundos\n", tiempo_actual - actual->tiempo_inicio);
      printf("Exit code: %d\n", actual->exit_code);
      printf("Signal value: %d\n", actual->signal);
      printf("\n");

      // Mover al siguiente proceso en la lista
      actual = actual->siguiente;
  }
}



int main(int argc, char const *argv[])
{
  // Instancia de proceso p1
  Proceso p1;
  strcpy(p1.nombre, "Proceso 1");
  p1.pid = 10000;
  p1.anterior = NULL;
  p1.siguiente = NULL;
  p1.exit_code = -1;
  p1.tiempo_inicio = time(NULL);
  p1.signal = 0;
  // Instancia p2
  sleep(1);
  Proceso p2;
  strcpy(p2.nombre, "Proceso 2");
  p2.pid = 20000;
  p2.anterior = NULL;
  p2.siguiente = NULL;
  p2.exit_code = -1;
  p2.tiempo_inicio = time(NULL);
  p2.signal = 0;
  // Instancia P3
  sleep(1);
  Proceso p3;
  strcpy(p3.nombre, "Proceso 3");
  p3.pid = 30000;
  p3.anterior = NULL;
  p3.siguiente = NULL;
  p3.exit_code = -1;
  p3.tiempo_inicio = time(NULL);
  p3.signal = 0;
  // Enlazar procesos como lista ligada
  p1.siguiente = &p2;
  p2.anterior = &p1;
  p2.siguiente = &p3;
  p3.anterior = &p2;
  p3.siguiente = NULL;
  // Imprimir lista enlazada

  puntero_procesos = &p1;

  info();
  return 0;
}