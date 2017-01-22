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
  for(x = 0; x<p;x++){

    if(atoi(possibleMoves[x][1])>max){
      maxIndex = x;
    }
  }

  strcpy(move[0], possibleMoves[maxIndex][0]);
  strcpy(move[1], possibleMoves[maxIndex][1]);


  //Speicher freigeben
  for(int y = 0; y<p;y++){
    free(possibleMoves[y][0]);
    free(possibleMoves[y][1]);
  }

  for(int y = 0; y<p;y++){
    if(possibleMoves[y]!=NULL)
      free(possibleMoves[y]);
  }

  free(possibleMoves);

  printf("Move: \"%s\" Weight: %s\n",move[0],move[1]);
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
  p = 0; //Zählvariable für die möglichen Züge im Array
  for(int x = -1; x<2;x++){
    for(int y = -1; y<2;y++){
      if(!(x==0&&y==0) && abs(x)==abs(y) && (COLUMNS-1-i+x)>=0 && (j+y)>=0 && (COLUMNS-1-i+x)<=COLUMNS && (j+y)<=ROWS){
        printf("Moves: %i\n",p);
        //TODO schauen das es nicht über die Ränder hinaus überprüft

        possibleMoves[p] = malloc(sizeof(char*)*2);
        possibleMoves[p][0] = malloc(sizeof(char)*BUFFERLENGTH_MOVE);
        possibleMoves[p][1] = malloc(sizeof(char));

        //TODO nach den möglichen Zügen schauen und die Gewichtung vergeben

        sprintf(possibleMoves[p][0], "%c%i:%s", inttocolumn(j),COLUMNS-i,"00");
        sprintf(possibleMoves[p][1], "%i",JUMP);
        p++;
      }
    }
  }
  return possibleMoves;
}
