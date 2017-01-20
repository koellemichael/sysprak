#include "think.h"

void think(int sig){
  (void)sig;
  if(serverinfo->startcalc == 1){
    printfield();
    //move = "A3:B4\0";
    printf("Spielzug: %s\n", bestMoveAll(serverinfo->clientplayernr));
    move = "\0";

    if((write (fd[1], move, 25)) != 25){                    //schreibt in die Pipe; höchstens 12 sprünge möglich
        perror("Error trying to write into the pipe");
        exit (EXIT_FAILURE);
    }
    serverinfo->startcalc = 0;
  }
}

char **bestMove(int i, int j){
  char **move = malloc(sizeof(char*)*2);
  move[0] = malloc(sizeof(char)*BUFFERLENGTH_MOVE); //MOVE STRING
  move[1] = malloc(sizeof(char)*BUFFERLENGTH_MOVE); //GEWICHTUNG
  //Gültige Möglichkeiten für Stein(i,j)
  //maximum der gewichtung der gültigen möglichkeiten

  *move[0] = inttocolumn((j)%8);
  sprintf(move[1], "%i", i+j+1);
  printf("Move:%s Gewichtung: %s\n",move[0],move[1]);


  return move;
}

char *bestMoveAll(int playernr){
  (void)playernr;
  printf("bestMoveAll\n");
  char ***moves = malloc(sizeof(char***)*ROWS*COLUMNS);
  int x=0;
  for(int i = 0; i<ROWS;i++){
    for(int j = 0; j<COLUMNS;j++){
      //if(isWhite(i, j)==playernr)
        moves[x] = bestMove(i,j);
      x++;
    }
  }
  return maxWeightMove(moves);
  return NULL;
}

char *maxWeightMove(char ***moves){
  int maxIndex = -1;
  int max = 0;
  for(int i = 0; i<ROWS*COLUMNS;i++){
    if(atoi(moves[i][1])>max){
      maxIndex = i;
    }
  }
  return *moves[maxIndex];
}
