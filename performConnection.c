#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "performConnection.h"
#include "responseHandler.h"

#define BUFFERLENGTH 256

int strtoken(char *str, char *separator, char *token[])
{
  int i = 0;

  token[0] = strtok(str, separator);

  while ( token[i] ) {
    i++;
    token[i] = strtok(NULL, separator);
  }
  return ( i );
}


void performConnection(int *fd){

  char *buffer = malloc(BUFFERLENGTH*sizeof(char));                        //Speicher für Puffervariable allokalisieren
  char **s_response = malloc(BUFFERLENGTH*sizeof(char*));
  int end;
    do{
      recv(*fd, buffer, BUFFERLENGTH-1, 0);
      strtoken(buffer, "\n",s_response);
      int x = 0;
     do{
       end = !rescmp(s_response[x],"+ ENDPLAYERS");
        if(rescmp(s_response[x],"+")){                                              //Wenn Serverantwort positiv ausfällt
          if(strlen(s_response[x])>2){                                              //Leere Antworten vom Server ignorieren
            printf("server: %s\n",(s_response[x]+2));                               //Gibt Antwort des Servers aus
            char *response = handle(s_response[x]+2);                                 //Holt sucht die passende Antwort auf die Serveranfrage
            if (response!=NULL){                                             //Wenn es eine Antwort gibt
                send(*fd,response,strlen(response),0);                      //TODO Antwort an den Server senden ssize_t send(int sockfd, const void *buf, size_t len, int flags);
                printf("client: %s",response);                             //Gesendete Antwort ausgeben
              }
            }
        }else if(rescmp(buffer,"-")){                                        //Wenn Serverantwort negativ ausfällt
          printf("server: Error! %s\nDisconnecting server...\n",buffer+2);   //Gebe Fehler aus
          exit(EXIT_FAILURE);                                                //Beende Programm
        }
        x++;
      }while(s_response[x]!=NULL && end);
    }while(end);                                //Nehme solange Antworten vom Server entgegen bis Server "+ ENDPLAYERS" antwortet

    if(buffer!=NULL){                                                      //Wenn der Speicher von buffer noch nicht freigegeben wurde
      free(buffer);                                                        //Speicher von buffer freigeben
    }

}
