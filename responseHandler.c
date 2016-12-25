
#include "responseHandler.h"

int prolog = 1;                                                                 //!Variable für den Fortschritt der Prologphase.
int command = 0;                                                                //Flag, das nach den verschiedenen Befehlen gesetzt wird

/**
 *Die Funktion handle verarbeitet die Anfrage des Servers zur passenden
 *Antwort des Clients, diese wird zurückgegeben.
 *
 *@param request char* auf die Anfrage/Antwort des Servers
 *@return Antwort des Clients, NULL falls keine passende Antwort
 */
char *handle(char *request){
  char *response;                                                               //Antwortvariable initialisieren
  char *out;
  if((response = malloc(BUFFERLENGTH*sizeof(char)))==NULL){
    perror("Not enough Memory for variables");
  }
  if((out = malloc(BUFFERLENGTH*sizeof(char)))==NULL){
    perror("Not enough Memory for variables");
  }
  if(prolog==1 && match(request,                                                //Wenn Anfrage des Servers übereinstimmt und der Prologfortschritt passt
    "MNM Gameserver .+accepting connections")){
    strcpy(response,"VERSION ");                                                //Setze response auf die passende Antwort
    strcat(response, VERSION);                                                  //TODO
    //Format
    strcpy(out, "The MNM Gameserver version ");                                 // Ausgabe The MNM Gameserver <Versionsnummer> accepted the connection
    char *version = substring(request, 16,strlen(request)-22);
    strcat(out, version);                                                       // Versionsnummer
    strcat(out, " accepted the connection.");
    if(version != NULL){
      free(version);
    }
    prolog++;                                                                   //Prologfortschritt erhöhen, da ein Schritt des Prologs fertig gestellt wurde
  }else if(prolog==2 && match(request,                                          //Wenn Anfrage des Servers übereinstimmt und der Prologfortschritt passt
    "Client version accepted - please send Game-ID to join")){
    strcpy(response,"ID ");                                                     //Setze response auf die passende Antwort
    strcat(response,gameid);                                                    //Game-ID von client.c übernehmen
    //Format
    strcpy(out,"The clients version was accepted by the Gameserver. Please send a valid Game-ID to join the game.");
    prolog++;                                                                   //Prologfortschritt erhöhen, da ein Schritt des Prologs fertig gestellt wurde
  }else if(prolog==3 && match(request,                                          //Wenn Anfrage des Servers übereinstimmt und der Prologfortschritt passt
    "PLAYING .+")){
    if(response!=NULL){
      free(response);
    }
    response = NULL;                                                            //Setze response auf die passende Antwort
    //Format
    strcpy(out, "Preparing to play the game ");                                 //Ausgabe We are playing the game <Gamekind>
    char *gamekind = substring(request, 8,strlen(request));
    strcat(out, gamekind);                                                      //Gamekind
    if(gamekind != NULL){
      free(gamekind);
    }
    prolog++;                                                                   //Prologfortschritt erhöhen, da ein Schritt des Prologs fertig gestellt wurde
  }else if(prolog==4 && match(request,                                          //Wenn Anfrage des Servers übereinstimmt und der Prologfortschritt passt
    ".+")){
    strcpy(response,"PLAYER ");                                                 //Setze response auf die passende Antwort
    strcat(response, player);                                                   //Playernummer, falls vorhanden, anhängen
    //Format
    strcpy(out, "The games name is ");                                          //Ausgabe The game's name is <Gamename>
    strcpy(serverinfo->gamename, request);                                        //Spielname im struct speichern
    strcat(out, request);                                                         //Gamename
    prolog++;                                                                   //Prologfortschritt erhöhen, da ein Schritt des Prologs fertig gestellt wurde
  }else if(prolog==5 && match(request,                                          //Wenn Anfrage des Servers übereinstimmt und der Prologfortschritt passt
    "YOU .+ .+")){
    if(response!=NULL){
      free(response);
    }
    response = NULL;                                                            //Setze response auf die passende Antwort
    //Format
    char *player = substring(request, 6,strlen(request));
    strcpy(serverinfo->clientname, player);                                     //Spielername des Clients im struct speichern
    strcpy(out, player);                                                        //Player name
    strcat(out, " you are player number ");                                     //Ausgabe <Player name> you are player number <Player number>
    char *splayernumber = substring(request, 4, 5);
    serverinfo->clientplayernr = atoi(splayernumber);                           //Spielernr des Clients im struct speichern
    strcat(out, splayernumber);                                                 //Player number
    free(player);
    free(splayernumber);
    prolog++;                                                                   //Prologfortschritt erhöhen, da ein Schritt des Prologs fertig gestellt wurde
  }else if(prolog==6 && match(request,                                          //Wenn Anfrage des Servers übereinstimmt und der Prologfortschritt passt
    "TOTAL .+")){
    if(response!=NULL){
     free(response);
    }
    response = NULL;                                                            //Setze response auf die passende Antwort
    //Format
    char *stotalplayers = substring(request,6,strlen(request));
    serverinfo->totalplayers = atoi(stotalplayers);                             //Anzahl der Spieler im struct Speichern
    strcpy(out, stotalplayers);                                                 //Ausgabe <Total> player will take part in this game
    if(atoi(stotalplayers) <2){
      strcat(out, " player will take part in this game.");
    }else{
      strcat(out, " players will take part in this game.");
    }

    for(int i = 0; i<serverinfo->totalplayers-1; i++){
      shmid_player[i] = createSHM(sizeof(struct player));                       //Shared Memory Segment für jeden Spieler erstellen
      serverinfo->otherplayers[i] = attachSHM(shmid_player[i]);                 //Shared Memory Segment anbinden
    }

    free(stotalplayers);
    prolog++;                                                                   //Prologfortschritt erhöhen, da ein Schritt des Prologs fertig gestellt wurde
  }else if(match(request,                                                       //Wenn Anfrage des Servers übereinstimmt und der Prologfortschritt passt
    "ENDPLAYERS")){
    if(response!=NULL){
     free(response);
    }
    response = NULL;                                                            //Setze response auf die passende Antwort

    //Format
    strcpy(out, "Starting the game...");                                        //Ausgabe ENDPLAYERS - The prolog is finished!
  }else if(prolog>=7 && match(request,                                          //Wenn Anfrage des Servers übereinstimmt und der Prologfortschritt passt
      ".+ .+ .+")){
    if(response!=NULL){
      free(response);
    }
    response = NULL;                                                            //Setze response auf die passende Antwort
    //Format
    char *rplayer = substring(request, 2,strlen(request)-2);                        //Ausgabe <Player> (<Playernumber>) is ready/ not ready
    strcpy(serverinfo->otherplayers[prolog-7]->playername, rplayer);
    strcpy(out, rplayer);
    strcat(out," (");
    char *rplayernumb = substring(request, 0,1);
    serverinfo->otherplayers[prolog-7]->playernr = atoi(rplayernumb);
    strcat(out, rplayernumb);                                                   //(Playernumber)
    strcat(out,") is ");
    char *playerstatus = substring(request, strlen(request)-1, strlen(request));
    serverinfo->otherplayers[prolog-7]->ready = atoi(playerstatus);
    if(atoi(playerstatus)==1){                                                  //Spieler bereit -> letzte Zahl = 1
      strcat(out, "ready");
    }else if(atoi(playerstatus)==0){                                            //Spieler nicht bereit -> letzte Zahl = 0
      strcat(out, "not ready");
    }

    free(rplayer);
    free(rplayernumb);
    free(playerstatus);
    prolog++;                                                                   //Prologfortschritt erhöhen, da ein Schritt des Prologs fertig gestellt wurde
  }else if(match(request,"WAIT")){                                              //Wenn Anfrage des Servers übereinstimmt
    strcpy(response,"OKWAIT");
    strcpy(out, "Wait");
  }else if(match(request,"MOVE .+")){                                           //Wenn Anfrage des Servers übereinstimmt
    command = 1;
    return response;
  }else if(match(request,"OKTHINK")){ //TODO                                         
    strcpy(response,"PLAY");
    strcpy(out,"Make a move");
  }else if(match(request,"ENDPIECESLIST") && command ==1){
    strcpy(response,"THINKING\n");
    strcpy(out, "Start with turn calculation");
  }else{                                                                        //Ansonsten unbekannte Anfrage des Servers
    if(response!=NULL){
      free(response);
    }
    response = NULL;                                                            //Setze Antwort auf "NULL"
    if(out!=NULL){
      free(out);
    }
    out = NULL;
  }

  if(response!=NULL){                                                           //Wenn es eine passende Antwort gibt
    strcat(response,"\n");                                                      //hänge hinten eine new Line and damit der Antwortbefehlt vollständig ist
  }
  if(out != NULL){
    printf("server: %s\n",out);
    free(out);
  }

  return response;                                                              //Gibt die Antwort des Clients zurück
}
