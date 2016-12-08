#include "client.h"

int main (int argc, char **argv){
    
    // Kathis Codestück zur Konfigdatei beginnt hier 
    
/*
 * Hier wird geprüft, ob ein optionaler Kommandozeilenparameter mit dem flag "-f" angegeben wurde
 * Wenn nicht, dann wird die Standard Konfigurationsdatei "client.conf" verwendet
 * Wenn eine zusätzlicher Parameter als Dateipfad angegeben wurde, dann wird diese Datei verwendet
 * Danach ruft sie die Funkion readConfiguration() auf mit der gewünschten Konfigurationsdatei.
 */
     
    int c;                                                                                   
    char *confile;                                                                           //Der Dateipfad der Konfigurationsdatei, 
                                                                                             //die verwendet werden soll
    c = getopt(argc, argv, ":f:");                                                           //Return Value von Getopt wird hier gespeichert  
        switch(c){                                                                           // Wenn ein Argument...
            case 'f': confile = optarg;                                                      // ..das flag f hat: ist dies die verwendete Config File
                    printf("Specified configuration file is \"%s\".\n", confile);               
                    break;
            default: confile = "client.conf";                                                //..kein flag f hat: Standard Config wird verwendet
                    printf("No configuration file specified. Using default configuration file \"client.conf\". \n"); 
        }
    
    readConfiguration(confile);                                                              //Funktionsaufruf readConfiguration(confile)
    
    //Kathis Codestück zur Konfigdatei endet hier
    
    

  gameid = NULL;
  player = NULL;

  if(argc<2){                                                                   //Test: Wird eine Game-ID übergeben
    perror("No game id");                                                       //Keine Game-ID vorhanden
    exit(EXIT_FAILURE);                                                         //Programm beenden
  }
  else{                                                                         //Game-ID vorhanden
    if (strlen(argv[1])!=13){                                                   //Test: Ist die Game-ID 13-stellig
      perror("Invalid game id");                                                //Game-ID zu kurz oder zu lang
      exit(EXIT_FAILURE);                                                       //Programm beenden
    }
    else {                                                                      //13-stellige Game-ID übergeben
        gameid=argv[1];                                                         //kopieren des Strings nach gameid
    }

    if (argv[2]!=NULL){                                                         //Wurder Player eingegeben?
      player = argv[2];
    }
    else {                                                                      //Kein Player eingegeben
        player = "";                                                            //Player leer lassen
    }

    if(connectServer(PORTNUMBER, HOSTNAME) != 0){
        perror("Client failed to call 'connectServer'\n");
        exit(EXIT_FAILURE);
    }

  }
  exit(EXIT_SUCCESS);
}
