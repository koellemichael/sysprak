#include "format.h"

int pro = 1;                                                                    //Prologvariable

/*
 *Funktion format formatiert die Nachrichten vom Server.
 *@param *input Nachricht vom Servers
 *@return out gibt formatierte Ausgabe zurrück
 */
char *format(char *input){

  char *out = malloc(256*sizeof(char));

    if(pro==1 && match(input, "MNM Gameserver .+accepting connections")){       // Stimmen Prologvariable und Text überein
      strcpy(out, "The MNM Gameserver version ");                               // Ausgabe The MNM Gameserver <Versionsnummer> accepted the connection
      char *version = substring(input, 16,strlen(input)-22);
      strcat(out, version);                                                     // Versionsnummer
      strcat(out, " accepted the connection.");
      if(version != NULL){
        free(version);
      }
      pro++;                                                                    // Prolog erhöhen
    }else if(pro==2 &&
      match(input, "Client version accepted - please send Game-ID to join")){   // Stimmen Prologvariable und Text überein
      strcpy(out,"The clients version was accepted by the Gameserver. Please send a valid Game-ID to join the game.");                          // Ausgabe The client version was accepted. Now please send Game-ID to join the game
      pro++;                                                                    // Prolog erhöhen
    }else if(pro==3 && match(input, "PLAYING .+")){                             // Stimmen Prologvariable und Text überein
      strcpy(out, "Preparing to play the game ");                               // Ausgabe We are playing the game <Gamekind>
      char *gamekind = substring(input, 8,strlen(input));
      strcat(out, gamekind);                                                    // Gamekind
      if(gamekind != NULL){
        free(gamekind);
      }
      pro++;                                                                    // Prolog erhöhen
    }else if(pro==4 && match(input, ".+")){                                     // Stimmen Prologvariable und Text überein
      strcpy(out, "The games name is ");                                        // Ausgabe The game's name is <Gamename>
      strcpy(serverinfo->gamename, input);                                      // Spielname im struct speichern
      strcat(out, input);                                                       // Gamename
      pro++;                                                                    // Prolog erhöhen
    }else if(pro==5 && match(input, "YOU .+ .+")){                              // Stimmen Prologvariable und Text überein
      char *player = substring(input, 6,strlen(input));
      strcpy(serverinfo->clientname, player);                                   // Spielername des Clients im struct speichern
      strcpy(out, player);                                                      // Player name
      strcat(out, " you are player number ");                                   // Ausgabe <Player name> you are player number <Player number>
      char *splayernumber = substring(input, 4, 5);
      serverinfo->clientplayernr = atoi(splayernumber);                         // Spielernr des Clients im struct speichern
      strcat(out, splayernumber);                                               // Player number
      pro++;                                                                    // Prolog erhöhen
      free(player);
      free(splayernumber);
    }else if(pro==6 && match(input, "TOTAL .+")){                               // Stimmen Prologvariable und Text überein
      char *stotalplayers = substring(input,6,strlen(input));
      serverinfo->totalplayers = atoi(stotalplayers);                           // Anzahl der Spieler im struct Speichern
      strcpy(out, stotalplayers);                                               // Ausgabe <Total> player will take part in this game
      if(atoi(stotalplayers) <2){
        strcat(out, " player will take part in this game.");
      }else{
        strcat(out, " players will take part in this game.");
      }

      for(int i = 0; i<serverinfo->totalplayers-1; i++){
        shmid_player[i] = createSHM(sizeof(struct player));                     //Shared Memory Segment für jeden Spieler erstellen
        serverinfo->otherplayers[i] = attachSHM(shmid_player[i]);               //Shared Memory Segment anbinden
      }

      pro++;                                                                    // Prolog erhöhen
      free(stotalplayers);
    }else if(match(input, "ENDPLAYERS")){                                       // Input ENDPLAYERS
      strcpy(out, "Starting the game...");                                      // Ausgabe ENDPLAYERS - The prolog is finished!
    }else if(pro>=7 && match(input, ".+ .+ .+")){                               // Stimmen Prologvariable und Text überein
      char *rplayer = substring(input, 2,strlen(input)-2);                      // Ausgabe <Player> (<Playernumber>) is ready/ not ready
      strcpy(serverinfo->otherplayers[pro-7]->playername, rplayer);
      strcpy(out, rplayer);
      strcat(out," (");
      char *rplayernumb = substring(input, 0,1);
      serverinfo->otherplayers[pro-7]->playernr = atoi(rplayernumb);
      strcat(out, rplayernumb);                                                 // (Playernumber)
      strcat(out,") is ");
      char *playerstatus = substring(input, strlen(input)-1, strlen(input));
      serverinfo->otherplayers[pro-7]->ready = atoi(playerstatus);
      if(atoi(playerstatus)==1){                                                //Spieler bereit -> letzte Zahl = 1
        strcat(out, "ready");
      }else if(atoi(playerstatus)==0){                                          //Spieler nicht bereit -> letzte Zahl = 0
        strcat(out, "not ready");
      }
      pro++;                                                                    // Prolog erhöhen
      free(rplayer);
      free(rplayernumb);
      free(playerstatus);
    }else{                                                                      // Unbekannte Eingabe
      strcpy(out,"Unknown request");
    }

    if(input!=NULL){
      strcat(out,"\n");                                                         //Anfügen Zeilenumbruch
    }
    return out;

    free(out);
}
