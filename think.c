#include "think.h"

void printfieldcopy(char fieldcopy[ROWS][COLUMNS][BUFFERLENGTH]){
  for(int i = 0; i<ROWS; i++){
    for(int j = 0; j<COLUMNS; j++){
      if(fieldcopy[i][j][strlen(fieldcopy[i][j])-1]==0){
        printf("  ");
      }else{
        printf("%c ", fieldcopy[i][j][strlen(fieldcopy[i][j])-1]);
      }

    }
    printf("\n");
  }
}

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
    move bestmove = maxWeightMove(possibleMoves);
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
  movearray bestmoves;
  //memset(bestmoves.moves, 0, BUFFERLENGTH);
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
movearray calcPossibleMoves(int i, int j){
  printf("Calculate possible moves for piece(%c:%i)\n",inttocolumn(j),COLUMNS-i);
  char fieldcopy[ROWS][COLUMNS][BUFFERLENGTH];

  movearray possibleMoves;
  p = 0; //Zählvariable für die möglichen Züge im Array
  if(isQueen(i,j,serverinfo->field)==1){
    for(int a=-ROWS; a<ROWS; a++){
      for (int b=-COLUMNS; b<COLUMNS; b++){
        if(!(a==0&&b==0) && abs(a)==abs(b) && (i+a)>0 && (j+b)>0 && (i+a)<ROWS-1 && (j+b)<COLUMNS-1){
          //Spielfeld kopieren
          for(int m = 0; m<ROWS; m++){
            for(int n = 0; n<COLUMNS; n++){
              memset(fieldcopy, 0, strlen(fieldcopy[m][n]));
              strcpy(fieldcopy[m][n], serverinfo->field[m][n]);
            }
          }
            switch (isAlly(i+a,j+b,fieldcopy)){
                case 0:  vza= (int)(abs(a)/a);                                            //Vorzeichen: wenn a negativ, dann -1 addiert
                         vzb= (int)(abs(b)/b);                                            //Vorzeichen: wenn b positiv, dann +1 addiert
                        if(isFieldEmpty(i+(a+vza), j+(b+vzb),fieldcopy)){
                            obstacle = 0;                                                      //Testet, ob Steine im Weg liegen
                            printf("Testi Test");
                            for(int c=1; c < abs(a); c++){                                       //Testen, ob Bei Damensprung Steine im Weg liegen
                                printf("ZWISCHENFELDPOSITION: %i, %c\n", (i+a)-vza*c, inttocolumn((j+b)-vzb*c));
                                if(!(isFieldEmpty((i+a)-vza*c, (j+b)-vzb*c,fieldcopy))){             //Geht den Weg ab, den dame überspringt
                                    printf("FELD NICHT LEER");
                                    obstacle=1;                                            //1, wenn Steine im Weg liegen
                                }
                            }
                          if(obstacle==0){                                                    //Nur wenn check != 1, also keine Steine im Weg
                          printf("Kathis Test: %i %i, %c \n", j+b, j+b+vzb, inttocolumn(j+b+vzb));
                          sprintf(possibleMoves.moves[p].move, "%c%i:%c%i", inttocolumn(j), ROWS-i, inttocolumn(j+(b+vzb)), (ROWS-i)-(a+vza));
                          possibleMoves.moves[p].weight = JUMP;
                          strcpy(fieldcopy[i+a][j+b], "");
                          strcpy(fieldcopy[i+(a+vza)][j+(b+vzb)], fieldcopy[i][j]);
                          strcpy(fieldcopy[i][j], "");
                          printf("Möglicher Damesprung %s %i\n",possibleMoves.moves[p].move, possibleMoves.moves[p].weight);
                          jump(i+(a+vza), j+(b+vzb), &possibleMoves,p, fieldcopy);
                          p++;
                          }
                        }
                        break;
                case -1:if((i>(i+a && serverinfo->clientplayernr == 0)||(i<(i+a && serverinfo->clientplayernr == 1)))){
                          sprintf(possibleMoves.moves[p].move, "%c%i:%c%i", inttocolumn(j),COLUMNS-i,inttocolumn(j+b),COLUMNS-(i+a));
                          possibleMoves.moves[p].weight = MOVE;
                          p++;
                        }
                        break;

                case 1: break;

                default:perror("Unknown piece");
                        exit(EXIT_FAILURE);
                        break;
          }
        }
      }
    }
  }else{
    for(int x = -1; x<2;x++){
      for(int y = -1; y<2;y++){
        if(!(x==0&&y==0) && abs(x)==abs(y) && (i+x)>=0 && (j+y)>=0 && (i+x)<ROWS && (j+y)<COLUMNS){
          //Spielfeld kopieren
          for(int m = 0; m<ROWS; m++){
            for(int n = 0; n<COLUMNS; n++){
              memset(fieldcopy, 0, strlen(fieldcopy[m][n]));
              strcpy(fieldcopy[m][n], serverinfo->field[m][n]);
            }
          }
          memset(possibleMoves.moves[p].move,0,strlen(possibleMoves.moves[p].move));
          possibleMoves.moves[p].weight = 0;
          switch (isAlly(i+x,j+y,fieldcopy)){
            case 0:     if(isFieldEmpty(i+(2*x), j+(2*y),fieldcopy) && (i+(2*x))>=0 && (j+(2*y))>=0 && (i+(2*x))<ROWS && (j+(2*y))<COLUMNS){
                          sprintf(possibleMoves.moves[p].move, "%c%i:%c%i", inttocolumn(j),COLUMNS-i,inttocolumn(j+(2*y)),COLUMNS-(i+(2*x)));
                          possibleMoves.moves[p].weight = JUMP;
                          
                          printf("Möglicher Sprung mit Gewicht %s %i\n",possibleMoves.moves[p].move, possibleMoves.moves[p].weight);
                          strcpy(fieldcopy[i+x][j+y], "");

                          strcpy(fieldcopy[i+(2*x)][j+(2*y)], fieldcopy[i][j]);
                          strcpy(fieldcopy[i][j], "");
                          if((serverinfo->clientplayernr==0&&(i+(2*x))==0)){
                            fieldcopy[i+(2*x)][j+(2*y)][strlen(fieldcopy[i+(2*x)][j+(2*y)]-1)]='W';
                          }else if((serverinfo->clientplayernr==1&&i+(2*x)==ROWS-1)){
                            fieldcopy[i+(2*x)][j+(2*y)][strlen(fieldcopy[i+(2*x)][j+(2*y)]-1)]='B';
                          }
                          jump(i+(2*x), j+(2*y), &possibleMoves,p,fieldcopy);
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
        }
      }
    }
  }
  possibleMoves.count = p;
  return possibleMoves;
}

void jump (int i, int j, movearray *possibleMoves, int p, char fieldcopy[ROWS][COLUMNS][BUFFERLENGTH]){
  if(isQueen(i, j,fieldcopy)==1){
       for(int a = -8; a<8;a++){
          for(int b = -8; b<8; b++){
            if(!(a==0&&b==0) && abs(a)==abs(b)
              && (i+a)>0 && (j+b)>0
              && (i+a)<ROWS-1 && (j+b)<COLUMNS-1
              && isAlly(i+a,j+b,fieldcopy)==0){                               //TODO einfügen von nicht rückwärts springen
              vza= (int)(abs(a)/a);                                            //Vorzeichen: wenn a negativ, dann -1 addiert
              vzb= (int)(abs(b)/b);                                            //Vorzeichen: wenn b positiv, dann +1 addiert
              if(isFieldEmpty(i+(a+vza), j+(b+vzb),fieldcopy)
                 && (i+a+vza)>=0 && (j+b+vzb)>=0
                 && (i+a+vza)<ROWS-1 && (j+b+vzb)<COLUMNS){

                //Test: Liegen beim zweiten Sprung Steine im Weg  
                obstacle = 0;
                for(int c=1; c < abs(a); c++){                                       //Testen, ob Bei Damensprung Steine im Weg liegen
                   printf("ZWISCHENFELDPOSITION: %i, %c\n", (i+a)-vza*c, inttocolumn((j+b)-vzb*c));
                   if(!(isFieldEmpty((i+a)-vza*c, (j+b)-vzb*c,fieldcopy))){             //Geht den Weg ab, den dame überspringt
                       printf("FELD NICHT LEER");
                       obstacle=1;                                            //1, wenn Steine im Weg liegen
                                }           
                   }
                //Wenn keine Steine im Weg liegen darf gesprungen werden
                if(obstacle==0){

                char *onemore = malloc(sizeof(char)*BUFFERLENGTH_MOVE);
                memset(onemore, 0, strlen(onemore));
                sprintf(onemore, ":%c%i", inttocolumn((j)+(b+vzb)), (ROWS-i)-(a+vza));

                strcat(possibleMoves->moves[p].move, onemore);
                possibleMoves->moves[p].weight += JUMP;
                printf("Weiterer möglicher Damesprung mit Gewicht %s %i\n",possibleMoves->moves[p].move, possibleMoves->moves[p].weight);
                sprintf(onemore, ":%c%i", inttocolumn(j+(b+vzb)), (ROWS-i)-(a+vza));
                free(onemore);
                strcpy(fieldcopy[i+a][j+b], "");
                strcpy(fieldcopy[i+(a+vza)][j+(b+vzb)], fieldcopy[i][j]);
                strcpy(fieldcopy[i][j], "");
                printfieldcopy(fieldcopy);
                jump(i+(a+vza), j+(b+vzb), possibleMoves,p, fieldcopy);
                p++;
                }
              }
            }
          }
        }
  }else{
    for(int x = -1; x<2;x++){
      for(int y = -1; y<2;y++){
        if(!(x==0&&y==0) && abs(x)==abs(y)
          && (i+x)>=0 && (j+y)>=0
          && (i+x)<ROWS && (j+y)<COLUMNS
          && isAlly(i+x,j+y,fieldcopy)==0 && isFieldEmpty(i+(2*x), j+(2*y),fieldcopy)
          && (i+(2*x))>=0 && (j+(2*y))>=0
          && (i+(2*x))<ROWS && (j+(2*y))<COLUMNS){
            int lastrow = possibleMoves->moves[p].move[strlen(possibleMoves->moves[p].move)-4] - '0';         //lastrow-i positiv: nach oben; negativ nach unten
            int lastcol = columntoint(possibleMoves->moves[p].move[strlen(possibleMoves->moves[p].move)-5]);  //lastcol-j positiv: nach links; negativ nach rechts (?)
            printf("lastrow = %i\n", lastrow);
            printf("i = %i\n", i);
            printf("abs(lastrow-i)) = %i\n", abs(lastrow-i));
            printf("(((abs(lastrow-i))/lastrow-i) = %i\n", (((abs(lastrow-i))/(lastrow-i))));
            printf("(-1)*(abs(i-x))/i-x) = %i\n", (-1)*((abs(i-x))/i-x));
            printf("(((abs(lastcol-j))/lastcol-j) = %i\n", (((abs(lastcol-j))/(lastcol-j))));
            printf("(-1)*(abs(j-y))/j-y) = %i\n", (-1)*((abs(j-y))/j-y));
            
            if(!(((abs(lastrow-i))/(lastrow-i))==(-1)*((abs(i-x))/(i-x)))&&(((abs(lastcol-j))/(lastcol-j))==(-1)*((abs(j-y))/(j-y)))){
              printf("HIER GEHTS WEITER!\n");
              char *onemore = malloc(sizeof(char)*BUFFERLENGTH_MOVE);
              memset(onemore, 0, strlen(onemore));
              sprintf(onemore, ":%c%i",inttocolumn(j+(2*y)),COLUMNS-(i+(2*x)));

              strcat(possibleMoves->moves[p].move, onemore);
              possibleMoves->moves[p].weight += JUMP;
              printf("Weiterer möglicher Sprung mit Gewicht %s %i\n",possibleMoves->moves[p].move, possibleMoves->moves[p].weight);
              strcpy(fieldcopy[i+x][j+y], "");
              strcpy(fieldcopy[i+(2*x)][j+(2*y)], fieldcopy[i][j]);
              strcpy(fieldcopy[i][j], "");
              if((serverinfo->clientplayernr==0&&(i+(2*x))==0)){
                printf("Weiße QUEEN %c%i erstellt\n",inttocolumn(j+(2*y)),COLUMNS-(i+(2*x)));
                fieldcopy[i+(2*x)][j+(2*y)][strlen(fieldcopy[i+(2*x)][j+(2*y)])-1]='W';
              }else if((serverinfo->clientplayernr==1&&i+(2*x)==ROWS-1)){
                printf("Schwarze QUEEN %c%i erstellt\n",inttocolumn(j+(2*y)),COLUMNS-(i+(2*x)));
                fieldcopy[i+(2*x)][j+(2*y)][strlen(fieldcopy[i+(2*x)][j+(2*y)])-1]='B';
            }
            free(onemore);
            jump(i+(2*x), j+(2*y), possibleMoves, p,fieldcopy);
            p++;
          }
        }
      }
    }
  }
}
