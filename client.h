#ifndef client                                                                  //Wenn das Makro noch nicht existiert
#define client                                                                  //Definiere das Makro


//Bibliotheken einbinden
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Module einbinden
#include "connectServer.h"
#include "config.h"

//Konstanten definieren
#define GAMEKINDNAME "Bashni"                                                   //Konstante GAMEKINDNAME definieren
#define PORTNUMBER 1357                                                         //Konstante PORTNUMBER definieren
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"                               //Konstante HOSTNAME definieren

//Globale Variablen deklarieren
char *gameid;                                                                   //Globale Variable für die Game-ID deklarieren
char *player;                                                                   //Gloabel Variable für die PlayerNr deklarieren

#endif
