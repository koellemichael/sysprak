#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "performConnection.h"
#include "responseHandler.h"

#define BUFFERLENGTH 256

void performConnection(int *fd){

  char *buffer = malloc(BUFFERLENGTH*sizeof(char));                        //Speicher für Puffervariable allokalisieren

    do{
      fgets(buffer,BUFFERLENGTH,stdin);                                    //TODO entfernen und ersetzen durch
      buffer[strcspn(buffer, "\n")] = 0;                                   //#include <sys/types.h>
                                                                           //#include <sys/socket.h>
                                                                           //ssize_t recv(int sockfd, void *buf, size_t len, int flags);

      if(rescmp(buffer,"+")){                                              //Wenn Serverantwort positiv ausfällt
        if(strlen(buffer)>2){                                              //Leere Antworten vom Server ignorieren
          printf("server: %s\n",(buffer+2));                               //Gibt Antwort des Servers aus
          char *response = handle(buffer);                                 //Holt sucht die passende Antwort auf die Serveranfrage
          if (response!=NULL){                                             //Wenn es eine Antwort gibt
              //send(response);                                            //TODO Antwort an den Server senden ssize_t send(int sockfd, const void *buf, size_t len, int flags);
              printf("client: %s\n",response);                             //Gesendete Antwort ausgeben
          }
        }
      }else if(rescmp(buffer,"-")){                                        //Wenn Serverantwort negativ ausfällt
        printf("server: Error! %s\nDisconnecting server...\n",buffer+2);   //Gebe Fehler aus
        exit(EXIT_FAILURE);                                                //Beende Programm
      }

    }while(!rescmp(buffer,"+ ENDPLAYERS"));                                //Nehme solange Antworten vom Server entgegen bis Server "+ ENDPLAYERS" antwortet

    if(buffer!=NULL){                                                      //Wenn der Speicher von buffer noch nicht freigegeben wurde
      free(buffer);                                                        //Speicher von buffer freigeben
    }

}
