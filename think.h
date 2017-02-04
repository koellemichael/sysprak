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
movearray calcPossibleMoves(int i, int j, char field[ROWS][COLUMNS][BUFFERLENGTH],movearray *possibleMoves, int p, int jump, int prevVZX, int prevVZY);
move bestMoveAll(int playernr);
move bestMove(int i, int j);

#endif
