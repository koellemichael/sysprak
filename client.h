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
#include "sharedMemory.h"

//Globale Variablen deklarieren
char *gameid;                                                                   //Globale Variable für die Game-ID deklarieren
char *player;                                                                   //Gloabel Variable für die PlayerNr deklarieren
int *shmid_player;                                                              //Gobales Int Array für die ShmIDs der Player Segmente
struct serverinfo *serverinfo;                                                  //Gobales struct für die Serverinfos

#endif
