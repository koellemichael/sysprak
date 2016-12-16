#ifndef para                                                                    //Wenn das Makro noch nicht existiert
#define para                                                                    //Definiere das Makro

//Konstanten definieren
#define GAMEKINDNAME "Bashni"                                                   //Konstante GAMEKINDNAME definieren
#define PORTNUMBER 1357                                                         //Konstante PORTNUMBER definieren
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"                               //Konstante HOSTNAME definieren
#define BUFFERLENGTH 256                                                        //Konstante für die Puffergröße definieren
#define VERSION "2.3"                                                           //TODO provisorische Konstante
#define PERMISSION 0644                                                         //Konstante für die Zugriffsrechte auf den Shared Memory
#define KEY IPC_PRIVATE                                                         //Konstante für den Key des Shared Memory

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


#endif
