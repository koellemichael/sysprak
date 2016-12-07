#ifndef responseHdl                                                             //Wenn das Makro noch nicht existiert
#define responseHdl                                                             //Definiere das Makro

//Bibliotheken einbinden
#include <stdlib.h>
#include <stdio.h>
#include <String.h>

//Module einbinden
#include "processRequest.h"

//Konstanten definieren
#define VERSION "2.3"                                                           //TODO provisorische Konstante

//Externe Variablen einbinden
extern char *gameid;                                                            //gameid. Zugriff auf Globale Variable in client.c
extern char *player;                                                            //player. Zugriff auf Globale Variable in client.c

//Funktionen deklarieren
char *handle(char *request);

#endif
