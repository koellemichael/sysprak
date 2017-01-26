#include "think.h"

void think(int sig){
  (void)sig;
  if(serverinfo->startcalc == 1){
    printfield();
    //move = "A3:B4\0";
    move move = bestMoveAll(serverinfo->clientplayernr);
    printf("Spielzug: %s\n", move.move);
    //strcat(move, "\n");
    //move = "\0";
    space = ' ';
    shortMove = strtok(move.move, &space);
    moveSize = strlen(shortMove);
    //printf("Movesize: %d \n", moveSize);
    //printf("strtok: %s\n", shortMove);
    if(write (fd[1], shortMove, moveSize) != moveSize){     //schreibt in die Pipe; höchstens 12 sprünge möglich
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
move bestMove(int i, int j){
  //Gültige Möglichkeiten für Stein(i,j)
  movearray possibleMoves = calcPossibleMoves(i,j);

  //Maximum der Gewichtung der gültigen Möglichkeiten
  if(possibleMoves.count>0){
    /*int maxIndex = 0;
    int max = 0;
    for(int x = 0; x<possibleMoves.count;x++){
      if(possibleMoves.moves[x].weight>max){
        max = possibleMoves.moves[x].weight;
        maxIndex = x;
      }
    }*/
    move bestmove = maxWeightMove(possibleMoves);
    //printf("Best move for %c%i is: \"%s\"  with weight: %i\n",inttocolumn(j),COLUMNS-i,possibleMoves.moves[maxIndex].move,possibleMoves.moves[maxIndex].weight);
    return bestmove;
  }

  return *possibleMoves.moves;
}

/**
 * Berechnet den besten nächsten Speilzug aus
 * all den berechneten besten Zügen der Steine eines Players.
 * @param playernr Playernummer des zu berechnenden besten Zugs
 * @return Bester möglicher Zug als String
 */
move bestMoveAll(int playernr){
  movearray bestmoves;
  int x=0;
  for(int i = 0; i<ROWS;i++){
    for(int j = 0; j<COLUMNS;j++){
      if(playernr){
        if(isBlack(i,j)==1){
          bestmoves.moves[x] = bestMove(i,j);
          x++;
        }
      }else{
        if(isWhite(i,j)==1){
          bestmoves.moves[x] = bestMove(i,j);
          x++;
        }
      }
    }
  }
  bestmoves.count = x;
  return maxWeightMove(bestmoves);
}

/**
 * Berechnet den Zug mit der maximalen Gewichtung.
 * @param moves Array der Spielzüge und zugehörigen Gewichtungen
 * @param pieces Anzahl der in moves enthaltenen Spielzüge
 * @return Den nach der Gewichtung besten Zug
 */
move maxWeightMove(movearray moves){
  int maxIndex = 0;
  int max = 0;
  for(int i = 0; i<moves.count;i++){
    if(moves.moves[i].weight>max){
      max = moves.moves[i].weight;
      maxIndex = i;
    }
  }

  //Besten Spielzug zurückgeben
  return moves.moves[maxIndex];
}

/**
 * Berechnet die möglichen gültigen Spielzüge für einen Stein(i,j)
 * @param i Zeile des Steins
 * @param j Spalte des Steins
 * @return Array mit allen mögichen Spielzügen und zugehörigen Gewichtungen
 */
movearray calcPossibleMoves(int i, int j){
  printf("Calculate possible moves for piece(%c:%i)\n",inttocolumn(j),COLUMNS-i);
  movearray possibleMoves;
  p = 0; //Zählvariable für die möglichen Züge im Array
  for(int x = -1; x<2;x++){
    for(int y = -1; y<2;y++){
      if(!(x==0&&y==0) && abs(x)==abs(y) && (COLUMNS-1-(i+x))>0 && (j+y)>0 && (COLUMNS-(i+x))<COLUMNS && (j+y)<ROWS){
        memset(possibleMoves.moves[p].move,0,strlen(possibleMoves.moves[p].move));
        possibleMoves.moves[p].weight = 0;
        if(!(isQueen(i,j))){
          switch (isAlly(i+x,j+y)) {
            case 0:   if(isFieldEmpty(i+(2*x), j+(2*y)) && inttocolumn(j+2+y) != 'x'){
                      sprintf(possibleMoves.moves[p].move, "%c%i:%c%i", inttocolumn(j),COLUMNS-i,inttocolumn(j+(2*y)),COLUMNS-(i+(2*x)));
                      possibleMoves.moves[p].weight = JUMP;
                      jump(i+(2*x), j+(2*y), &possibleMoves,p);
                      printf("Möglicher Sprung mit Gewicht %s %i\n",possibleMoves.moves[p].move, possibleMoves.moves[p].weight);
                      p++;
                    }
                    break;
            case -1:  if((i>(i+x) && serverinfo->clientplayernr == 0)||(i<(i+x) && serverinfo->clientplayernr == 1)){
                      sprintf(possibleMoves.moves[p].move, "%c%i:%c%i", inttocolumn(j),COLUMNS-i,inttocolumn(j+y),COLUMNS-(i+x));
                      possibleMoves.moves[p].weight = MOVE;
                      p++;
                    }
                    break;
            case 1:   break;
            default:  perror("Unknown piece");
                    exit(EXIT_FAILURE);
                    break;
          } 
        } else if(isQueen(i,j)){
          for(int a=-8; a<8; a++){
            for (int b=-8; b<8; b++){
              if(!(a==0&&b==0) && abs(a)==abs(b) && (COLUMNS-1-(a+x))>0 && (b+y)>0 && (COLUMNS-(i+a))<COLUMNS && (j+b)<ROWS){
                switch (isAlly(i+a,j+b)){                                                                                                    
                  case 0:   if(isFieldEmpty(i+a, j+b)){   //TODO RICHTIGES FELD BERECHNEN +/- JE NACH RECHT/LINKS/OBEN/UNTEN es fehlt ein +1 (je nach Richtung)
                            sprintf(possibleMoves.moves[p].move, "%c%i:%c%i", inttocolumn(j),COLUMNS-i,inttocolumn(j+a),COLUMNS-(i+b)); //TODO anpassen wir drüber
                            possibleMoves.moves[p].weight = JUMP;
                            jump(i+a, j+b, &possibleMoves,p);
                            printf("Möglicher Sprung %s %i\n",possibleMoves.moves[p].move, possibleMoves.moves[p].weight);
                            p++;
                          }
                          break;
                  case -1:  if((i>(i+a && serverinfo->clientplayernr == 0)||(i<(i+a && serverinfo->clientplayernr == 1)))){
                            sprintf(possibleMoves.moves[p].move, "%c%i:%c%i", inttocolumn(j),COLUMNS-i,inttocolumn(j+y),COLUMNS-(i+x));
                            possibleMoves.moves[p].weight = MOVE;
                            p++;
                          }

                          break;

                  case 1:   break;

                  default:  perror("Unknown piece");
                            exit(EXIT_FAILURE);
                            break;
                }  
              }
            }
          }
        }
      }
    }
  }
  possibleMoves.count = p;
  return possibleMoves;
}


void jump (int i, int j, movearray *possibleMoves, int p){
  printf("jump1\n");
  for(int x = -1; x<2;x++){
    for(int y = -1; y<2;y++){
      if(!(x==0&&y==0) && abs(x)==abs(y)
      && (COLUMNS-1-(i+x))>0 && (j+y)>0
      && (COLUMNS-(i+x))<COLUMNS && (j+y)<ROWS
      && isAlly(i+x,j+y) == 0 && isFieldEmpty(i+(2*x), j+(2*y))){

        char *onemore = malloc(sizeof(char)*BUFFERLENGTH_MOVE);
        memset(onemore, 0, strlen(onemore));
        sprintf(onemore, ":%c%i",inttocolumn(j+(2*y)),COLUMNS-(i+(2*x)));

        strcat(possibleMoves->moves[p].move, onemore);
        possibleMoves->moves[p].weight += JUMP;

        sprintf(serverinfo->field[i][j], " s");
        free(onemore);
        if(((j+y)<ROWS-1)||(j+y)>0){
          printf("jump2\n");
          jump(i+(2*x), j+(2*y), possibleMoves, p);
        }
      }
    }
  }
}
