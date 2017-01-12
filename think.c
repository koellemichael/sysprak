#include "think.h"

void think(int sig, int* fd){
    
    if(serverinfo->startcalc == 1){
    printfield();
    //TODO: SPIELTAKTIK
    move = "A3:B4";
    
    if((write (fd[1], move, 5)) != 5){
        perror("Error trying to write into the pipe");
        exit (EXIT_FAILURE);
    }
    serverinfo->startcalc = 0;
  }
}
