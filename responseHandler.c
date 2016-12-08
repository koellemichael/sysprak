
#include "responseHandler.h"

int prolog = 1;                                                                 //!Variable für den Fortschritt der Prologphase.


/**
 *Die Funktion handle verarbeitet die Anfrage des Servers zur passenden
 *Antwort des Clients, diese wird zurückgegeben.
 *
 *@param request char* auf die Anfrage/Antwort des Servers
 *@return Antwort des Clients, NULL falls keine passende Antwort
 */
char *handle(char *request){
  char *response = malloc(256*sizeof(char));                                    //Antwortvariable initialisieren
    if(prolog==1 && match(request,                                              //Wenn Anfrage des Servers übereinstimmt und der Prologfortschritt passt
      "MNM Gameserver .+accepting connections")){
      strcpy(response,"VERSION ");                                              //Setze response auf die passende Antwort
      strcat(response, VERSION);                                                //TODO
      prolog++;                                                                 //Prologfortschritt erhöhen, da ein Schritt des Prologs fertig gestellt wurde
    }else if(prolog==2 && match(request,                                        //Wenn Anfrage des Servers übereinstimmt und der Prologfortschritt passt
      "Client version accepted - please send Game-ID to join")){
      strcpy(response,"ID ");                                                   //Setze response auf die passende Antwort
      strcat(response,gameid);                                                  //Game-ID von client.c übernehmen
      prolog++;                                                                 //Prologfortschritt erhöhen, da ein Schritt des Prologs fertig gestellt wurde
    }
    else if(prolog==3 && match(request,                                         //Wenn Anfrage des Servers übereinstimmt und der Prologfortschritt passt
      "PLAYING .+")){
      if(response!=NULL){
        free(response);
      }
      response = NULL;                                                          //Setze response auf die passende Antwort
      prolog++;                                                                 //Prologfortschritt erhöhen, da ein Schritt des Prologs fertig gestellt wurde
    }else if(prolog==4 && match(request,                                        //Wenn Anfrage des Servers übereinstimmt und der Prologfortschritt passt
      ".+")){
      strcpy(response,"PLAYER ");                                               //Setze response auf die passende Antwort
      strcat(response, player);                                                 //Playernummer, falls vorhanden, anhängen
      prolog++;                                                                 //Prologfortschritt erhöhen, da ein Schritt des Prologs fertig gestellt wurde
    }else if(prolog==5 && match(request,                                        //Wenn Anfrage des Servers übereinstimmt und der Prologfortschritt passt
      "YOU .+ .+")){
      if(response!=NULL){
        free(response);
      }
      response = NULL;                                                          //Setze response auf die passende Antwort
      prolog++;                                                                 //Prologfortschritt erhöhen, da ein Schritt des Prologs fertig gestellt wurde
    }else if(prolog==6 && match(request,                                        //Wenn Anfrage des Servers übereinstimmt und der Prologfortschritt passt
     "TOTAL .+")){
     if(response!=NULL){
       free(response);
     }
     response = NULL;                                                           //Setze response auf die passende Antwort
     prolog++;                                                                  //Prologfortschritt erhöhen, da ein Schritt des Prologs fertig gestellt wurde
    }else if(match(request,                                                     //Wenn Anfrage des Servers übereinstimmt und der Prologfortschritt passt
     "ENDPLAYERS")){
     if(response!=NULL){
       free(response);
     }
     response = NULL;                                                           //Setze response auf die passende Antwort
   }else if(prolog>=7 && match(request,                                         //Wenn Anfrage des Servers übereinstimmt und der Prologfortschritt passt
      ".+ .+ .+")){
      if(response!=NULL){
        free(response);
      }
      response = NULL;                                                          //Setze response auf die passende Antwort
      prolog++;                                                                 //Prologfortschritt erhöhen, da ein Schritt des Prologs fertig gestellt wurde
    }else{                                                                      //Ansonsten unbekannte Anfrage des Servers
      if(response!=NULL){
        free(response);
      }
      response = NULL;                                                          //Setze Antwort auf "NULL"
    }

    if(response!=NULL){                                                         //Wenn es eine passende Antwort gibt
      strcat(response,"\n");                                                    //hänge hinten eine new Line and damit der Antwortbefehlt vollständig ist
    }
    return response;                                                            //Gibt die Antwort des Clients zurück
}
