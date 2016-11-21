#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "performConnection.h"

#define BUFFERLENGTH 256

void performConnection(int *fd){

  char *buffer = malloc(BUFFERLENGTH*sizeof(char));                       //Speicher für Puffervariable allokalisieren

  do{
    fgets(buffer,BUFFERLENGTH,stdin);                                     //TODO entfernen und durch
    buffer[strcspn(buffer, "\n")] = 0;                                    //#include <sys/types.h>
                                                                          //#include <sys/socket.h>
                                                                          //ssize_t recv(int sockfd, void *buf, size_t len, int flags);

    if(strncmp(buffer, "+", 1) == 0){                                     //Wenn Serverantwort positiv ausfällt
      if(strlen(buffer+2)!=0){                                              //Bei leerer Antwort von Server die Antwort nicht ausgeben
        printf("server: %s\n",(buffer+2));                                  //Gibt Antwort des Servers aus
      }
      if(strncmp((buffer+2), "MNM", 3) == 0){                               //Wenn Serveranwtwort: "+ MNM Gameserver <<Gameserver Version>> accepting connections"
                                                                            //Schicke Server: VERSION <<Client Version>> TODO ssize_t send(int sockfd, const void *buf, size_t len, int flags);
        printf("client: VERSION <<Client Version>>\n");
      }
      if(strncmp((buffer+2), "Client", 6) == 0){                            //Wenn Serveranwtwort: "+ Client version accepted - please send Game-ID to join"
                                                                            //Schicke Server: ID <<Game-ID>> TODO ssize_t send(int sockfd, const void *buf, size_t len, int flags);
        printf("client: ID <<Game-ID>>\n");
      }
      if(strncmp((buffer+2), "Bashni", 6) == 0){                            //Wenn Serveranwtwort: "+ <<Game-Name>>"
                                                                            //Schicke Server: PLAYER [[Gewunschte Spielernummer]] TODO ssize_t send(int sockfd, const void *buf, size_t len, int flags);
        printf("client: PLAYER [[Gewunschte Spielernummer]]\n");
      }
    }else if(strncmp(buffer, "-", 1) == 0){                               //Wenn Serverantwort negativ ausfällt
      printf("Error: %s\nDisconnecting server...\n",buffer+2);              //Gebe Fehler aus
      exit(EXIT_FAILURE);                                                   //Beende Programm
    }
  }while(strcmp(buffer, "+ ENDPLAYERS")!=0);                              //Nehme solange Antworten vom Server entgegen bis Server "+ ENDPLAYERS" antwortet


  if(buffer!=NULL){                                                       //Wenn der Speicher von buffer noch nicht freigegeben wurde
    free(buffer);                                                           //Speicher von buffer freigeben
  }

}
