
#include <stdio.h>
#include <stdlib.h>
#include "../input_manager/manager.h"
#include <unistd.h> 
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

void start(const char *executable_path, char *const argv[]){
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
    return 1;
  }
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
typedef struct proceso {
  // Siguiente proceso cosa de hacer LL
} Proceso;

int main(int argc, char const *argv[])
{
  char** input = read_user_input();
  printf("%s\n", input[0]);
  perror("EEEEERORR TEST");
  free_user_input(input);

  pid_t pid = fork();
  if(pid < 0){
    perror("fork falló");
    return 1;
  }
  else if (pid == 0) {
    // Child process
    printf("Soy el hijo con PID %d\n", getpid());
  } else if (pid > 0) {
    // Parent process
    printf("Soy el padre con PID %d y mi hijo tiene PID %d\n", getpid(), pid);
  }
  return 0;
}

