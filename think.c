#include "think.h"

void think(int sig){
  (void)sig;
  if(serverinfo->startcalc == 1){
    printfield();
    //move = "A3:B4\0";
    char *move = bestMoveAll(serverinfo->clientplayernr);
    printf("Spielzug: %s\n", move);
    free(move);
    move = "\0";

    if((write (fd[1], move, 25)) != 25){                    //schreibt in die Pipe; höchstens 12 sprünge möglich
        perror("Error trying to write into the pipe");
        exit (EXIT_FAILURE);
    }
    serverinfo->startcalc = 0;
  }
}

/**
 * Berechnet den besten nächsten Speilzug des Steins an Position (i,j):
 * @param i Zeile des Steins
 * @param j Spalte des Steins
 * @return Array das den Spielzug und die dazugehörige Gewichtung enthält
 */
char **bestMove(int i, int j){
  char **move = malloc(sizeof(char*)*2);
  move[0] = malloc(sizeof(char)*BUFFERLENGTH_MOVE); //MOVE STRING
  move[1] = malloc(sizeof(char)*BUFFERLENGTH_MOVE); //GEWICHTUNG
  //Gültige Möglichkeiten für Stein(i,j)
  char ***possibleMoves = calcPossibleMoves(i,j);
  //maximum der gewichtung der gültigen möglichkeiten
  int maxIndex = 0;
  int max = 0;
  int x = 0;
  for(x = 0; x<BUFFERLENGTH_MOVE;x++){
    if(atoi(possibleMoves[x][1])>max){
      maxIndex = x;
    }
  }
  strcpy(move[0], possibleMoves[maxIndex][0]);
  strcpy(move[1], possibleMoves[maxIndex][1]);

  //Speicher freigeben
  for(int y = 0; y<BUFFERLENGTH_MOVE;y++){
    free(possibleMoves[y][0]);
    free(possibleMoves[y][1]);
  }
  for(int y = 0; y<2;y++){
    free(possibleMoves[y]);
  }
  free(possibleMoves);

  printf("Move:%s Weight: %s\n",move[0],move[1]);
  return move;
}

/**
 * Berechnet den besten nächsten Speilzug aus
 * all den berechneten besten Zügen der Steine eines Players.
 * @param playernr Playernummer des zu berechnenden besten Zugs
 * @return Bester möglicher Zug als String
 */
char *bestMoveAll(int playernr){
  char ***moves = malloc(sizeof(char***)*BUFFERLENGTH_MAXPIECES);
  int x=0;
  for(int i = 0; i<ROWS;i++){
    for(int j = 0; j<COLUMNS;j++){
      if(playernr){
        if(isBlack(i,j)==1){
          moves[x] = bestMove(i,j);
          x++;
        }
      }else{
        if(isWhite(i,j)==1){
          moves[x] = bestMove(i,j);
          x++;
        }
      }
    }
  }
  return maxWeightMove(moves,x);
}

/**
 * Berechnet den Zug mit der maximalen Gewichtung.
 * @param moves Array der Spielzüge und zugehörigen Gewichtungen
 * @param pieces Anzahl der in moves enthaltenen Spielzüge
 * @return Den nach der Gewichtung besten Zug
 */
char *maxWeightMove(char ***moves,int pieces){
  int maxIndex = 0;
  int max = 0;
  for(int i = 0; i<pieces;i++){
    if(atoi(moves[i][1])>max){
      maxIndex = i;
    }
  }
  char *bestmove = malloc(sizeof(char)*BUFFERLENGTH_MOVE);
  strcpy(bestmove, *moves[maxIndex]);

  //Speicher freigeben
  for(int i = 0; i<pieces;i++){
    free(moves[i][0]);
    free(moves[i][1]);
  }
  for(int i = 0; i<2;i++){
    free(moves[i]);
  }
  free(moves);

  //Besten Spielzug zurückgeben
  return bestmove;
}

/**
 * Berechnet die möglichen gültigen Spielzüge für einen Stein(i,j)
 * @param i Zeile des Steins
 * @param j Spalte des Steins
 * @return Array mit allen mögichen Spielzügen und zugehörigen Gewichtungen
 */
char ***calcPossibleMoves(int i, int j){
  printf("Calculate possible moves for piece(%c:%i)\n",inttocolumn(j),COLUMNS-i);
  char ***possibleMoves = malloc(sizeof(char*)*BUFFERLENGTH_MOVE);

  for(int x = 0; x<BUFFERLENGTH_MOVE;x++){
    possibleMoves[x] = malloc(sizeof(char*)*2);
    possibleMoves[x][0] = malloc(sizeof(char*)*BUFFERLENGTH_MOVE);
    possibleMoves[x][1] = malloc(sizeof(int));

    //TODO Hier alle möglichen Spielzüge berechnen und pro Spielzug in das Array kopieren,

    strcpy(possibleMoves[x][0],"testmove");
    sprintf(possibleMoves[x][1], "%i", calcWeight(possibleMoves[x][0]));
  }
  return possibleMoves;
}

/**
 * Berechnet die Gewichtung für einen gegebenen Zug
 * @param move Spielzug als String
 * @return Die Gewichtung des Zugs
 */
int calcWeight(char *move){
  (void)move;
  //printf("Calculating weight für possible move %s\n",move);

  //TODO Für einen Spielzug Gewichtung ausrechnen

  return 0;
}
