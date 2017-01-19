#include "think.h"

void think(int sig){
  (void)sig;
  if(serverinfo->startcalc == 1){
    printfield();
    if(test)
      move = "A3:B4\0";
    else
      move = "E3:F4\0";
    //move = "\0";
    if((write (fd[1], move, 5)) != 5){
        perror("Error trying to write into the pipe");
        exit (EXIT_FAILURE);
    }
    test = 0;
    serverinfo->startcalc = 0;
  }
}
