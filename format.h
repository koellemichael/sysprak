#ifndef form                                                              //Wenn das Makro noch nicht existiert
#define form                                                              //Definiere das Makro

//Bibliotheken einbinden
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Module einbinden
#include "processRequest.h"
#include "parameter.h"
#include "sharedMemory.h"

//Externe Variablen
extern struct serverinfo *serverinfo;
extern int *shmid_player;

//Funktionen deklarieren
char *format (char *input);


#endif
