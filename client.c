#include "client.h"


int main (int argc, char **argv){

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
