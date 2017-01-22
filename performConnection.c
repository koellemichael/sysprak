#include "performConnection.h"

/**
 *Die Funktion performConnection führt die Prologphase zwischen
 *Gameserver und Client aus.
 *Liest Anfragen vom Server ein und beantwortet diese.
 *
 *@param sock Filedeskriptor des Sockets
 */
void performConnection(int sock){
  end = 1;
  rdy = 0;
  do{
    //Beobachtung des Sockets und der Pipe mittels Select
    FD_ZERO(&readfds);                                                          //Macht das Set frei
    FD_SET(sock, &readfds);                                                     //Fügt dem Set den Socket hinzu (die Gameserververbindung)
    FD_SET(fd[0], &readfds);                                                    //Fügt dem Set die Pipe hinzu (Leseseite!)

    tv.tv_sec = 1;                                                              //Sekunden
    tv.tv_usec = 0;                                                             //Mikrosekunden

    //Selectmethode, die aus dem Filedeskriptorset im festgelegten Zeitintervall überprüft, ob Daten anstehen
    retval = select(sizeof(readfds)*2, &readfds, NULL, NULL, &tv);
    if(retval == -1){
      perror("select()");
      exit(EXIT_FAILURE);
    }else if(retval){
      pipeData = FD_ISSET(fd[0], &readfds);                                 //ISSET testet, ob an DIESER PIPE etwas ansteht
      socketData = FD_ISSET(sock, &readfds);

      if(socketData!=0){                                                        //Wenn etwas ansteht, dann..
        char *buffer = malloc(BUFFERLENGTH*sizeof(char));                       //Speicher für Puffervariable allokalisieren
        memset(buffer,0, strlen(buffer));                                         //Puffer leeren
        if((read(sock, buffer, BUFFERLENGTH)) < 0){                             //Lese nächsten Spielzug aus der Pipe
          perror("Couldn't read from socket");                                  //Error, wenn aus der Pipe nicht gelesen werden konnte
        }
        processAndSendResponse(buffer, sock);
      }

      //TODO nachdem einmal was in die Pipe geschrieben wurde, gibt FD_ISSET permanent !=0 zurück was dazu führt dass der nächste zug "" ist
      if(pipeData!=0&&rdy){                                                     //Wenn etwas ansteht, dann..Aus der Pipe lesen
        char *move = malloc(sizeof(char)*BUFFERLENGTH_MOVE);
        memset(move,0, strlen(move));
        if((read(fd[0], move, sizeof(move))) < 0){                              //Lese nächsten Spielzug aus der Pipe
          perror("Couldn't read from pipe");                                    //Error, wenn aus der Pipe nicht gelesen werden konnte
        }else {
          sendMove(move, sock);
          rdy = 0;
        }
      } 
    }

  }while(end);                                                                  //Nehme solange Antworten vom Server entgegen bis Server "+ ENDPLAYERS" antwortet
}

void sendMove(char *move, int sock){
  char *response = malloc((sizeof(char)*BUFFERLENGTH_MOVE)+sizeof(move));
  memset(response, 0, strlen(move));
  strcpy(response, "PLAY ");
  strcat(response,move);
  strcat(response,"\n");
  sendResponse(response, sock);
  free(move);
}

void processAndSendResponse(char *buffer, int sock){
  char **requests = malloc(BUFFERLENGTH*sizeof(char*));                         //Speicher für das Array der einzelnen Serveranfragen allokalisieren
  strtoken(buffer, "\n",requests);                                              //Wenn der Server mehrere Anfragen "Unknown requestaufeinmal schickt, werden sie hier in ein String Array eingelesen

  int x = 0;                                                                    //Laufvariable da mehrere Anfragen aufeinmal geschickt werden können
  do{
    end = !match(requests[x]+2,"QUIT");                                         //Wurde +ENDPLAYERS gesendet?
    if(requests[x][0]=='+'){                                                    //Wenn Serveranfrage positiv ausfällt
      if(strlen(requests[x])>2){                                                //Leere Anfrage vom Server ignorieren               //TODO Fehlerbehandlung
        char *response = handle(requests[x]+2);                                 //Sucht die passende Anfrage auf die Serveranfrage
        sendResponse(response, sock);
      }
    }else if(requests[x][0]=='-'){                                              //Wenn Serveranfrage negativ ausfällt
      printf("server: Error! %s\nDisconnecting server...\n",
      requests[x]+2);                                                           //Gebe Fehler aus
      exit(EXIT_FAILURE);                                                       //Beende Programm
    }
    x++;

  }while(requests[x]!=NULL && end);                                             //Solange es noch Anfragen aus dem requests Array gibt und + ENDPLAYERS noch nicht gesendet wurde

  if(buffer!=NULL){                                                             //Wenn der Speicher von buffer noch nicht freigegeben wurde
    free(buffer);                                                               //Speicher von buffer freigeben
  }
  if(requests!=NULL){                                                           //Wenn der Speicher von requests noch nicht freigegeben wurde
    free(requests);                                                             //Speicher von buffer freigeben
  }
}

void sendResponse(char *response, int sock){
  if (response!=NULL){                                                          //Wenn es eine Anfrage gibt
    send(sock,response,strlen(response),0);                                     //Sendet dem Server die Antwort des Clients
    printf("client: %s",response);                                              //Gesendete Antwort ausgeben
  }
  if(response!=NULL){                                                           //Speicher von response freigeben da, in handle Speicher allokalisiert wurde
      free(response);                                                           //Response referenziert auf diesen Speicher
  }
}
