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

struct info{
  char gamename[BUFFERLENGTH];
  int clientplayernr;
  char clientname[BUFFERLENGTH];
  int totalplayers;
  pid_t pid_connector;
  pid_t pid_thinker;
  struct player *players[BUFFERLENGTH];
};

struct player{
  int playernr;
  char playername[BUFFERLENGTH];
  int ready;
};

#endif
