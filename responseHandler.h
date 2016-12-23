#ifndef responseHdl                                                             //Wenn das Makro noch nicht existiert
#define responseHdl                                                             //Definiere das Makro

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
extern char *gameid;                                                            //gameid. Zugriff auf Globale Variable in client.c
extern char *player;                                                            //player. Zugriff auf Globale Variable in client.c

//Funktionen deklarieren
char *handle(char *request);

#endif
