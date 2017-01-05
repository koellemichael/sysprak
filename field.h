#ifndef printing
#define printing

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
char inttocolumn(int col);
void printfield(void);

#endif
