#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "performConnection.h"

#define GAMEKINDNAME "Bashni"
#define PORTNUMBER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"

int main (int argc, char **argv){

  char gameid [13];
  if(argc<2){                         //Test: Wird eine Game-ID übergeben
    perror("No game id");             //Keine Game-ID vorhanden
    exit(EXIT_FAILURE);               //Programm beenden
  }
  else{                               //Game-ID vorhanden

    if (strlen(argv[1])!=13){         //Test: Ist die Game-ID 13-stellig
      perror("Invalid game id");      //Game-ID zu kurz oder zu lang
      exit(EXIT_FAILURE);             //Programm beenden
    }
    else {                            //13-stellige Game-ID übergeben
      for(int i=0; i<13; i++){        //for-Schleife um Game-ID auszulesen
        gameid[i]=argv[1][i];         //speichern der Game-ID in gameid
      }
      printf("%s\n", gameid);
    }
  }

    int *fd = malloc(2*sizeof(int));
    performConnection(fd);
    if(fd!=NULL){
      free(fd);
    }

}
