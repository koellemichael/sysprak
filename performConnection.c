#include "performConnection.h"

/**
 *Die Funktion performConnection führt die Prologphase zwischen
 *Gameserver und Client aus.
 *Liest Anfragen vom Server ein und beantwortet diese.
 *
 *@param sock Filedeskriptor des Sockets
 */
void performConnection(int sock){
 char *buffer = malloc(BUFFERLENGTH*sizeof(char));                              //Speicher für Puffervariable allokalisieren
 nextmove = malloc(BUFFERLENGTH_MOVE*sizeof(char));
 char **requests = malloc(BUFFERLENGTH*sizeof(char*));                          //Speicher für das Array der einzelnen Serveranfragen allokalisieren 
    
  end = 1;
    do{
      //Beobachtung des Sockets und der Pipe mittels Select
      FD_ZERO(&readfds);                                                            //Macht das Set frei
      FD_SET(sock, &readfds);                                                       //Fügt dem Set den Socket hinzu (die Gameserververbindung)
      FD_SET(fd[0], &readfds);                                                      //Fügt dem Set die Pipe hinzu (Leseseite!)
        
        /*Wait up to 1 second*/
        tv.tv_sec = 20;          //Sekunden
        tv.tv_usec = 0;         //Mikrosekunden

        //Selectmethode, die aus dem Filedeskriptorset im festgelegten Zeitintervall überprüft, ob Daten anstehen
        retval = select(sizeof(readfds)*2, &readfds, NULL, NULL, &tv);
        if(retval == -1){
            perror("select()");
            exit(EXIT_FAILURE);
            printf("No Data available at the moment");
        }else if(retval){
         int pipeData = FD_ISSET(fd[0], &readfds);                              //ISSET testet, ob an DIESER PIPE etwas ansteht
         int socketData = FD_ISSET(sock, &readfds);
            
         memset(buffer,0, BUFFERLENGTH);                                        //Puffer leeren
         
        if(socketData!=0){                                                      //Wenn etwas ansteht, dann..
            if((read(sock, buffer, BUFFERLENGTH)) < 0){                            //Lese nächsten Spielzug aus der Pipe
             perror("Couldn't read from socket");                                 //Error, wenn aus der Pipe nicht gelesen werden konnte
            } 
            printf("Buffercontent: %s", buffer);
        }
         
         
         if(pipeData!=0){                                                       //Wenn etwas ansteht, dann..
         //Aus der Pipe lesen[0] 
             if((read(fd[0], buffer, BUFFERLENGTH_MOVE)) < 0){                    //Lese nächsten Spielzug aus der Pipe
              perror("Couldn't read from pipe");                                 //Error, wenn aus der Pipe nicht gelesen werden konnte
            } else strcpy(nextmove, buffer);                                    //Kopiere diesen Inhalt in nextmove
         }
         
        }
        //else{printf("No Data available right now\n");}
        
      strtoken(buffer, "\n",requests);                                          //Wenn der Server mehrere Anfragen "Unknown requestaufeinmal schickt, werden sie hier in ein String Array eingelesen
      int x = 0;                                                                //Laufvariable da mehrere Anfragen aufeinmal geschickt werden können
      
     do{         
       end = !match(requests[x]+2,"QUIT");                                      //Wurde +ENDPLAYERS gesendet?
        if(buffer[0]=='+'){                                                     //Wenn Serveranfrage positiv ausfällt
          if(strlen(requests[x])>2){                                            //Leere Anfrage vom Server ignorieren               //TODO Fehlerbehandlung
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
