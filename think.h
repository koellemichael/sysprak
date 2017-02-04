#ifndef thinking
#define thinking

//Bibliotheken einbinden
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

//Module einbinden
#include "parameter.h"
#include "sharedMemory.h"
#include "responseHandler.h"
#include "field.h"

//Variablen einbinden
int p;
int vza;                                                                        //Vorzeichen von a
int vzb;                                                                        //Vorzeichen von b
int obstacle;                                                                      //testet, ob Steine im Weg liegen
//Externe Variablen
extern int fd[2];
extern struct serverinfo *serverinfo;                                           //Globales struct für die Serverinfos

typedef struct {
  char move[BUFFERLENGTH_MOVE];
  int weight;
}move;

typedef struct {
  move moves[BUFFERLENGTH];
  int count;
}movearray;


//Funktionen deklarieren/Users/KatharinaWinter/Desktop/Systempraktikum/sysprak/think.h
void think(int sig);
move maxWeightMove(movearray moves);
movearray calcPossibleMoves(int i, int j);
move bestMoveAll(int playernr);
move bestMove(int i, int j);
void jump (int i, int j, movearray *possibleMoves, int p,char fieldcopy[ROWS][COLUMNS][BUFFERLENGTH]);



#endif
