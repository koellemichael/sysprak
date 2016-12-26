#include "think.h"

int think(int* fd){
    
    string = "A3:B4";
    if((write (fd[1], string, 5)) != 5){
        perror("Error trying to write into the pipe");
        exit (EXIT_FAILURE);
    }
    
    return 1;
}
