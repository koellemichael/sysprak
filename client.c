#include "client.h"

void attachPlayers(int sig){
  sig = 0;
  printf("%i\n", sig);
  for(int i = 0; i<serverinfo->totalplayers-1; i++){                          //Shared Memory Segment jedes Spielers attachen und im struct speichern
    serverinfo->otherplayers[i] = attachSHM(shmid_player[i]);
  }
}

int main (int argc, char **argv){
  confile = NULL;
  gameid = NULL;
  player = NULL;
  gameid = NULL;
  player = NULL;
  fflag = 0;                                                                    //Flags für die optionalen Kommandozeilenargumente überprüfen, ob ein Argument für Player oder die KonfigDatei angegeben wurde
  pflag = 0;
  pid_t pid = 0;
  int shmid_serverinfo = -1;
  int shmid_shmid_player = -1;
  shmid_serverinfo = createSHM(sizeof(struct serverinfo));                      //Shared Memory erstellen, für das Serverinformationen struct
  shmid_shmid_player = createSHM(BUFFERLENGTH*sizeof(int));                     //Shared Memory erstellen, in diesem Segment werden die shmids der einzelnen Players
  signal(SIGUSR1, think);
  signal(SIGUSR2, attachPlayers);

  if(argc<2){                                                                   //Test: Wird eine Game-ID übergeben
    perror("No game id");                                                       //Keine Game-ID vorhanden
    exit(EXIT_FAILURE);                                                         //Programm beenden
  }else{
    printLogo();
    while(optind < argc){                                                       //(optind = Index des Arguments) iteriert durch alle Argumente
        if((c = getopt(argc, argv, ":p:f:")) != -1){                            //Parsed die mit flag besetzten Kommandozeilenargumente, bis alle durch sind (dann retval -1)
            switch(c){
                case 'p': player = optarg;                                      //p-flag: Wert des Players wird aus der Kommandozeile übernommen
                          pflag = 1;                                            //pflag signalisiert, dass ein Player angegeben wurde
                          break;
                case 'f': confile = optarg;                                     //f-flag: Pfad der Konfigdatei wird aus der Kommandozeile übernommen
                          fflag = 1;                                            //fflag signalisiert, dass eine Konfigdatei angegeben wurde
                          printf("The specified confile is %s.\n", confile);
                          break;
                default:
                          break;
            }
        }else{                                                                  //Wenn kein Flag angegeben wurde, ist dies die gameid
            //Game-ID vorhanden
            if (strlen(argv[1])!=13){                                           //Test: Ist die Game-ID 13-stellig
                perror("Invalid game id");                                      //Game-ID zu kurz oder zu lang
                exit(EXIT_FAILURE);                                             //Programm beenden
            }else {                                                             //13-stellige Game-ID übergeben
               gameid=argv[1];                                                  //kopieren des Strings nach gameid
            }
            optind++;                                                           //Der index muss hier manuell erhöht werden
        }
    }

    if(pflag == 0){                                                             //Wenn kein Player angegeben wurde
      player = "";
      printf("No Player specified\n");
    }
    if(fflag == 0){                                                             //Wenn keine Konfigdatei angegeben wurde
      printf("No configuration file specified\n");
      confile = "client.conf";                                                  //Default Konfigdatei ist client.conf
      createDefaultConfig();
      printf("Using default configuration file \"%s\"\n",confile);
    }

    //Struct befüllen mit den Werten zur Verbindung mit dem Server
    cp.hostName = readConfiguration(paramNameHost);
    cp.portNumber = atoi(readConfiguration(paramNamePort));
    cp.gameKindName = readConfiguration(paramNameGame);
  }


  //Unnamed Pipe einrichten, die zwischen Connector und Thinker laufen soll
  if(pipe(fd) < 0){
      perror("Error establishing an unnamed pipe");
      exit(EXIT_FAILURE);
  }


  if((pid=fork())<0){                                                           //Aufsplitten des Prozesses
    perror("Error while splitting the process");                                //Fehler bei fork
    exit(EXIT_FAILURE);
  } else if(pid == 0){                                                          //Kindprozess: Prozess-ID == 0
      //CONNECTOR
      int sock;

      //Schreibeseite der Pipe schließen
      close(fd[1]);

      //Shared Memory Segmente anbinden
      serverinfo = attachSHM(shmid_serverinfo);
      shmid_player = attachSHM(shmid_shmid_player);

      sock = connectServer(cp.portNumber, cp.hostName);                         //Aufruf connectServer
      performConnection(sock);                                                  //Abarbeitung der Prologphase

      //Schliesst das Socket
      close(sock);
  } else {                                                                      //Elternprozess: Prozess-ID > 0
    //THINKER
    //Leseseite der Pipe schließen
    close(fd[0]);

    //Shared Memory Segmente anbinden
    serverinfo = attachSHM(shmid_serverinfo);
    shmid_player = attachSHM(shmid_shmid_player);

    serverinfo->pid_thinker = getpid();                                         //pid vom Parent im struct speichern
    serverinfo->pid_connector = pid;                                            //pid vom Child im struct speichern

    if(waitpid(pid,NULL,0) != pid){                                             //Warten bis der Kindprozess terminiert
      perror("Error while waiting for childprocess");
      exit(EXIT_FAILURE);
    }

    //Löschen der Shared Memory Segmente
    if(shmid_player != 0){
      for(int i = 0; i<serverinfo->totalplayers-1; i++){
        deleteSHM(shmid_player[i]);
      }
    }
    //Löschen der Shared Memory Segmente
    deleteSHM(shmid_shmid_player);
    deleteSHM(shmid_serverinfo);
  }
  exit(EXIT_SUCCESS);
}
