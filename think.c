#include "think.h"

void think(int sig){
  (void)sig;
  if(serverinfo->startcalc == 1){
    printfield();
    move bestmove = {.move = {0},.weight=0};
    bestmove = bestMoveAll(serverinfo->clientplayernr);
    printf("Spielzug: %s\n", bestmove.move);
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
  }else{
    printf("Kein Zug vorhanden\n");
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
   printf("Calculate possible moves for piece(%c:%i)\n",inttocolumn(j),COLUMNS-i);
   //Variablen deklarieren/initialisieren
   char fieldcopy[ROWS][COLUMNS][BUFFERLENGTH] = {{{0}}};
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
                    if(!(prevVZX==(-1*vzx) &&prevVZY==(-1*vzy))){
                      //Ist das daraufliegende Feld frei?
                      if(isFieldEmpty(i+x+vzx, j+y+vzy,fieldcopy) && (i+x+vzx)>=0 && (j+y+vzy)>=0 && (i+x+vzx)<ROWS && (j+y+vzy)<COLUMNS){
                        if(isQueen(i,j,fieldcopy)==1){
                          printf("Dame Sprung\n");
                          int obstacle = 0;
                          printf("Zu prüfende Zwischensteine: %i\n",abs(y)-1);
                          for(int a = 0; a<abs(y)-1;a++){
                            printf("Teste ob Stein im Weg bei %c:%i\n", inttocolumn(j+((a+1)*vzy)), COLUMNS-(i+((a+1)*vzx)));
                            if(!isFieldEmpty(i+((a+1)*vzx),j+((a+1)*vzy),fieldcopy)){
                              if(i+((a+1)*vzx)!=j+y &&  j+((a+1)*vzy) != i+x){
                                printf("Stein im Weg bei %c:%i\n", inttocolumn(j+((a+1)*vzy)), COLUMNS-(i+((a+1)*vzx)));
                                obstacle = 1;
                              }else{
                                printf("Zu überspringender Stein bei %c:%i\n", inttocolumn(j+((a+1)*vzy)), COLUMNS-(i+((a+1)*vzx)));
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

                            printf("Möglicher Damesprung mit Gewicht %s %i\n",possibleMoves->moves[p].move, possibleMoves->moves[p].weight);

                            //Übersprungerner Stein löschen und eigenen Stein auf das Feld hinter dem Überprungen setzen
                            strcpy(fieldcopy[i+x][j+y], "");
                            strcpy(fieldcopy[i+x+vzx][j+y+vzy], fieldcopy[i][j]);
                            strcpy(fieldcopy[i][j], "");

                            jump = 1;
                            calcPossibleMoves(i+x+vzx, j+y+vzy,fieldcopy,possibleMoves,p,jump,vzx,vzy);
                            p++;
                          }
                        }else{
                          printf("Normaler Sprung\n");
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

                          printf("Möglicher Sprung mit Gewicht %s %i gespeichert in %i\n",possibleMoves->moves[p].move, possibleMoves->moves[p].weight,p);

                          //Übersprungerner Stein löschen und eigenen Stein auf das Feld hinter dem Überprungen setzen
                          strcpy(fieldcopy[i+x][j+y], "");
                          strcpy(fieldcopy[i+x+vzx][j+y+vzy], fieldcopy[i][j]);
                          strcpy(fieldcopy[i][j], "");

                          //Ist eine Dame beim Sprung entstanden?
                          if((serverinfo->clientplayernr==0&&(i+x+vzx)==0)){
                            printf("Weiße Dame bei %c%i erstellt\n",inttocolumn(j+(2*y)),COLUMNS-(i+(2*x)));
                            fieldcopy[i+x+vzx][j+y+vzy][strlen(fieldcopy[i+x+vzx][j+y+vzy])-1]='W';
                          }else if((serverinfo->clientplayernr==1&&i+x+vzx==ROWS-1)){
                            printf("Schwarze Dame bei %c%i erstellt\n",inttocolumn(j+(2*y)),COLUMNS-(i+(2*x)));
                            fieldcopy[i+x+vzx][j+y+vzy][strlen(fieldcopy[i+x+vzx][j+y+vzy])-1]='B';
                          }

                          jump = 1;
                          calcPossibleMoves(i+x+vzx, j+y+vzy,fieldcopy,possibleMoves,p,jump,vzx,vzy);
                          p++;
                        }
                      }
                    }else{
                      printf("FALSCHE RICHTUNG\n");
                    }

                    break;
           case -1: if(isQueen(i, j, fieldcopy)==1 && jump ==0){
                      printf("Dame Zug\n");
                      //Speichern des Zuges
                      int obstacle = 0;
                      printf("Zu prüfende Zwischensteine: %i\n",abs(y)-1);
                      for(int a = 0; a<abs(y)-1;a++){
                        printf("Teste ob Stein im Weg bei %c:%i\n", inttocolumn(j+((a+1)*vzy)), COLUMNS-(i+((a+1)*vzx)));
                        if(!isFieldEmpty(i+((a+1)*vzx),j+((a+1)*vzy),fieldcopy)){
                          printf("Stein im Weg bei %c:%i\n", inttocolumn(j+((a+1)*vzy)), COLUMNS-(i+((a+1)*vzx)));
                          obstacle = 1;
                        }
                      }
                      if(obstacle==0){
                        sprintf(possibleMoves->moves[p].move, "%c%i:%c%i", inttocolumn(j),COLUMNS-i,inttocolumn(j+y),COLUMNS-(i+x));
                        possibleMoves->moves[p].weight = MOVE;
                        printf("Möglicher Zug mit Gewicht %s %i gespeichert in %i\n",possibleMoves->moves[p].move, possibleMoves->moves[p].weight,p);
                        p++;
                      }
                    }else{
                      printf("Normaler Zug\n");
                      if(((i>(i+x) && serverinfo->clientplayernr == 0)||(i<(i+x) && serverinfo->clientplayernr == 1))&&jump==0){
                       //Speichern des Zuges
                       sprintf(possibleMoves->moves[p].move, "%c%i:%c%i", inttocolumn(j),COLUMNS-i,inttocolumn(j+y),COLUMNS-(i+x));
                       possibleMoves->moves[p].weight = MOVE;
                       printf("Möglicher Zug mit Gewicht %s %i gespeichert in %i\n",possibleMoves->moves[p].move, possibleMoves->moves[p].weight,p);
                       p++;
                      }
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
