#ifndef fieldutil
#define fieldutil

//Bibliotheken einbinden
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

//Module einbinden
#include "parameter.h"
#include "sharedMemory.h"
#include "responseHandler.h"
#include "processRequest.h"

//Funktionen deklarieren
int isWhite(int i, int j);
int isBlack(int i, int j);
int columntoint(char column);
char inttocolumn(int col);
void printfield(void);

#endif
