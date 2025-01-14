#include <stdio.h>
#include <stdlib.h>
#include "repl.h"
#include <stdbool.h>
#include <string.h>

int main(int argc, char* argv[], char* envp[]){
  printf("Nombre d'arg : %d\n", argc);
  for (int i = 0; i < argc; i++){
    printf("arg %d : %s\n", i, argv[i]);
  }

  printf("variables : \n");
  for(char **env = envp; *env != NULL; env++){
    printf("%s\n", *env);
  }    

  repl();

  return 0;
  
}

