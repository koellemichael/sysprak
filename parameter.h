#ifndef para                                                                    //Wenn das Makro noch nicht existiert
#define para                                                                    //Definiere das Makro

//Bibliotheken einbinden
#include <stdio.h>

//Konstanten definieren
#define GAMEKINDNAME "Bashni"                                                   //Konstante GAMEKINDNAME definieren
#define PORTNUMBER 1357                                                         //Konstante PORTNUMBER definieren
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"                               //Konstante HOSTNAME definieren
#define BUFFERLENGTH 2048	                                                      //Konstante für die std Puffergröße definieren
#define BUFFERLENGTH_FIELD 2048
#define BUFFERLENGTH_SMALL 128
#define BUFFERLENGTH_PORT 6
#define BUFFERLENGTH_MOVE 25                                                    //Konstante für die Puffergröße eines Spielzugs definieren
#define BUFFERLENGTH_MAXPIECES 12                                               //Konstante für die maximale Anzahl an Steinen eines Spielers
#define VERSION "2.3"                                                           //TODO provisorische Konstante
#define ROWS 8                                                                  //Konstante für die Reihen des Spielfelds
#define COLUMNS 8                                                               //Konstante für die Spalten des Spielfelds

//Gewichtungen
#define JUMP 500
#define MOVE 5

//Funktionen definieren
void printLogo(void);

#endif
