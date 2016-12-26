#include "performConnection.h"

/**
 *Die Funktion performConnection führt die Prologphase zwischen
 *Gameserver und Client aus.
 *Liest Anfragen vom Server ein und beantwortet diese.
 *
 *@param sock Filedeskriptor des Sockets
 */
void performConnection(int sock){
  char *buffer = malloc(BUFFERLENGTH*sizeof(char));                             //Speicher für Puffervariable allokalisieren
  char **requests = malloc(BUFFERLENGTH*sizeof(char*));                         //Speicher für das Array der einzelnen Serveranfragen allokalisieren
  int end = 1;                                                                  //Variable in der gespeichert wird ob der Server das Ende des Prologs (+ ENDPLAYERS) gesendet hat
    do{
      memset(buffer,0, BUFFERLENGTH);                                           //Puffer leeren
      recv(sock, buffer, BUFFERLENGTH-1, 0);                                    //Warte auf Anfrage des Servers
      strtoken(buffer, "\n",requests);                                          //Wenn der Server mehrere Anfragen "Unknown requestaufeinmal schickt, werden sie hier in ein String Array eingelesen
      int x = 0;                                                                //Laufvariable da mehrere Anfragen aufeinmal geschickt werden können
     do{
       end = !match(requests[x]+2,"QUIT");                                      //Wurde +ENDPLAYERS gesendet?
        if(buffer[0]=='+'){                                                     //Wenn Serveranfrage positiv ausfällt
          if(strlen(requests[x])>2){                                            //Leere Anfrage vom Server ignorieren               //TODO Fehlerbehandlung
            printf("server RAW: %s\n",requests[x]);
            char *response = handle(requests[x]+2);                             //Sucht die passende Anfrage auf die Serveranfrage
            if (response!=NULL){                                                //Wenn es eine Anfrage gibt
                send(sock,response,strlen(response),0);                         //Sendet dem Server die Antwort des Clients
                printf("client: %s",response);                                  //Gesendete Antwort ausgeben
            }
            if(response!=NULL){                                                 //Speicher von response freigeben da, in handle Speicher allokalisiert wurde
              free(response);                                                   //Response referenziert auf diesen Speicher
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
