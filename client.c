#include "client.h"

int main (int argc, char **argv){
      
  //NULL Initialisierungen verhindern das Erhalten von unbestimmten Werten
  confile = NULL;                                                   
  gameid = NULL;                                                    
  player = NULL;   
    
  //Flags für die optionalen Kommandozeilenargumente überprüfen, ob ein Argument für Player oder die KonfigDatei angegeben wurde
  fflag = 0;    
  pflag = 0;

<<<<<<< HEAD
  if(argc<2){                                                        //Test: Wird eine Game-ID übergeben
    perror("No game id");                                            //Keine Game-ID vorhanden
    exit(EXIT_FAILURE);                                              //Programm beenden
  }
    
  else{ 
    while(optind < argc){                                            //(optind = Index des Arguments) iteriert durch alle Argumente
        if((c = getopt(argc, argv, ":p:f:")) != -1){                 //Parsed die mit flag besetzten Kommandozeilenargumente, bis alle durch sind (dann retval -1)
            switch(c){
                case 'p': player = optarg;                           //p-flag: Wert des Players wird aus der Kommandozeile übernommen
                    pflag = 1;                                       //pflag signalisiert, dass ein Player angegeben wurde
                    break;    
                case 'f': confile = optarg;                          //f-flag: Pfad der Konfigdatei wird aus der Kommandozeile übernommen
                    fflag = 1;                                       //fflag signalisiert, dass eine Konfigdatei angegeben wurde
                    printf("The specified confile is %s.\n", confile); 
                    break;
               //TO-DO: ?PRINT_USAGE OR NOT?
            }
        }
        else{                                                          //Wenn kein Flag angegeben wurde, ist dies die gameid
            //Game-ID vorhanden
            if (strlen(argv[1])!=13){                                  //Test: Ist die Game-ID 13-stellig
                perror("Invalid game id");                             //Game-ID zu kurz oder zu lang
                exit(EXIT_FAILURE);                                    //Programm beenden
            }
            else {                                                     //13-stellige Game-ID übergeben
               gameid=argv[1];                                         //kopieren des Strings nach gameid
            } 
            optind++;                                                  //Der index muss hier manuell erhöht werden
        }
        
=======
  gameid = NULL;
  player = NULL;
  pid_t pid = 0;
  int shmid_serverinfo = -1;
  int shmid_shmid_player = -1;

  shmid_serverinfo = createSHM(sizeof(struct serverinfo));                      //Shared Memory erstellen, für das Serverinformationen struct
  shmid_shmid_player = createSHM(BUFFERLENGTH*sizeof(int));                     //Shared Memory erstellen, in diesem Segment werden die shmids der einzelnen Players

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
>>>>>>> master
    }
    
      
      
    if(pflag == 0){                                                     //Wenn kein Player angegeben wurde
        player = "";                                                    
        printf("No Player specified.");
    }
<<<<<<< HEAD
    if(fflag == 0){                                                     //Wenn keine Konfigdatei angegeben wurde
         confile = "client.conf";                                       //Default Konfigdatei ist client.conf
        printf("No configuration file specified. Using default configuration file \"client.conf\". \n"); 
    }
      
    
  //Struct befüllen mit den Werten zur Verbindung mit dem Server
  cp.hostName = readConfiguration(paramNameHost);   
  cp.portNumber = atoi(readConfiguration(paramNamePort));
  cp.gameKindName = readConfiguration(paramNameGame); 
      
  printf("hostname: %s\n", cp.hostName); 
  printf("gamekindname: %s\n", cp.gameKindName);
  printf("portnummer: %i\n", cp.portNumber);

      
    if(connectServer(PORTNUMBER, HOSTNAME) != 0){
        perror("Client failed to connect to server'\n");
        exit(EXIT_FAILURE);
=======
    else {                                                                      //Kein Player eingegeben
      player = "";                                                              //Player leer lassen
    }
  }

  if((pid=fork())<0){                                                           //Aufsplitten des Prozesses
    perror("Error while splitting the process!");                               //Fehler bei fork
    exit(EXIT_FAILURE);

  } else if(pid == 0){                                                          //Kindprozess: Prozess-ID == 0
    //CONNECTOR
    serverinfo = attachSHM(shmid_serverinfo);                                   //Shared Memory Segment anbinden
    shmid_player = attachSHM(shmid_shmid_player);                               //Shared Memory Segment anbinden

    if(connectServer(PORTNUMBER, HOSTNAME) != 0){                               //Aufruf connectServer und damit performConnection
      perror("Client failed to call 'connectServer'\n");                        //Fehler bei Verbindung zum Server
      exit(EXIT_FAILURE);
>>>>>>> master
    }

    kill(serverinfo->pid_thinker, SIGCONT);                                     //Signal damit Thinker weiterarbeiten kann und somit den playershm attachen kann

  } else {                                                                      //Elternprozess: Prozess-ID > 0
    //THINKER
    serverinfo = attachSHM(shmid_serverinfo);                                   //Shared Memory Segment anbinden
    shmid_player = attachSHM(shmid_shmid_player);                               //Shared Memory Segment anbinden

    serverinfo->pid_thinker = getpid();                                         //pid vom Parent im struct speichern
    serverinfo->pid_connector = pid;                                            //pid vom Child im struct speichern

    pause();                                                                    //Pause bis Signal kommt

    for(int i = 0; i<serverinfo->totalplayers-1; i++){                          //Shared Memory Segment jedes Spielers attachen und im struc speichern
      serverinfo->otherplayers[i] = attachSHM(shmid_player[i]);
    }

    if(waitpid(pid,NULL,0) != pid){                                             //Warten bis der Kindprozess terminiert
      perror("Error while waiting for childprocess");
      exit(EXIT_FAILURE);
    }

    for(int i = 0; i<serverinfo->totalplayers-1; i++){                          //Delete Shared Memory jedes Spielers
      deleteSHM(shmid_player[i]);
    }
    deleteSHM(shmid_shmid_player);                                              //Delete Shared Memory
    deleteSHM(shmid_serverinfo);                                                //Delete Shared Memory
  }
  exit(EXIT_SUCCESS);
}

