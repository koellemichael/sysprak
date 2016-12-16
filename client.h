#ifndef client                                                                  //Wenn das Makro noch nicht existiert
#define client                                                                  //Definiere das Makro

//Bibliotheken einbinden
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "signal.h"

//Module einbinden
#include "connectServer.h"
#include "parameter.h"

//Globale Variablen deklarieren
char *gameid;                                                                   //Globale Variable für die Game-ID deklarieren
char *player;                                                                   //Gloabel Variable für die PlayerNr deklarieren
int *playerid;


struct info *info;

#endif
