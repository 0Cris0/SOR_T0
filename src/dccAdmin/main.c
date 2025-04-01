
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>

#include "../input_manager/manager.h"
#include <signal.h>
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
  time_t tiempo_final;
  int exit_code;
  int signal;
} Proceso;


// Podría ser el nombre del proceso un atributo de proceso?
struct proceso* procesos;

void agregar_proceso(struct proceso* procesos, char* name, pid_t pid){
  if(procesos->pid == 0){
    strcpy(procesos->nombre, name);
    procesos->anterior = NULL;
    procesos->siguiente = NULL;
    procesos->exit_code = -1;
    procesos->pid = pid;
    procesos->tiempo_inicio = time(NULL);
  //printf("Ha quedado el head con PID = %d\n", procesos->pid);
  }
  else{
    struct proceso* nuevo_proceso = calloc(1, sizeof(struct proceso));
    strcpy(nuevo_proceso->nombre, name);
    nuevo_proceso->anterior = NULL;
    nuevo_proceso->siguiente = NULL;
    nuevo_proceso->exit_code = -1;
    nuevo_proceso->pid = pid;
    nuevo_proceso->tiempo_inicio = time(NULL);
    
    struct proceso* proceso_actual = procesos;
    while(proceso_actual->siguiente != NULL){
      proceso_actual = proceso_actual->siguiente;
    }
    nuevo_proceso->anterior = proceso_actual;
    proceso_actual->siguiente = nuevo_proceso;
  //printf("Ha agregado proceso con PID = %d\n", nuevo_proceso->pid);
  }
}

Proceso* buscar_proceso(pid_t pid_buscado, Proceso* procesos) {
  Proceso* proceso_actual = procesos; // Lista ligada (puntero al primer elemento)
  while (proceso_actual != NULL) {
      if (proceso_actual->pid == pid_buscado) {
          return proceso_actual; // Retorna el puntero del proceso buscado
      }
      proceso_actual = proceso_actual->siguiente;
  }
  return NULL;
}

void modificar_exit_code(Proceso* proceso, int nuevo_exit_code) {
  if (proceso != NULL) {
      proceso->exit_code = nuevo_exit_code;
      // Solo en el caso que haya terminado actualizo el tiempo final<
      if(nuevo_exit_code != -1){
        proceso->tiempo_final = difftime(time(NULL), proceso->tiempo_inicio); //TODO: Ver que sea en seg
      }
  } else {
      printf("Error: El puntero al proceso es NULL\n");
  }
}

void modificar_signal(Proceso* proceso, int signal){
  if (proceso != NULL) {
    proceso->signal = signal;
  } else {
      printf("Error: El puntero al proceso es NULL\n");
  }
}
void actualizar_proceso(Proceso* proceso, int signal, int nuevo_exit_code){
  modificar_exit_code(proceso, nuevo_exit_code);
  modificar_signal(proceso, signal);
}

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
          exit(EXIT_FAILURE);
      default:
          // Padre
          agregar_proceso(procesos, input[1], pid);
          //TODO:
/*            int status;
          pid_t resultado = waitpid(pid, &status, WNOHANG); //TODO: ver si esto funciona
          // Debo agregar manera de que espere al hijo
          
          if(resultado == 0){
            printf("> Hijo aún en ejecución, esperando...\n");
            //printf("> Status hijo: %d", status);
          }
          else{
            printf("> Hijo ya terminó, continuando ejecución\n");
            //printf("> Status hijo: %d", status); 
          }*/
      }
  }
  else{
    perror("[Error]: Por favor entrega un path del ejecutable y argumentos válidos\n");
  }
  

}


void info(struct proceso* procesos){
  time_t tiempo_actual;
  // Se asume que puntero_procesos es una variable global
  Proceso* actual = procesos;  
  printf("=== ******<--------->******* ===\n");
  printf("***  INFORMACIÓN DE PROCESOS *****\n");
  // Recorrer la lista mientras el puntero 'actual' no sea NULL
  while (actual != NULL) {
      // Imprimir la información del proceso actual
      printf("Nombre: %s\n", actual->nombre);
      printf(" - Información del proceso:\n");
      printf(" - PID: %d\n", actual->pid);
      tiempo_actual = time(NULL);
      printf(" - Tiempo de ejecución: %ld s\n", actual->tiempo_final);
      printf(" - Exit code: %d\n", actual->exit_code);
      printf(" - Signal value: %d\n", actual->signal);
      printf("\n");

      // Mover al siguiente proceso en la lista
      actual = actual->siguiente;
  }
  printf("=== ******<--------->******* ===\n");
}

