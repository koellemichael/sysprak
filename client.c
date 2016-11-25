#include <stdlib.h>
#include <stdio.h>
#include <String.h>

#define GAMEKINDNAME "Bashni"
#define PORTNUMBER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"

int main (int argc, char **argv){

  char gameid [13];
  if(argc<2){                         //Test: Wird eine Game-ID übergeben
    perror("No game id");             //Keine Game-ID vorhanden
  }
  else{                               //Game-ID vorhanden

    if (strlen(argv[1])!=13){         //Test: Ist die Game-ID 13-stellig
      perror("Invalid game id");      //Game-ID zu kurz oder zu lang
    }
    else {                            //13-stellige Game-ID übergeben
        strcpy(gameid, argv[1]);           //kopieren des Strings nach gameid
      }
      printf("%s\n", gameid);
    }


}
