#ifndef responseHdl                                                             //Wenn das Makro noch nicht existiert
#define responseHdl                                                             //Definiere das Makro

//Bibliotheken einbinden
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

//Module einbinden
#include "processRequest.h"
#include "parameter.h"
#include "sharedMemory.h"
#include "field.h"

//Externe Variablen
extern int fd[2];
extern struct serverinfo *serverinfo;
extern int* shmid_player;
extern char* gameid;                                                            //gameid. Zugriff auf Globale Variable in client.c
extern char* player;                                                            //player. Zugriff auf Globale Variable in client.c
extern int rdy;
int prolog;                                                                     //!Variable für den Fortschritt der Prologphase
int command;                                                                    //Flag für den Befehl MOVE
int won0;
int won1;

//Funktionen deklarieren
char *handle(char *request);

#endif
