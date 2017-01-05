#include "connectServer.h"

/**
 * connectServer soll den Client mit dem Gameserver verbinden.
 * Dazu wird ein Socket implementiert.
 * Der Hostname wird in eine IP Adresse umgewandelt.
 * Zusätzlich wird die Methode performconnetion mit dem Socket aufgerufen.
 * @param portnumber Port auf den Verbunden werden soll
 * @param hostname Adresse auf die verbunden werden soll
 * @return Filedeskriptor Socket
 */
int connectServer(int portnumber, char *hostname){
  int sock;                                                                     //Filedeskriptor des Sockets
  struct hostent *he;                                                           //Hostip
  struct sockaddr_in addr;                                                      //Die Struktur enthält Informationen über Adresse, Port und Internetprotokollfamilie des Gameservers

  //SOCKET ANLEGEN
  /* PF_INET: Internet Protokoll Version 4
   * SOCK_STREAM: Typ des Sockets.(Gegensatz: SOCK_DGRAM). Stream Zuverlässiger.
   * 0: Default Transportprotokoll des Sockettyps. Von SOCK_STREAM ist dies TCP.
   */
  sock = socket(PF_INET, SOCK_STREAM, 0);
  if(sock == -1){
    perror("Could not generate socket");
    exit(EXIT_FAILURE);
  }

  //HOSTNAME in IP Adresse umwandeln
  if( (he = gethostbyname(hostname)) == NULL){                                  //Hostname von Client übergeben
    perror("Could not resolve hostname");
    exit(EXIT_FAILURE);
  }

  /* Die Struktur enthält Informationen über Adresse, Port und Internetprotokollfamilie des Gameservers
   * sin_family: Wird die Protokollfamilie zugewiesen (PF_INET für Internet-Protokoll Version 4)
   * sin_port: Speichert den IP-Port (=Endpunkt, der zur Kommunikation mit Netzwerk belegt wird)
   * sin_addr: Speichert die IP-Adresse des Gameservers
   */
  addr.sin_family = AF_INET;
  addr.sin_port = htons(portnumber);
  memcpy(&addr.sin_addr.s_addr, he->h_addr_list[0], he -> h_length);

  //Socket auf non blocking stellen
  long arg;

  if((arg = fcntl(sock, F_GETFL, NULL)) < 0){                                   //Filedeskriptorstatus anzeigen, prüfen ob Rechte gegeben sind den Modus zu wechseln
    perror("Could not get socket status");
    exit(EXIT_FAILURE);
  }
  arg |= O_NONBLOCK;
  if(fcntl(sock, F_SETFL, arg) < 0){                                            //Filedeskriptorstatus auf nonblocking setzen
    perror("Could not set socket status to nonblocking");
    exit(EXIT_FAILURE);
  }


  printf("Connecting to %s:%i... \n", hostname, portnumber);
  int res = connect(sock, (struct sockaddr *)&addr, sizeof(addr));              //Versuche Verbindung herzustellen

  fd_set fset;
  struct timeval tv;

  if (res < 0) {                                                                //Da non blocking wird dieser Teil während der Connection ausgeführt
      do {
         tv.tv_sec = TIMEOUT;
         tv.tv_usec = 0;
         FD_ZERO(&fset);
         FD_SET(sock, &fset);
         res = select(sock+1, NULL, &fset, NULL, &tv);                          //Wenn socket nach TIMEOUT Sekunden nicht connected hat wird die Verbindung abgebrochen
         if (res > 0) {
            printf("Successfully connected to server\n\n");
            break;
         }else if (res < 0 && errno != EINTR) {
            perror("Could not connect to server");
            exit(EXIT_FAILURE);
         }else{
            printf("Could not connect to server: Connection timeout\n");
            exit(EXIT_FAILURE);
         }
      } while (1);
  }

  //Socket Status wieder auf blockierend setzen
  if( (arg = fcntl(sock, F_GETFL, NULL)) < 0) {
    perror("Could not get socket status");
    exit(EXIT_FAILURE);
  }
  arg &= (~O_NONBLOCK);
  if( fcntl(sock, F_SETFL, arg) < 0) {
    perror("Could not set socket status to blocking");
    exit(EXIT_FAILURE);
  }

  return sock;
}
