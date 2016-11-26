#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "performConnection.h"
#include "responseHandler.h"

#define BUFFERLENGTH 256                                                        //Puffergröße

/**
 *Die Funktion strtoken zerteilt einen String in mehrere durch den Seperator
 *getrennte Teilstrings und speichert diese ein einem array.
 *
 *@param str Der String der in Teilstrings zerlegt werden soll
 *@param seperator Der Seperator nach dem zerlegt werden soll
 *@param token Das String Array in dem die Teilstring
 *@return Anzahl der Teilstrings als int
 */
int strtoken(char *str, char *separator, char *token[]){
  int i = 0;                                                                    //Zählvariable/Laufvariable für die Anzahl
  token[0] = strtok(str, separator);                                            //strtok initialisieren und ersten Eintrag in das String Array
  while (token[i]){                                                             //Solange der vorherige Eintrag nicht NULL ist
    i++;                                                                        //Zählvariable hochzählen
    token[i] = strtok(NULL, separator);                                         //Da strtok schon initialisiert wurde, kann man mit NULL den restlichen Teilstring zerlegen, speichert ihn im Array
  }
  return i;                                                                     //Gibt Anzahl zurück
}

/**
 *Die Funktion performConnection führt die Prologphase zwischen
 *Gameserver und Client aus.
 *Liest Anfragen vom Server ein und beantwortet diese.
 *
 *@param sock Filedeskriptor des Sockets
 */
void performConnection(int *sock){

  char *buffer = malloc(BUFFERLENGTH*sizeof(char));                             //Speicher für Puffervariable allokalisieren
  char **requests = malloc(BUFFERLENGTH*sizeof(char*));                         //Speicher für das Array der einzelnen Serveranfragen allokalisieren
  int end = 1;                                                                  //Variable in der gespeichert wird ob der Server das Ende des Prologs (+ ENDPLAYERS) gesendet hat
    do{
      recv(*sock, buffer, BUFFERLENGTH-1, 0);                                   //Warte auf Anfrage des Servers
      strtoken(buffer, "\n",requests);                                          //Wenn der Server mehrere Anfragen aufeinmal schickt, werden sie hier in ein String Array eingelesen
      int x = 0;                                                                //Laufvariable da mehrere Anfragen aufeinmal geschickt werden können
     do{
       end = !reqcmp(requests[x],"+ ENDPLAYERS");                               //Wurde +ENDPLAYERS gesendet?
        if(buffer[0]=='+'){                                                     //Wenn Serveranfrage positiv ausfällt
          if(strlen(requests[x])>2){                                            //Leere Anfrage vom Server ignorieren
            printf("server: %s\n",(requests[x]+2));                             //Gibt Anfrage des Servers aus
            char *response = handle(requests[x]+2);                             //Sucht die passende Anfrage auf die Serveranfrage
            if (response!=NULL){                                                //Wenn es eine Anfrage gibt
              if(strcmp(response, "Unknown response\n")){                       //Wenn diese vom responseHandler erkannt wurde
                send(*sock,response,strlen(response),0);                        //Sendet dem Server die Antwort des Clients
              }
                printf("client: %s",response);                                  //Gesendete Antwort ausgeben
            }
          }
        }else if(buffer[0]=='-'){                                               //Wenn Serveranfrage negativ ausfällt
          printf("server: Error! %s\nDisconnecting server...\n",buffer+2);      //Gebe Fehler aus
          exit(EXIT_FAILURE);                                                   //Beende Programm
        }
        x++;
      }while(requests[x]!=NULL && end);                                         //Solange es noch Anfragen aus dem requests Array gibt und + ENDPLAYERS noch nicht gesendet wurde
    }while(end);                                                                //Nehme solange Antworten vom Server entgegen bis Server "+ ENDPLAYERS" antwortet

    if(buffer!=NULL){                                                           //Wenn der Speicher von buffer noch nicht freigegeben wurde
      free(buffer);                                                             //Speicher von buffer freigeben
    }
    if(requests!=NULL){                                                         //Wenn der Speicher von requests noch nicht freigegeben wurde
      free(requests);                                                           //Speicher von buffer freigeben
    }

}
