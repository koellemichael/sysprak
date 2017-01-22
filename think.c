#include "think.h"

void think(int sig){
  (void)sig;
  if(serverinfo->startcalc == 1){
    printfield();
    //move = "A3:B4\0";
    char *move = bestMoveAll(serverinfo->clientplayernr);
    printf("Spielzug: %s\n", move);
    //strcat(move, "\n");
    //move = "\0";

    if((write (fd[1], move, sizeof(move))) != sizeof(move)){                    //schreibt in die Pipe; höchstens 12 sprünge möglich
        perror("Error trying to write into the pipe");
        exit (EXIT_FAILURE);
    }
    free(move);
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
  //Gültige Möglichkeiten für Stein(i,j)
  char **move = NULL;
  char ***possibleMoves = calcPossibleMoves(i,j);
  //Wenn es mind. einen möglichen Zug gibt
  if(p>0){
    //maximum der gewichtung der gültigen möglichkeiten
    int maxIndex = 0;
    int max = 0;
    int x = 0;
    for(x = 0; x<p;x++){
      if(atoi(possibleMoves[x][1])>max){
        max = atoi(possibleMoves[x][1]);
        maxIndex = x;
      }
    }
    move = malloc(sizeof(char*)*2);
    move[0] = malloc(sizeof(char)*BUFFERLENGTH_MOVE); //MOVE STRING
    move[1] = malloc(sizeof(char)*BUFFERLENGTH_MOVE); //GEWICHTUNG
    strcpy(move[0], possibleMoves[maxIndex][0]);
    strcpy(move[1], possibleMoves[maxIndex][1]);
    printf("Move: \"%s\" Weight: %s\n",move[0],move[1]);
  }

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
    if(moves[i]!=NULL){
      if(atoi(moves[i][1])>max){
        max = atoi(moves[i][1]);
        maxIndex = i;
      }
    }
  }

  char *bestmove = malloc(sizeof(char)*BUFFERLENGTH_MOVE);
  strcpy(bestmove, *moves[maxIndex]);
  //Speicher freigeben
  for(int i = 0; i<pieces;i++){
    if(moves[i]!=NULL){
      free(moves[i][0]);
      free(moves[i][1]);
    }
  }
  for(int i = 0; i<2;i++){
    if(moves[i]!=NULL)
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
      if(!(x==0&&y==0) && abs(x)==abs(y) && (COLUMNS-1-(i+x))>0 && (j+y)>0 && (COLUMNS-1-(i+x))<COLUMNS && (j+y)<ROWS){
        possibleMoves[p] = malloc(sizeof(char*)*2);
        possibleMoves[p][0] = malloc(sizeof(char)*BUFFERLENGTH_MOVE);
        possibleMoves[p][1] = malloc(sizeof(char));
        //TODO nach den möglichen Zügen schauen und die Gewichtung vergeben
        switch (isAlly(i+x,j+y)) {
          case 0:   //printf("%c%i Gegner Stein\n",inttocolumn(j+y),COLUMNS-(i+x));
                    if(isFieldEmpty(i+(2*x), j+(2*y))){
                      //printf("springbar\n");
                      sprintf(possibleMoves[p][0], "%c%i:%c%i", inttocolumn(j),COLUMNS-i,inttocolumn(j+(2*y)),COLUMNS-(i+(2*x)));
                      sprintf(possibleMoves[p][1], "%i",JUMP);
                      p++;
                      //TODO prüfen ob man noch mal springen kann
                    }
                    break;
          case -1:  //printf("%c%i Leeres Feld %i\n",inttocolumn(j+y),COLUMNS-(i+x),i>(i+x));
                    if(i>(i+x)){
                      sprintf(possibleMoves[p][0], "%c%i:%c%i", inttocolumn(j),COLUMNS-i,inttocolumn(j+y),COLUMNS-(i+x));
                      sprintf(possibleMoves[p][1], "%i",MOVE);
                      p++;
                    }
                    break;
          case 1:   //printf("%c%i Unser Stein\n",inttocolumn(j+y),COLUMNS-(i+x));
                    break;
          default:  perror("Unknown piece");
                    exit(EXIT_FAILURE);
                    break;
        }
      }
    }
  }
  return possibleMoves;
}
