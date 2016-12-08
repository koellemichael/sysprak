#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "processRequest.h"
#include "format.h"

int pro = 1;                                                                                        //Prologvariable

/*
 *Funktion format formatiert die Nahcrichten vom Server
 *@param *input Nachricht vom Servers
 *@return out gibt formatierte Ausgabe zurrück
 */

char *format(char *input){

  char *out = malloc(256*sizeof(char));

    if(pro==1 && match(input, "MNM Gameserver .+accepting connections")){                          // Stimmen Prologvariable und Text überein
      strcpy(out, "The MNM Gameserver ");                                                          // Ausgabe The MNM Gameserver <Versionsnummer> accepted the connection
      strcat(out, substring(input, 14,strlen(input)-21));                                          // Versionsnummer
      strcat(out, "accepted the connection.");
      pro++;                                                                                       // Prolog erhöhen


    }else if(pro==2 && match(input, "Client version accepted - please send Game-ID to join")){     // Stimmen Prologvariable und Text überein
      strcpy(out,"The client version was accepted. Now please send Game-ID to join the game.");    // Ausgabe The client version was accepted. Now please send Game-ID to join the game
      pro++;                                                                                       // Prolog erhöhen



    }else if(pro==3 && match(input, "PLAYING .+")){                                                // Stimmen Prologvariable und Text überein
      strcpy(out, "We are playing the game ");                                                     // Ausgabe We are playing the game <Gamekind>
      strcat(out, substring(input, 8,strlen(input)));                                              // Gamekind
      pro++;                                                                                       // Prolog erhöhen



    }else if(pro==4 && match(input, ".+")){                                                        // Stimmen Prologvariable und Text überein
      strcpy(out, "The games name is ");                                                          // Ausgabe The game's name is <Gamename>
      strcat(out, input);                                                                          // Gamename
      pro++;                                                                                       // Prolog erhöhen


    }else if(pro==5 && match(input, "YOU .+ .+")){                                                 // Stimmen Prologvariable und Text überein
      strcpy(out, substring(input, 6,strlen(input)));                                              // Player name
      strcat(out, " you are player number ");                                                      // Ausgabe <Player name> you are player number <Player number>
      strcat(out, substring(input, 4, 5));                                                         // Player number
      pro++;                                                                                       // Prolog erhöhen


    }else if(pro==6 && match(input, "TOTAL .+")){                                                  // Stimmen Prologvariable und Text überein
      strcpy(out, substring(input,6,strlen(input)));                                               // Ausgabe <Total> player will take part in this game
      strcat(out, " player will take part in this game.");                                         // Total
      pro++;                                                                                       // Prolog erhöhen


    }else if(match(input, "ENDPLAYERS")){                                                          // Input ENDPLAYERS
      strcpy(out, "ENDPLAYERS - The prolog is finished!");                                         // Ausgabe ENDPLAYERS - The prolog is finished!


    }else if(pro>=7 && match(input, ".+ .+ .+")){                                                  // Stimmen Prologvariable und Text überein
      strcpy(out, substring(input, 2,strlen(input)-2));                                            // Ausgabe <Player> (<Playernumber>) is ready/ not ready
      strcat(out," (");
      strcat(out, substring(input, 0,1));                                                          // (Playernumber)
      strcat(out,") is ");


      if(atoi(substring(input, strlen(input)-2, strlen(input)-1))==1){                              //Spieler bereit -> letzte Zahl = 1
        strcat(out, "ready");
      }
      else if(atoi(substring(input, strlen(input)-2, strlen(input)-1))==0){                         //Spieler nicht bereit -> letzte Zahl = 0
        strcat(out, "not ready");
      }

      pro++;                                                                                       // Prolog erhöhen


    }else{                                                                                         // Unbekannte Eingabe
      strcpy(out,"Unknown request");
    }



    if(input!=NULL){
      strcat(out,"\n");                                                                           // Anfügen Zeilenumbruch
    }
    return out;
}
