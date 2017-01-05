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
#include "printField.h"

//Variablen einbinden
char* move;

//Externe Variablen
extern int fd[2];
extern struct serverinfo *serverinfo;                                           //Globales struct für die Serverinfos

//Funktionen deklarieren
void think(int sig);
//void printfield(void);

#endif
