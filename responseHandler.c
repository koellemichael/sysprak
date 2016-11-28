#include <stdlib.h>
#include <stdio.h>
#include <String.h>
#include "responseHandler.h"

#define VERSION "2.3"                                                           //TODO provisorische Konstante
#define PLAYERNR ""                                                             //TODO provisorische Konstante

extern char *gameid;                                                            //!gameid. Zugriff auf Globale Variable in client.c
int prolog = 1;                                                                 //!Variable für den Fortschritt der Prologphase

/**
 *Die Funktion removeSubstring ertfernt einen Teilstring aus einem gegebenen String.
 *
 *@param string Der String aus dem der Teilsting gelöschickt werden soll
 *@param toRemove Der zu entfernende Teilstring
 */
void removeSubstring(char *string, char *toRemove){
  string = strstr(string,toRemove);                                             //Adresse des Anfangs des Teilstrings im String
  memmove(string,string+strlen(toRemove),strlen(string+strlen(toRemove)));      //Speicherbereich so verschieben das der Teilstring überschrieben wird
  string[strlen(string)-strlen(toRemove)] = 0;                                  //Ende des Strings festlegen
}

/**
 *Die Funktion substring extrahiert einen Teilstring von from bis to aus einem
 *gegebenen String und gibt ihn als char Pointer zurück.
 *
 *@param string Der String aus dem der Teilsting extrahiert werden soll
 *@param from Anfangsposition des Teilstrings als int
 *@param to Endposition des Teilstrings als int
 *@return Teilstring von from bis to als char* oder NULL falls ein Fehler auftritt.
 */
char *substring(char *string, unsigned int from, unsigned int to){
  if(from>=to || to>strlen(string)){                                            //Unsinnige Eingaben abfangen
    return NULL;                                                                //NULL zurückgeben falls
  }else{                                                                        //Falls sinbvolle Eingabe
    char *substring = malloc((to-from)*sizeof(char));                           //Speicher füresponse substring allokalisieren
    strncpy(substring,string+from,(to-from));                                   //Teilstring in substring speichern
    substring[(to-from)]=0;                                                     //Teilstringende festlegen
    return substring;                                                           //Substring zurückgeben
  }
}

/**
 *Partitioniert einen String, wenn im String Ausdrücke in spitzen
 *Klammern <Ausdruck> vorhanden sind.
 *Speichert alle Teilstrings in einem gegebenen Array.
 *
 *@param string Der zu partitionierende String
 *@param i Index an welchem der Teilstring im Array gespeichert werden soll
 *@param array Array in dem die Teilstrings gespeichert werden sollen
 */
void partition(char *string, int i, char **array){
  if((strcspn(string, "<")!=strlen(string))                                     //Enthält es < und >?
  &&(strcspn(string, ">")!=strlen(string))){
    unsigned int end_part1 = strcspn(string,"<");                               //Ende des ersten Teilstrings festlegen
    unsigned int start_part2 = strcspn(string,">")+1;                           //Anfang des 2. Teilstrings festlegen
    if(end_part1>=1)                                                            //Startpunkt des Teilstrings muss >0 sein
        array[i] = substring(string,0,end_part1);                               //Speichere den Teilstring bis zum ersten Vorkommen vom n-ten '<'
    if(start_part2<strlen(string))                                              //Anfang des 2 Parts darf nicht über den String hinausgehen
        partition(substring(string,start_part2,strlen(string)),(i+1),array);    //Rekusiver Aufruf da in diesem Teilstring immernoch <Ausdrücke> sein können.
  }else{
      array[i] = string;                                                        //Falls keine spitzen Klammern oder keine mehr vorhanden sind, speichere letzten Teilstring im Array
  }
}

/**
 *Die Funktion reqcmp (request Comparison), vergleicht eine Antwort vom
 *Server mit der Erwartung (expectation), dabei ignoriert die Funktion
 *geklammerte Ausdrücke. Bsp.: <Version>
 *
 *@param request Antwort des Servers
 *@param expectation Erwartete Antwort des Servers. Variable Ausdrücke in spitzen Klammern <Ausdruck>.
 *@return 0 wenn verschieden,1 wenn gleich
 */
