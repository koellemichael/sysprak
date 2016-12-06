#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "processRequest.h"
#include "format.h"

char *format (char *input){
  char *out = malloc(256*sizeof(char));
  char **arr = malloc(256*sizeof(char));
  int p=1;

  if(match(input, "MNM Gameserver .+accepting connections")){                                       // Übereinstimmung des Textes
    for(unsigned int i=14; i<strlen(input)-21; i++){                                                         // Einlesen der Versions-Variable
      strcat(arr[1], input[i]);                                                                                                              //warning: passing argument 1 of ‘strcat’ from incompatible pointer type [-Wincompatible-pointer-types]
    }
    strcat(out, arr);                                                                               // Output mit Versionsnummer
    strcat(out, " accepted the connection.");
  }


  else if(match(input, "Client version accepted - please send Game-ID to join")){                   // Übereinstimmung des Textes
      strcpy(out,"The client version was accepted. Now please send Game-ID to join the game.");     // Output
  }


  else if(match(input, "PLAYING .+")){                                                              // Übereinstimmung desTextes
    for(unsigned int i=8; i<strlen(input); i++){                                                             // Einlesen Spielart
      strcat(arr, input[i]);
    }
    strcpy(out, "We are playing the game ");                                                        // Output mit Spielart
    strcat(out, arr);
  }


  else if(match(input, ".+")){                                                                       // Übereinstimmung des Textes
    strcpy(out, "The game's name is ");                                                             // Output mit Spielname
    strcat(out, input);
  }


  else if(match(input, "YOU .+ .+")){                                                               // Übereinstimmung Text   
    for(unsigned int i=4; i<strlen(input); i++){                                                             // Einlesen Spielernummer und Name
      if(input[i]=" ")
        p++;
      strcat(arr[p], input[i]);                                                                     
    }
    strcpy(out, arr[2]);                                                                            // Output: <Spielername>, you are player number <Spielernummer>
    strcat(out, " is player number ");
    strcat(out, arr[1]);
    p=1;
  }


  else if(match(input, "TOTAL .+")){                                                                // Übereinstimmung Text
    for(unsigned int i=6; i<strlen(input); i++){                                                             // Einlesen Spieleranzahl
      strcat(arr, input[i]);
    }
    strcpy(out, "In this game ");                                                                   // Output mit Spieleranzahl
    strcat(out, arr);
    strcat(out, " player will take part.");
  }


  else if(match(input, ".+ .+ .+")){                                                                // Übereinstimmung Text
    for(unsigned int i=0; i<strlen(input); i++){                                                             // Einlesen <Spielernummer> <Spielername> <Bereit>
      if((int)input[i]==1){                                                                         // Zahl = 1
        strcat(arr[p], "ready");                                                                    // Spieler ready
      } 
      else if(input[i]==0){                                                                    // Zahl = 0
        strcat(arr[p], "not ready");                                                                // Spieler not ready
      }
      else 
        strcat(arr[p], input[i]);                                                                   // Einlesen Variable

      if(input[i]==" ")                                                                             // Erhöhen Laufvariable
        p++;                                                                     
    }
    strcpy(out, arr[2]);                                                                            // Spieler
    strcat(out, "(");                                                                              // (x)
    strcat(out, arr[1]);                                                                            
    strcat(out, ")");
    strcat(out, " is ");                                                                            // is 
    strcat(out, arr[3]);                                                                            // ready/ not ready

  }


  strcat(out, "\n");                                                                                // Zeilenumbruch an jede Zeile
  return out;                                                                                       // Rückgabe out

}
