#include "think.h"

int think(int* fd){
    
    print();
    move = "A3:B4";
    if((write (fd[1], move, 5)) != 5){
        perror("Error trying to write into the pipe");
        exit (EXIT_FAILURE);
    }
    
    return 1;
}
