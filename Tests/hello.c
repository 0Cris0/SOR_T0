#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char const *argv[]){
    printf("Soy el programa/proceso de Hello\n");
    // Los argumentos válidos parten desde el 1, porque el 0 es la dirección
    printf("Argumentos = {%d} 1: [%s] 2:[%s]\n", argc, argv[1], argv[2]);
    sleep(3);
    printf("AAAAAAAAA\n");
    printf("-----+++------\n");
    return 0;
}