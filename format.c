#include "format.h"

int pro = 1;                                                                                        //Prologvariable

char *version;                                                                  //TODO speicher freigeben nicht vergessen
char *gamekind;
char *gamename;

char *player;
int playernumber;
int totalplayers;

/*
 *Funktion format formatiert die Nachrichten vom Server.
 *@param *input Nachricht vom Servers
 *@return out gibt formatierte Ausgabe zurrück
 */
char *format(char *input){

  char *out = malloc(256*sizeof(char));

    if(pro==1 && match(input, "MNM Gameserver .+accepting connections")){                           // Stimmen Prologvariable und Text überein
      strcpy(out, "The MNM Gameserver version ");                                                 // Ausgabe The MNM Gameserver <Versionsnummer> accepted the connection
      version = substring(input, 16,strlen(input)-22);
      strcat(out, version);                                                                       // Versionsnummer
      strcat(out, " accepted the connection.");
      pro++;                                                                                       // Prolog erhöhen
    }else if(pro==2 && match(input, "Client version accepted - please send Game-ID to join")){     // Stimmen Prologvariable und Text überein
      strcpy(out,"The client version was accepted by the Gameserver. Please send a valid Game-ID to join the game.");    // Ausgabe The client version was accepted. Now please send Game-ID to join the game
      pro++;                                                                                       // Prolog erhöhen
    }else if(pro==3 && match(input, "PLAYING .+")){                                                // Stimmen Prologvariable und Text überein
      strcpy(out, "Preparing to play the game ");                                                  // Ausgabe We are playing the game <Gamekind>
      gamekind = substring(input, 8,strlen(input));
      strcat(out, gamekind);                                                                       // Gamekind
      pro++;                                                                                       // Prolog erhöhen
    }else if(pro==4 && match(input, ".+")){                                                        // Stimmen Prologvariable und Text überein
      strcpy(out, "The games name is ");                                                           // Ausgabe The game's name is <Gamename>
      gamename = input;
      strcat(out, input);                                                                          // Gamename
      pro++;                                                                                       // Prolog erhöhen
    }else if(pro==5 && match(input, "YOU .+ .+")){                                                 // Stimmen Prologvariable und Text überein
      player = substring(input, 6,strlen(input));
      strcpy(out, player);                                                                         // Player name
      strcat(out, " you are player number ");                                                      // Ausgabe <Player name> you are player number <Player number>
      char *splayernumber = substring(input, 4, 5);
      playernumber = atoi(splayernumber);
      strcat(out, splayernumber);                                                                   // Player number
      pro++;                                                                                       // Prolog erhöhen
      free(splayernumber);
    }else if(pro==6 && match(input, "TOTAL .+")){                                                  // Stimmen Prologvariable und Text überein
      char *stotalplayers = substring(input,6,strlen(input));
      totalplayers = atoi(stotalplayers);
      strcpy(out, stotalplayers);                                                                  // Ausgabe <Total> player will take part in this game
      if(totalplayers <2){
        strcat(out, " player will take part in this game.");
      }else{
        strcat(out, " players will take part in this game.");
      }
      pro++;                                                                                       // Prolog erhöhen
      free(stotalplayers);
    }else if(match(input, "ENDPLAYERS")){                                                          // Input ENDPLAYERS
      strcpy(out, "Starting the game...");                                                                // Ausgabe ENDPLAYERS - The prolog is finished!
    }else if(pro>=7 && match(input, ".+ .+ .+")){                                                  // Stimmen Prologvariable und Text überein
      char *rplayer = substring(input, 2,strlen(input)-2);
      strcpy(out, rplayer);                                                                        // Ausgabe <Player> (<Playernumber>) is ready/ not ready
      strcat(out," (");
      char *rplayernumb = substring(input, 0,1);
      strcat(out, rplayernumb);                                                                    // (Playernumber)
      strcat(out,") is ");
      char *playerstatus = substring(input, strlen(input)-1, strlen(input));
      if(atoi(playerstatus)==1){                                                                  //Spieler bereit -> letzte Zahl = 1
        strcat(out, "ready");
      }
      else if(atoi(playerstatus)==0){                                                             //Spieler nicht bereit -> letzte Zahl = 0
        strcat(out, "not ready");
      }
      pro++;                                                                                       // Prolog erhöhen
      free(rplayer);
      free(rplayernumb);
      free(playerstatus);
    }else{                                                                                         // Unbekannte Eingabe
      strcpy(out,"Unknown request");
    }

    if(input!=NULL){
      strcat(out,"\n");                                                                           // Anfügen Zeilenumbruch
    }
    return out;

    free(out);
}