int reqcmp(char *request, char *expectation){
  char *req = malloc(strlen(request)*sizeof(char));                             //Speicher anfordern
  strcpy(req, request);                                                         //Kopieren des Requests in eine Variable damit nicht der eigentliche Request verändert wird
  int eq = 1;                                                                   //Variable zum Überprüfen ob einer Teilstrings Differenzen zur request aufweist
  int substrings = (strlen(expectation)/3)+1;                                   //Länge des zu erstellenden Arrays festlegen. Kann maximal 1/3 des zu erwartenden
                                                                                //Strings sein da im Worst Case immer zwei Klammern auf ein Zwichen folgen
  char **array = malloc(substrings*sizeof(char*));                              //Speicherplatz füresponse die Pointer des Arrays allokalisieren
  for(int i = 0; i< substrings;i++){
    array[i] = NULL;                                                            //Jeden String im Array mit NULL initialisieren
  }

  partition(expectation,0,array);                                               //Alle Teilstrings in das Array laden

  for(int i = 0; i< substrings;i++){                                            //Pro Eintrag im Array vergleichen ob es Abweichungen zu request gibt
    if(array[i]!=NULL){                                                         //Falls der Eintrag nicht NULL ist
        if(strstr(req, array[i]) == NULL){                                      //Falls es Abweichungen gibt
            eq = 0;                                                             //Setze eq = 0
        }else{                                                                  //Falls nicht
          removeSubstring(req, array[i]);                                       //Entfernt gefundenen Teilstring aus der Server request
                                                                                //damit jeder Teilstring der request nur ein mal gefunden werden kann
        }
    }
  }

  if(array != NULL){                                                            //Speicherplatz des Arrays freigeben
    free(array);
  }

  if(req != NULL){                                                              //Speicherplatz von req freigeben
    free(req);
  }

  return eq;                                                                    //Ergebnis eq zurückgeben
}

/**
 *Die Funktion handle verarbeitet die Anfrage des Servers zur passenden
 *Antwort des Clients, diese wird zurückgegeben.
 *
 *@param request char* auf die Anfrage/Antwort des Servers
 *@return Antwort des Clients, NULL falls keine passende Antwort
 */
char *handle(char *request){
  char *response = malloc(256*sizeof(char));                                    //Antwortvariable initialisieren
    if(prolog==1 && reqcmp(request,                                             //Wenn Anfrage des Servers übereinstimmt und der Prologfortschritt passt
      "MNM Gameserver <Gameserver Version> accepting connections")){
      strcpy(response,"VERSION ");                                              //Setze response auf die passende Antwort
      strcat(response, VERSION);                                                //TODO
      prolog++;                                                                 //Prologfortschritt erhöhen, da ein Schritt des Prologs fertig gestellt wurde
    }else if(prolog==2 && reqcmp(request,                                       //Wenn Anfrage des Servers übereinstimmt und der Prologfortschritt passt
      "Client version accepted - please send Game-ID to join")){
      strcpy(response,"ID ");                                                   //Setze response auf die passende Antwort
      strcat(response,gameid);                                                  //Game-ID von client.c übernehmen
      prolog++;                                                                 //Prologfortschritt erhöhen, da ein Schritt des Prologs fertig gestellt wurde
    }
    else if(prolog==3 && reqcmp(request,                                        //Wenn Anfrage des Servers übereinstimmt und der Prologfortschritt passt
      "PLAYING <Gamekind-Name>")){
      response = NULL;                                                          //Setze response auf die passende Antwort
      prolog++;                                                                 //Prologfortschritt erhöhen, da ein Schritt des Prologs fertig gestellt wurde
    }else if(prolog==4 && reqcmp(request,                                       //Wenn Anfrage des Servers übereinstimmt und der Prologfortschritt passt
      "<Game-Name>")){
      strcpy(response,"PLAYER ");                                               //Setze response auf die passende Antwort
      strcat(response, PLAYERNR);                                               //TODO
      prolog++;                                                                 //Prologfortschritt erhöhen, da ein Schritt des Prologs fertig gestellt wurde
    }else if(prolog==5 && reqcmp(request,                                       //Wenn Anfrage des Servers übereinstimmt und der Prologfortschritt passt
      "YOU <Spielernummer> <Spielername>")){
      response = NULL;                                                          //Setze response auf die passende Antwort
      prolog++;                                                                 //Prologfortschritt erhöhen, da ein Schritt des Prologs fertig gestellt wurde
    }else if(prolog==6 && reqcmp(request,                                       //Wenn Anfrage des Servers übereinstimmt und der Prologfortschritt passt
     "TOTAL <Spieleranzahl>")){
     response = NULL;                                                           //Setze response auf die passende Antwort
     prolog++;                                                                  //Prologfortschritt erhöhen, da ein Schritt des Prologs fertig gestellt wurde
    }else if(reqcmp(request,                                                    //Wenn Anfrage des Servers übereinstimmt und der Prologfortschritt passt
     "ENDPLAYERS")){
     response = NULL;                                                           //Setze response auf die passende Antwort
    }else if(prolog>=7 && reqcmp(request,                                       //Wenn Anfrage des Servers übereinstimmt und der Prologfortschritt passt
      "<Spielernummer> <Spielername> <Bereit>")){
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
