#include <stdlib.h>
#include <stdio.h>
#include <String.h>
#include "responseHandler.h"
#include "processRequest.h"

#define VERSION "2.3"                                                           //TODO provisorische Konstante
#define PLAYERNR ""                                                             //TODO provisorische Konstante

extern char *gameid;                                                            //!gameid. Zugriff auf Globale Variable in client.c
int prolog = 1;                                                                 //!Variable für den Fortschritt der Prologphase

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
      response = NULL;                                                          //Setze response auf die passende Antwort
      prolog++;                                                                 //Prologfortschritt erhöhen, da ein Schritt des Prologs fertig gestellt wurde
    }else if(prolog==4 && match(request,                                        //Wenn Anfrage des Servers übereinstimmt und der Prologfortschritt passt
      ".+")){
      strcpy(response,"PLAYER ");                                               //Setze response auf die passende Antwort
      strcat(response, PLAYERNR);                                               //TODO
      prolog++;                                                                 //Prologfortschritt erhöhen, da ein Schritt des Prologs fertig gestellt wurde
    }else if(prolog==5 && match(request,                                        //Wenn Anfrage des Servers übereinstimmt und der Prologfortschritt passt
      "YOU .+ .+")){
      response = NULL;                                                          //Setze response auf die passende Antwort
      prolog++;                                                                 //Prologfortschritt erhöhen, da ein Schritt des Prologs fertig gestellt wurde
    }else if(prolog==6 && match(request,                                        //Wenn Anfrage des Servers übereinstimmt und der Prologfortschritt passt
     "TOTAL .+")){
     response = NULL;                                                           //Setze response auf die passende Antwort
     prolog++;                                                                  //Prologfortschritt erhöhen, da ein Schritt des Prologs fertig gestellt wurde
    }else if(match(request,                                                     //Wenn Anfrage des Servers übereinstimmt und der Prologfortschritt passt
     "ENDPLAYERS")){
     response = NULL;                                                           //Setze response auf die passende Antwort
   }else if(prolog>=7 && match(request,                                         //Wenn Anfrage des Servers übereinstimmt und der Prologfortschritt passt
      ".+ .+ .+")){
      response = NULL;                                                          //Setze response auf die passende Antwort
      prolog++;                                                                 //Prologfortschritt erhöhen, da ein Schritt des Prologs fertig gestellt wurde
    }else{                                                                      //Ansonsten unbekannte Anfrage des Servers
      strcpy(response,"Unknown request");                                       //Setze Antwort auf "unknown request"
    }

    if(response!=NULL){                                                         //Wenn es eine passende Antwort gibt
      strcat(response,"\n");                                                    //hänge hinten eine new Line and damit der Antwortbefehlt vollständig ist
    }
    return response;                                                            //Gibt die Antwort des Clients zurück
}
