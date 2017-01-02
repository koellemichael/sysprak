#ifndef thin                                                                    //Wenn das Makro noch nicht existiert
#define thin                                                                    //Definiere das Makro

//Bibliotheken einbinden
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

//Module einbinden
#include "parameter.h"
#include "sharedMemory.h"
#include "responseHandler.h"

//Globale Variablen
extern struct serverinfo *serverinfo;                                                  //Gobales struct für die Serverinfos

//Funktionen deklarieren
void think(int sig);
void printServerInfo(void);

#endif
