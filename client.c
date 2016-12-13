#include "client.h"

int main (int argc, char **argv){
    
  confile = NULL;      
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
    if(argv[3]!=NULL){
        confile = argv[3];
        printf("Specified configuration file is \"%s\".\n", confile);  
    }
    else{
        confile = "client.conf";
        printf("No configuration file specified. Using default configuration file \"client.conf\". \n"); 
    }
    
      
      
  cp.hostName = readConfiguration(paramNameHost);   
  cp.portNumber = atoi(readConfiguration(paramNamePort));
  cp.gameKindName = readConfiguration(paramNameGame); 
      
  printf("hostname: %s\n", cp.hostName); 
  printf("gamekindname: %s\n", cp.gameKindName);
  printf("portnummer: %i\n", cp.portNumber);

      
    if(connectServer(PORTNUMBER, HOSTNAME) != 0){
        perror("Client failed to call 'connectServer'\n");
        exit(EXIT_FAILURE);
    }

  }
  exit(EXIT_SUCCESS);
}

