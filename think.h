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

//Externe Variablen
extern int fd[2];
extern struct serverinfo *serverinfo;                                           //Globales struct für die Serverinfos

//Funktionen deklarieren
void think(int sig);
char **bestMove(int i, int j);
char *bestMoveAll(int playernr);
char *maxWeightMove(char ***moves,int pieces);
char ***calcPossibleMoves(int i, int j);
void jump (int i, int j, char ***possibleMoves, int p);

#endif
