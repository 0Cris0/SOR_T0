
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include "../input_manager/manager.h"
// TODO: Entender qué hace eso

typedef struct proceso {
  // Para lista ligada (LL)
  struct proceso* siguiente;
  struct proceso* anterior;
  // Información del proceso
  pid_t pid;
  char nombre[256];
  int tiempo_inicio;
  int tiempo_final; // Ver qué conviene más en esto, si tener como atributo
  // o si solo armarlo cuando al final necesitemos imprimir
  int exit_code;
} Proceso;

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
void info(){
  /* Debo printear listado de todos los programas ejecutados desde
   dccAdmin y que esten siendo ejecutados en ese entonces
      PID
      Nombre del ejecutable
      Tiempo de ejecución en segundos
      Exit code
        -1 si ell proceso hijo NO ha terminado
        exit code si hijo ha terminado
      Signal value recibida por el proceso
        -1 si no recibió señal
        Valor de la señal recibida por proceso
          Depende del tipo de señal que emita
   */
}



void agregar_proceso(struct proceso* procesos, char name[256], pid_t pid){
  if(procesos == NULL){
    strcpy(procesos->nombre, name);
    printf(">>>> testeando nombre %s v/s copiado %s\n", name, procesos->nombre);
    procesos->anterior = NULL;
    procesos->siguiente = NULL;
    procesos->exit_code = -1;
    procesos->pid = pid;
    procesos->tiempo_inicio = time(NULL);
    procesos->tiempo_final = 0;
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
    nuevo_proceso->tiempo_final = 0; 
    
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

int main(int argc, char const *argv[])
{
  char** input = read_user_input();
  printf("%s\n", input[0]);
  perror("EEEEERORR TEST");
  free_user_input(input);


  struct proceso* procesos = calloc(1, sizeof(struct proceso));
  // Hacr análisis de casos según command
  // Caso start
  // Caso info
  // Caso timeout
  // Caso quit
  liberar_procesos(procesos);
  return 0;
}