void timeout(struct proceso* procesos, int tiempo){
  // ARREGLAR
  if(procesos->pid != 0){
    printf("No hay procesos en ejecución. Timeout no se puede ejecutar.\n");
  }
  else{
    // Creo conveniente partir desde atrás liberando porque esos serían los procesos hijos
    // Sino puede que mate un proceso padre sin haberme encargado de los hijos
    int procesos_activos = 0;
    struct proceso* proceso_actual = procesos;
    struct proceso* proceso_final = NULL;
    while(proceso_actual != NULL){
      // Ver si aquí agregar la actualización de exit_code
      if(proceso_actual->exit_code == -1){
        procesos_activos+=1;
      }
      proceso_final = proceso_actual;
      proceso_actual = proceso_actual->siguiente;
    }
    if(procesos_activos==0){
      printf("No hay procesos en ejecución. Timeout no se puede ejecutar.\n");
    }
    else{
      // TODO: Esperar que transcurra "tiempo"
      time_t tiempo_inicial = time(NULL);
      while(difftime(time(NULL), tiempo_inicial)<tiempo){
        // Aquí espero;
      }
      //
      proceso_actual = proceso_final;
      while(proceso_actual != NULL){
        if(proceso_actual->exit_code == -1 && proceso_actual->pid != getpid() && proceso_actual->pid > 0){
          printf("Timeout cumplido!\n");
          int tiempo_ejecucion = time(NULL)-proceso_actual->tiempo_inicio;
          printf("%d %s %d %d %d", proceso_actual->pid, proceso_actual->nombre, tiempo_ejecucion, proceso_actual->exit_code, proceso_actual->signal);
          // TODO: Enviar SIGTERM
          kill(proceso_actual->pid, SIGTERM);
        }
        proceso_actual = proceso_actual->anterior;
      }
    }
  }
}

void quit(struct proceso* procesos){
  printf("DCCAdmin finalizado\n");
  if(procesos->pid != 0){
    // Obtengo referencia al proceso final
    struct proceso* proceso_actual = procesos;
    struct proceso* proceso_final = NULL;
    while(proceso_actual != NULL){
      proceso_final = proceso_actual;
      proceso_actual = proceso_actual->siguiente;
    }
    
    // Envio el SIGINT
    proceso_actual = proceso_final;
    while(proceso_actual != NULL){
      if(proceso_actual->exit_code == -1 && proceso_actual->pid != getpid() && proceso_actual->pid > 0){
        kill(proceso_actual->pid, SIGINT);
        proceso_actual = proceso_actual->anterior;
      }
    }

    // Ahora espero 10s
    time_t tiempo_inicial = time(NULL);
    while(difftime(time(NULL), tiempo_inicial)<10){
      // Aquí espero;
    }

    // Ahora empiezo a mandar los SIGKILL
    proceso_actual = proceso_final;
    while(proceso_actual != NULL){
      // Si está activo y si no es el que ejecuta el código
      if(proceso_actual->exit_code == -1 && proceso_actual->pid != getpid() && proceso_actual->pid > 0){
        // Ahora reviso el estado del proceso
        int status;
        pid_t resultado = waitpid(proceso_actual->pid, &status, WNOHANG);
        if(resultado == 0){
          // Mando SIGKILLS porque proceso no ha terminado
          kill(proceso_actual->pid, SIGKILL);
          //TODO: 
          /* // Ahora espero que termine bien su cierre
          waitpid(proceso_actual->pid, &status, 0); */
          //Ver si conviene hacer ese o este, porque quiero saber el estado del proceso
          // Pero a la vez como estoy en SIGKILL no sé si esperarlo o no
          waitpid(proceso_actual->pid, &status, WNOHANG);
          actualizar_proceso(proceso_actual, 9, WEXITSTATUS(status));

        }
        else{
          actualizar_proceso(proceso_actual, 2, WEXITSTATUS(status));
        }
        proceso_actual = proceso_actual->anterior;
      }
      // Imprimir estadísticas
      info(procesos);
    }
  }
  // Liberar las cosas y terminar ejecución
  liberar_procesos(procesos);
  printf("[>] Cerrando consola [<]\n");
  exit(0);
}

void sigint_handler(int señal){
  quit(procesos);
}

void sigchld_handler(int señal){
  // https://stackoverflow.com/questions/7171722/how-can-i-handle-sigchld
  // stackoverflow.com/questions/7155810/example-of-waitpid-wnohang-and-sigchld?noredirect=1&lq=1
  int status;
  pid_t pid;
  while((pid=waitpid(-1, &status, WNOHANG))>0){
    struct proceso* proceso_actual = buscar_proceso(pid, procesos);
    actualizar_proceso(proceso_actual, 17, WEXITSTATUS(status));
  }
}



int main(int argc, char const *argv[])
{
  bool consola_activa = true;
  signal(SIGINT, sigint_handler);
  signal(SIGCHLD, sigchld_handler);

  printf("\n=== Bienvenido/a a DCC-Admin ===\n");
  printf("=== ------<--------->------- ===\n");
  procesos = calloc(1, sizeof(struct proceso));

/*   // Instancia de proceso p1
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

  info(); */

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
      info(procesos);
    }
    else if(strcmp(input[0], "timeout")==0){
      //
    }
    else if(strcmp(input[0], "quit")==0){
      // 
      // Ver cómo equiparar con Ctrl+C
      // Liberar procesos y memoria van aquí
      consola_activa = false;
    }
    free_user_input(input);

    

  } 
  return 0;
}