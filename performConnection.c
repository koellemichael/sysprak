#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "performConnection.h"

#define BUFFERLENGTH 256

void performConnection(int *fd){

  char *buffer = malloc(BUFFERLENGTH*sizeof(char));

  do{
    fgets(buffer,BUFFERLENGTH,stdin);

    if(strncmp(buffer, "+", 1) == 0){
      printf("positiv\n");
    }else if(strncmp(buffer, "-", 1) == 0){
      printf("negativ\n");
      
      printf("Disconnecting server...\n");
      exit(EXIT_FAILURE);
    }

  }while(strcmp(buffer, "+ ENDPLAYERS\n")!=0);

  if(buffer!=NULL){
    free(buffer);
  }

}
