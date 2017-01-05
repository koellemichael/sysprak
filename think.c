#include "think.h"

void think(int sig){
  sig = 0;
  printf("%i\n", sig);
  if(serverinfo->startcalc == 1){
    printf("Thinker\n");
    printfield();
    //move = "A3:B4\0";
    move = "\0";
    if((write (fd[1], move, 5)) != 5){
        perror("Error trying to write into the pipe");
        exit (EXIT_FAILURE);
    }
    serverinfo->startcalc = 0;
  }
}
