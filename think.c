#include "think.h"

void printfield(void){   //TODO besser: alles mit strcat zu einem string zusammenfassen und den dann ausgeben, damit die ausgaben vom connector nicht dazwischen geprintet werden
  for(int i = 0; i <ROWS; i++){
    printf("%i| ",i);
    for(int j = 0; j <COLUMNS; j++){
      if(!strcmp(serverinfo->field[i][j], "")){
        printf("  ");
      }else{
        printf("%s ",serverinfo->field[i][j]);
      }
    }
    printf("\n");
  }
  printf("   ");
  for(int j = 0; j <COLUMNS; j++){
    printf("%c ",(65+j));
  }
  printf("\n");
}

void think(int sig){
  sig = 0;
  if(serverinfo->startcalc == 1){
    printf("Thinker\n");
    printfield();
    //move = "A3:B4";
    move = "";
    if((write (fd[1], move, 5)) != 5){
        perror("Error trying to write into the pipe");
        exit (EXIT_FAILURE);
    }
    serverinfo->startcalc = 0;
  }
}
