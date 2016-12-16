#include "client.h"

int main (int argc, char **argv){

  gameid = NULL;
  player = NULL;
  pid_t pid = 0;
  int shmid_info = -1;
  int playeridshm = -1;

  if((shmid_info = shmget(KEY, sizeof(struct serverinfo), PERMISSION)) == -1){  //Shared Memory erstellen
      perror("Error while generating Shared Memory");                           //Fehlerbehandlung falls Fehler bei Erstellung
      exit(EXIT_FAILURE);
  }

  if((playeridshm = shmget(KEY, BUFFERLENGTH*sizeof(int), PERMISSION)) == -1){     //Shared Memory erstellen
      perror("Error while generating Shared Memory");                           //Fehlerbehandlung falls Fehler bei Erstellung
      exit(EXIT_FAILURE);
  }

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
      gameid=argv[1];                                                           //kopieren des Strings nach gameid
    }

    if (argv[2]!=NULL){                                                         //Wurder Player eingegeben?
      player = argv[2];
    }
    else {                                                                      //Kein Player eingegeben
      player = "";                                                              //Player leer lassen
    }
  }

  if((pid=fork())<0){                                                           //Aufsplitten des Prozesses
    perror("Error while splitting the process!");                               //Fehler bei fork
    exit(EXIT_FAILURE);

  } else if(pid == 0){                                                          //Kindprozess: Prozess-ID == 0
    //CONNECTOR
    serverinfo = (struct serverinfo*) shmat(shmid_info, 0, 0);                              //Shared Memory anbinden
    if(serverinfo == NULL){                                                           //Fehlerbehandlung falls shmat fehlschlägt
      perror("Error while attching Shared Memory in Connector process");
      exit(EXIT_FAILURE);
    }

    playerid = (int*) shmat(playeridshm, 0, 0);                                  //Shared Memory anbinden
    if(playerid == NULL){                                                       //Fehlerbehandlung falls shmat fehlschlägt
      perror("Error while attching Shared Memory in Connector process");
      exit(EXIT_FAILURE);
    }

    if(connectServer(PORTNUMBER, HOSTNAME) != 0){                               //Aufruf connectServer und damit performConnection
      perror("Client failed to call 'connectServer'\n");                        //Fehler bei Verbindung zum Server
      exit(EXIT_FAILURE);
    }

    kill(serverinfo->pid_thinker, SIGCONT);

    printf(" gamename: %s\n clientplayernr: %i\n totalplayers: %i\n connector: %i\n thinker: %i\n",serverinfo->gamename,serverinfo->clientplayernr,serverinfo->totalplayers,serverinfo->pid_connector,serverinfo->pid_thinker);
    for(int i = 0; i<serverinfo->totalplayers-1; i++){
      printf("  PLAYER [%i]:\n  playernr: %i\n  playername: %s\n  isready: %i\n",i,serverinfo->otherplayers[i]->playernr,serverinfo->otherplayers[i]->playername,serverinfo->otherplayers[i]->ready);
    }
    printf("\n");

  } else {                                                                      //Elternprozess: Prozess-ID > 0
    //THINKER
    serverinfo = (struct serverinfo*) shmat(shmid_info, 0, 0);                              //Shared Memory anbinden
    if(serverinfo == NULL){                                                           //Fehlerbehandlung falls shmat fehlschlägt
      perror("Error while attching Shared Memory in Thinker process");
      exit(EXIT_FAILURE);
    }
    playerid = (int*) shmat(playeridshm, 0, 0);                                 //Shared Memory anbinden
    if(playerid == NULL){                                                       //Fehlerbehandlung falls shmat fehlschlägt
      perror("Error while attching Shared Memory in Thinker process");
      exit(EXIT_FAILURE);
    }

    serverinfo->pid_thinker = getpid();                                             //pid vom Parent im struct speichern
    serverinfo->pid_connector = pid;                                                //pid vom Child im struct speichern

    pause();

    for(int i = 0; i<serverinfo->totalplayers-1; i++){
      serverinfo->otherplayers[i] = (struct player*) shmat(playerid[i], 0, 0);                   //Shared Memory anbinden
      if(serverinfo->otherplayers[i] == NULL){                                                   //Fehlerbehandlung falls shmat fehlschlägt
        perror("Error while attching Shared Memory in Thinker process");
        exit(EXIT_FAILURE);
      }
    }

    printf(" gamename: %s\n clientplayernr: %i\n totalplayers: %i\n connector: %i\n thinker: %i\n",serverinfo->gamename,serverinfo->clientplayernr,serverinfo->totalplayers,serverinfo->pid_connector,serverinfo->pid_thinker);
    for(int i = 0; i<serverinfo->totalplayers-1; i++){
      printf("  PLAYER [%i]:\n  playernr: %i\n  playername: %s\n  isready: %i\n",i,serverinfo->otherplayers[i]->playernr,serverinfo->otherplayers[i]->playername,serverinfo->otherplayers[i]->ready);
    }

    if(waitpid(pid,NULL,0) != pid){
      perror("Error while waiting for childprocess");
      exit(EXIT_FAILURE);
    }



    //Delete Shared Memory
    for(int i = 0; i<serverinfo->totalplayers-1; i++){
      if(shmctl(playerid[i], IPC_RMID, 0)!=0){
        perror("Error while deleting Shared Memory playerid");
      }
    }
    if(shmctl(playeridshm, IPC_RMID, 0)!=0){
      perror("Error while deleting Shared Memory playeridshm");
    }
    if(shmctl(shmid_info, IPC_RMID, 0)!=0){
      perror("Error while deleting Shared Memory shm");
    }
  }

  exit(EXIT_SUCCESS);
}
