
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>

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









void agregar_proceso(struct proceso* procesos, char* name, pid_t pid){
  /* printf("++++++++ procesos puntero = %p\n", procesos);
  printf("++++++++ procesos PID = %d\n", procesos->pid); */
  if(procesos->pid == 0){
    // Testear que se cumpla
    strcpy(procesos->nombre, name);
    procesos->anterior = NULL;
    procesos->siguiente = NULL;
    procesos->exit_code = -1;
    procesos->pid = pid;
    procesos->tiempo_inicio = time(NULL);
    procesos->tiempo_final = 0;
  printf("Ha quedado el head con PID = %d\n", procesos->pid);
  }
  else{
    struct proceso* nuevo_proceso = calloc(1, sizeof(struct proceso));
    strcpy(nuevo_proceso->nombre, name);
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
  printf("Ha agregado proceso con PID = %d\n", nuevo_proceso->pid);
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
  printf("\n== Liberando procesos\n");
  printf("=======================\n");
  while(proceso_actual != NULL){
    proceso_a_liberar = proceso_actual;
    proceso_actual = proceso_actual->siguiente;
    printf("--- Liberando proceso con PID = {%d}\n", proceso_a_liberar->pid);
    free(proceso_a_liberar);
  }
  printf("=======================\n");
 }





void start(char** input, struct proceso* procesos){
  if(input[1] != NULL){
    pid_t pid = fork();
    switch (pid) {
      case -1:
          perror("[Error]: Error en el fork\n");
          exit(EXIT_FAILURE);
      case 0:
          // Hijo;
          int cantidad_args = 0;
          for(int i=1; input[i]!=NULL; i++){
            cantidad_args+=1;
          }
          char** copia_args = calloc(cantidad_args+1, sizeof(char*));
          // Ahora recorro los argumentos del input copiándolos y agrego NULL al final
          //printf("Argumentos entregados:\n");

          for(int j=1; input[j]!=NULL; j++){ //TODO: Considerar volver j=2 para saltarme eel path
            copia_args[j-1] = strdup(input[j]);
            //printf("- %s\n", copia_args[j-1]);
          }
          copia_args[cantidad_args] = NULL;
          // Ahora aplico execve
          execve(input[1], copia_args, NULL);
          perror("[Error]: Execve ha fallado, por favor revisa que el ejecutable exista y que los argumentos entregados sean correctos\n");
          for(int k=0; k<cantidad_args; k++){
            free(copia_args[k]);
          }
          free(copia_args);
          
          //-------
          // Debo agregar manera de que termine
      default:
          // Padre
          agregar_proceso(procesos, input[1], pid);
          int status;
          pid_t resultado = waitpid(pid, &status, WNOHANG); //TODO: ver si esto funciona
          // Debo agregar manera de que espere al hijo
          if(resultado == 0){
            printf("> Hijo aún en ejecución, esperando...\n");
          }
          else{
            printf("> Hijo ya terminó, continuando ejecución\n");
          }
      }
  }
  else{
    perror("[Error]: Por favor entrega un path del ejecutable y argumentos válidos\n");
  }
  

}






int main(int argc, char const *argv[])
{
  bool consola_activa = true;
  printf("=== Bienvenido/a a DCC-Admin ===\n");
  printf("=== ------<--------->------- ===\n");
  struct proceso* procesos = calloc(1, sizeof(struct proceso));

  while(true){
    if(!consola_activa){
      // Cerrar consola
      liberar_procesos(procesos);
      printf("[>] Cerrando consola [<]\n");
      exit(0);
      break;
    }

    
    printf("[>] Por favor, ingrese un comando:\n");
    char** input = read_user_input();
    printf("$ %s\n", input[0]);

    // Casos de input
    if(strcmp(input[0], "exit")==0){
      // Liberar procesos y memoria van aquí
      consola_activa = false;
    }
    else if(strcmp(input[0], "start")==0){
      start(input, procesos);
    }
    else if(strcmp(input[0], "info")==0){
      //
    }
    else if(strcmp(input[0], "timeout")==0){
      //
    }
    else if(strcmp(input[0], "quit")==0){
      //
      // Liberar procesos y memoria van aquí
      consola_activa = false;
    }
    free_user_input(input);

    

  } 
  return 0;
}

