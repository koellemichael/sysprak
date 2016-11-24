#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "responseHandler.h"

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
  if(from>=to || to>strlen(string)){                                  //Unsinnige Eingaben abfangen
    return NULL;                                                                //NULL zurückgeben falls
  }else{                                                                        //Falls sinbvolle Eingabe
    char *substring = malloc((to-from)*sizeof(char));                           //Speicher für substring allokalisieren
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
    unsigned int end_part1 = strcspn(string,"<");                                        //Ende des ersten Teilstrings festlegen
    unsigned int start_part2 = strcspn(string,">")+1;                                    //Anfang des 2. Teilstrings festlegen
    if(end_part1>=1)                                                            //Startpunkt des Teilstrings muss >0 sein
        array[i] = substring(string,0,end_part1);                               //Speichere den Teilstring bis zum ersten Vorkommen vom n-ten '<'
    if(start_part2<strlen(string))                                              //Anfang des 2 Parts darf nicht über den String hinausgehen
        partition(substring(string,start_part2,strlen(string)),(i+1),array);    //Rekusiver Aufruf da in diesem Teilstring immernoch <Ausdrücke> sein können.
  }else{
      array[i] = string;                                                        //Falls keine spitzen Klammern oder keine mehr vorhanden sind, speichere letzten Teilstring im Array
  }
}

/**
 *Die Funktion rescmp (Response Comparison), vergleicht eine Antwort vom
 *Server mit der Erwartung (expectation), dabei ignoriert die Funktion
 *geklammerte Ausdrücke. Bsp.: <Version>
 *
 *@param response Antwort des Servers
 *@param expectation Erwartete Antwort des Servers. Variable Ausdrücke in spitzen Klammern <Ausdruck>.
 *@return 0 wenn verschieden,1 wenn gleich
 */
int rescmp(char *response, char *expectation){
  int eq = 1;                                                                   //Variable zum Überprüfen ob einer Teilstrings Differenzen zur Response aufweist
  int substrings = (strlen(expectation)/3)+1;                                   //Länge des zu erstellenden Arrays festlegen. Kann maximal 1/3 des zu erwartenden
                                                                                //Strings sein da im Worst Case immer zwei Klammern auf ein Zwichen folgen
  char **array = malloc(substrings*sizeof(char*));                              //Speicherplatz für die Pointer des Arrays allokalisieren
  for(int i = 0; i< substrings;i++){
    array[i] = NULL;                                                            //Pointer mit NULL initialisieren
  }

  partition(expectation,0,array);                                               //Alle Teilstrings in das Array laden

  for(int i = 0; i< substrings;i++){                                            //Pro Eintrag im Array vergleichen ob es Abweichungen zu Response gibt
    if(array[i]!=NULL){                                                         //Falls der Eintrag nicht NULL ist
        if(strstr(response, array[i]) == NULL){                                 //Falls es Abweichungen gibt
            eq = 0;                                                             //Setze eq = 0
        }
    }
  }

  if(array != NULL){                                                            //Speicherplatz des Arrays freigeben
    free(array);
  }
  return eq;                                                                    //Ergebnis eq zurückgeben
}

/**
 *Die Funktion handle verarbeitet die Anfrage des Servers zur passenden
 *Antwort des Clients, diese wird zurückgegeben.
 *
 *@param response char* auf die Anfrage/Antwort des Servers
 *@return Antwort des Clients, NULL falls keine passende Antwort
 */
char *handle(char *response){
  char *r = NULL;                                                               //Antwortvariable initialisieren
    if(rescmp(response,                                                         //Wenn Anfrage des Servers übereinstimmt
      "+ MNM Gameserver <Gameserver Version> accepting connections")){
      r = "VERSION <Client Version>";                                           //setze r auf die passende Antwort
    }else if(rescmp(response,
      "+ Client version accepted - please send Game-ID to join")){
      r = "ID <Game-ID>";
    }
    else if(rescmp(response,
      "+ PLAYING <Gamekind-Name>")){
      r = NULL;
    }else if(rescmp(response,
      "+ Bashni")){
      r = "PLAYER [Gewunschte Spielernummer]";
    }else if(rescmp(response,
      "+ YOU <Spielernummer> <Spielername>")){
      r = NULL;
    }else if(rescmp(response,
      "+ <Spielernummer> <Spielername> <Bereit>")){
      r = NULL;
    }else if(rescmp(response,
     "+ TOTAL <Spieleranzahl>")){
     r = NULL;
    }
    return r;                                                                   //Gibt die Antwort des Clients zurück
}
