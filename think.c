#include "think.h"

void think(int sig){
  (void)sig;
  if(serverinfo->startcalc == 1){
    printfield();
    move bestmove = {.move = {0},.weight=0};
    time_t start = 0;
    time_t stop = 0;
    double calcTime = 0.0;

    printf("Calculating turn...");
    time(&start);
    bestmove = bestMoveAll(serverinfo->clientplayernr);
    printf("done\n");
    time(&stop);
    calcTime = difftime(stop, start);
    printf("Calculation was finished in %g seconds\n",calcTime);

    printf("Move: %s\n", bestmove.move);
    char space = ' ';
    int moveSize = 0;                                                           //Größe von Short move
    char* shortMove = NULL;
    shortMove = strtok(bestmove.move, &space);
    if(shortMove!=NULL)
      moveSize = strlen(shortMove);

    if(write (fd[1], shortMove, moveSize) != moveSize){                         //schreibt in die Pipe; höchstens 12 sprünge möglich
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
  movearray possibleMoves = {.moves = {{.move={0},.weight =0}},.count=0};
  possibleMoves = calcPossibleMoves(i,j,serverinfo->field,&possibleMoves,0,0,0,0);

  //Maximum der Gewichtung der gültigen Möglichkeiten
  if(possibleMoves.count>0){
    move bestmove = {.move = {0},.weight=0};
    bestmove = maxWeightMove(possibleMoves);
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
  movearray bestmoves = {.moves = {{.move={0},.weight =0}},.count=0};
  int x=0;
  for(int i = 0; i<ROWS;i++){
    for(int j = 0; j<COLUMNS;j++){
      if(playernr){
        if(isBlack(i,j,serverinfo->field)==1){
          bestmoves.moves[x] = bestMove(i,j);
          x++;
        }
      }else{
        if(isWhite(i,j,serverinfo->field)==1){
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
  srand((unsigned) time(NULL));
  for(int i = 0; i<moves.count;i++){
    if(moves.moves[i].weight>max){
      max = moves.moves[i].weight;
      maxIndex = i;
    }else if (moves.moves[i].weight==max){
      if((rand()%99)<50){
        max = moves.moves[i].weight;
        maxIndex = i;
      }
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
movearray calcPossibleMoves(int i, int j, char field[ROWS][COLUMNS][BUFFERLENGTH],movearray *possibleMoves, int p, int jump, int prevVZX, int prevVZY){
   //Variablen deklarieren/initialisieren
   char fieldcopy[ROWS][COLUMNS][BUFFERLENGTH]={{{0}}};

   int xStart, yStart, xEnd, yEnd, vzx, vzy;
   xStart = yStart = xEnd = yEnd = vzx = vzy = 0;

   //Suchgrenzen
   if(isQueen(i, j, field)==1){
     xStart = -8;
     yStart = -8;
     xEnd = 8;
     yEnd = 8;
   }else{
     xStart = -1;
     yStart = -1;
     xEnd = 2;
     yEnd = 2;
   }

   for(int x = xStart; x<xEnd; x++){
     for(int y = yStart; y<yEnd; y++){
       if(!(x==0&&y==0) && abs(x)==abs(y) && (i+x)>=0 && (j+y)>=0 && (i+x)<ROWS && (j+y)<COLUMNS){

         //Spielfeld kopieren
         for(int m = 0; m<ROWS; m++){
           for(int n = 0; n<COLUMNS; n++){
             memset(fieldcopy, 0, strlen(fieldcopy[m][n]));
             strcpy(fieldcopy[m][n], field[m][n]);
           }
         }

         switch (isAlly(i+x,j+y,fieldcopy)){
           case 0:  vzx = (int)(abs(x)/x);
                    vzy = (int)(abs(y)/y);
                    //Schauen das es nicht zurückspringt
                    if(!(prevVZX==(-1*vzx) &&prevVZY == (-1*vzy)) && (i+x+vzx)>=0 && (j+y+vzy)>=0 && (i+x+vzx)<ROWS && (j+y+vzy)<COLUMNS){
                      //Ist das daraufliegende Feld frei?
                      if(isFieldEmpty(i+x+vzx, j+y+vzy,fieldcopy)){
                        if(isQueen(i,j,fieldcopy)==1){
                          int obstacle = 0;
                          //Alle zwischenliegende Steine prüfen
                          for(int a = 0; a<abs(y);a++){
                            //Schauen das das es in den Grenzen liegt
                            if((i+((a+1)*vzx))>=0 && (i+((a+1)*vzx))<ROWS && (j+((a+1)*vzy))>=0 && (j+((a+1)*vzy))<COLUMNS){
                              //Ist das Feld leer?
                              if(!isFieldEmpty(i+((a+1)*vzx),j+((a+1)*vzy),fieldcopy)){
                                //Wenn das Feld nicht leer ist, prüfen ob es der zu überspringende Stein ist
                                if(i+((a+1)*vzx)!=i+x &&  j+((a+1)*vzy) != j+y){
                                  obstacle = 1;
                                }
                              }
                            }
                          }
                          if(obstacle==0){
                            //Werte im Move speichern
                            //Wenn noch nichts im Move steht dann Startposition hinzufügen
                            if(!strcmp(possibleMoves->moves[p].move, "")){
                              sprintf(possibleMoves->moves[p].move, "%c%i",inttocolumn(j),COLUMNS-i);
                            }
                            //Ziel zum Move hinzufügen
                            char destination[4] = {0};
                            sprintf(destination, ":%c%i", inttocolumn(j+y+vzy),COLUMNS-(i+x+vzx));
                            strcat(possibleMoves->moves[p].move, destination);
                            possibleMoves->moves[p].weight += JUMP;

                            //Übersprungerner Stein löschen und eigenen Stein auf das Feld hinter dem Überprungen setzen
                            fieldcopy[i+x][j+y][strlen(fieldcopy[i+x][j+y])-1]='\0';                                                         
                            strcpy(fieldcopy[i+x+vzx][j+y+vzy], fieldcopy[i][j]);
                            strcpy(fieldcopy[i][j], "");

                            jump = 1;
                            calcPossibleMoves(i+x+vzx, j+y+vzy,fieldcopy,possibleMoves,p,jump,vzx,vzy);
                            p++;
                          }
                        }else{
                          //Werte im Move speichern
                          //Wenn noch nichts im Move steht dann Startposition hinzufügen
                          if(!strcmp(possibleMoves->moves[p].move, "")){
                            sprintf(possibleMoves->moves[p].move, "%c%i",inttocolumn(j),COLUMNS-i);
                          }

                          //Ziel zum Move hinzufügen
                          char destination[4] = {0};
                          sprintf(destination, ":%c%i", inttocolumn(j+y+vzy),COLUMNS-(i+x+vzx));
                          strcat(possibleMoves->moves[p].move, destination);
                          possibleMoves->moves[p].weight += JUMP;

                          //Übersprungerner Stein löschen und eigenen Stein auf das Feld hinter dem Überprungen setzen
                          fieldcopy[i+x][j+y][strlen(fieldcopy[i+x][j+y])-1]='\0';          
                          strcpy(fieldcopy[i+x+vzx][j+y+vzy], fieldcopy[i][j]);
                          strcpy(fieldcopy[i][j], "");

                          //Ist eine Dame beim Sprung entstanden?
                          if((serverinfo->clientplayernr==0&&(i+x+vzx)==0)){
                            fieldcopy[i+x+vzx][j+y+vzy][strlen(fieldcopy[i+x+vzx][j+y+vzy])-1]='W';
                          }else if((serverinfo->clientplayernr==1&&i+x+vzx==ROWS-1)){
                            fieldcopy[i+x+vzx][j+y+vzy][strlen(fieldcopy[i+x+vzx][j+y+vzy])-1]='B';
                          }

                          jump = 1;
                          calcPossibleMoves(i+x+vzx, j+y+vzy,fieldcopy,possibleMoves,p,jump,vzx,vzy);
                          p++;
                        }
                      }
                    }
                    break;
           case -1: if(isQueen(i, j, fieldcopy)==1 && jump ==0){
                      int obstacle = 0;
                      //Alle zwischenliegende Steine anschauen
                      for(int a = 0; a<abs(y)-1;a++){
                        //Grenzen beachten
                        if((i+((a+1)*vzx))>=0 && (i+((a+1)*vzx))<ROWS && (j+((a+1)*vzy))>=0 && (j+((a+1)*vzy))<COLUMNS){
                          //Prüfen ob Feld leer
                          if(!isFieldEmpty(i+((a+1)*vzx),j+((a+1)*vzy),fieldcopy)){
                            obstacle = 1;
                          }
                        }
                      }

                      if(obstacle==0){
                        //Speichern des Zuges
                        sprintf(possibleMoves->moves[p].move, "%c%i:%c%i", inttocolumn(j),COLUMNS-i,inttocolumn(j+y),COLUMNS-(i+x));
                        possibleMoves->moves[p].weight = MOVE;
                        p++;
                      }
                    }else if(((i>(i+x) && serverinfo->clientplayernr == 0)||(i<(i+x) && serverinfo->clientplayernr == 1))&&jump==0){
                      //Speichern des Zuges
                      sprintf(possibleMoves->moves[p].move, "%c%i:%c%i", inttocolumn(j),COLUMNS-i,inttocolumn(j+y),COLUMNS-(i+x));
                      possibleMoves->moves[p].weight = MOVE;
                      p++;
                    }
                    break;
           default: break;
         }
       }
     }
   }

   prevVZX = 0;
   prevVZY = 0;
   jump = 0;
   possibleMoves->count = p;
   return *possibleMoves;
}
