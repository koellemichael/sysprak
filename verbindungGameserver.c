#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>



/**
 * Diese Datei soll den Clienten mit dem Gameserver verbinden. Dazu wird ein Socket implementiert.
 * 
 */


//main wird noch umbenannt, wenn es mit den anderen Dateien verbunden wird. Nur zum Test.
int main(){
    //Variablen deklarieren
    
    //Diese hier wird noch geändert zu Lauras Version
    const char hostname[] = "sysprak.priv.lab.nm.ifi.lmu.de";
    int sock;
    char ip[100];
    struct hostent *he;
    
    //SOCKET ANLEGEN 
    
    /* PF_INET: Internet Protokoll Vesion 4
     * SOCK_STREAM: Typ des Sockets.(Gegensatz: SOCK_DGRAM). Stream Zuverlässiger.
     * 0: Default Transportprotokoll des Sockettyps. Von SOCK_STREAM ist dies TCP.
     */
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        perror("Konnte Socket nicht erzeugen\n");
    }
    
    
    //HOSTNAME in IP Adresse umwandeln
    if( (he = gethostbyname(hostname)) == NULL){
        exit(1);
    }
    
    
    struct sockaddr_in server;
    server.sin_family = PF_INET;
    server.sin_port = htons(1357);
    memcpy(&server.sin_addr.s_addr, he->h_addr_list[0], he -> h_length);
    
    
    //CONNECT 
    if(connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0){
        perror("connect fehlgeschlagen");
        return 1; 
    } else {
        printf("Mit dem Server erfolgreich verbunden.");
    }
    
    //Übergabe Filedeskriptor an Funktion performConnection()
    performConnection(sock);
    
    close(sock);
    return 0;
    
  
}
