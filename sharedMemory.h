#ifndef sharedMem                                                               //Wenn das Makro noch nicht existiert
#define sharedMem                                                               //Definiere das Makro

//Bibliotheken einbinden
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

//Konstanten
#define PERMISSION 0644                                                         //Schreib/Leserechte für User, Leserechte für Gruppen
#define KEY IPC_PRIVATE                                                         //Private Key zur Erstellung von Shared Memory Segmenten

//Stukturen deklarieren
struct serverinfo{                                                              //Struktur für das Speichern von Serverinformationen
  char gamename[BUFFERLENGTH];                                                  //Spielname
  int clientplayernr;                                                           //Spielernummer unseres Clients
  char clientname[BUFFERLENGTH];                                                //Spielername unseres Clients
  int totalplayers;                                                             //Anzahl der Spieler
  pid_t pid_connector;                                                          //Prozess-ID des Connector Prozesses
  pid_t pid_thinker;                                                            //Prozess-ID des Thinker Prozesses
  struct player *otherplayers[BUFFERLENGTH];                                    //Array mit Informationen zu den anderen Spielern
};

struct player{                                                                  //Struktur für das Speichern von Informationen über einen Spieler
  int playernr;                                                                 //Spielernummer
  char playername[BUFFERLENGTH];                                                //Spielername
  int ready;                                                                    //0 wenn NICHT am Zug/bereit, 1 wenn am Zug/bereit
};

//Funktionen deklarieren
int createSHM(size_t size);                                                     //Erstellt Shared Memory Segment
void *attachSHM(int shmid);                                                     //Stellt Bindung zu einem Shared Memory Segment her
void detachSHM(void *shm);                                                      //Löst Bindung zu einem Shared Memory Segment
void deleteSHM(int shmid);                                                      //Löscht ein Shared Memory Segment

#endif
