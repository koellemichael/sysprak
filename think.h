#ifndef thinking
#define thinking

//Bibliotheken einbinden
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

//Module einbinden
#include "parameter.h"
#include "sharedMemory.h"
#include "responseHandler.h"
#include "field.h"

//Variablen einbinden
int p;
char space;                                                                     //Delimiter: Leeres Zeichen
char* shortMove;                                                                //Move ohne Bufferspace danach
int moveSize;                                                                   //Größe von Short move

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


//Funktionen deklarieren
void think(int sig);
move maxWeightMove(movearray moves);
movearray calcPossibleMoves(int i, int j);
move bestMoveAll(int playernr);
move bestMove(int i, int j);
void jump (int i, int j, movearray *possibleMoves, int p);



#endif
