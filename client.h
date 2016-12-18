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
#include "config.h"
#include "parameter.h"
#include "sharedMemory.h"

//Konstanten definieren
#define GAMEKINDNAME "Bashni"                                                   //Konstante GAMEKINDNAME definieren
#define PORTNUMBER 1357                                                         //Konstante PORTNUMBER definieren
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"                               //Konstante HOSTNAME definieren

//Globale Variablen deklarieren
char *gameid;                                                                   //Globale Variable für die Game-ID deklarieren
char *player;                                                                   //Gloabel Variable für die PlayerNr deklarieren
char *paramNameHost = "hostName";                                               //Parametername des Hostnamens, nachdem wir filtern
char *paramNamePort = "portNumber";                                             //Parametername der Portnummer, nach der wir filtern
char *paramNameGame = "gameKindName";                                           //Parametername des Spiels, nach dem wir filtern
char *value = ""; 
configparams cp;                                                                //Eine Instanz vom Struct configparams namens "cp"
char *confile;                                                                  //Speichert den Pfad der Konfigurationsfile, die geöffnet werden soll
int pflag;                                                                      //Setzt ein Flag, wenn Player angegeben wurde
int fflag;                                                                      //Setzt ein Flag, wenn Konfigdatei angegeben wurde
int c;                                                                          //RetValue von getopt. Entweder Argument oder -1, wenn kein Argument vorhanden
int *shmid_player;                                                              //Gobales Int Array für die ShmIDs der Player Segmente
struct serverinfo *serverinfo;                                                  //Gobales struct für die Serverinfos


#endif
