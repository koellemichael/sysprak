#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"


/**
 * Diese Datei soll den Clienten mit dem Gameserver verbinden. Dazu wird ein Socket implementiert.
 * 
 */



int get_hostname_ip(char *, char *);

int main(){
    //Variablen deklarieren
    
    int sock;
    char ip[100];
    
    //SOCKET ANLEGEN 
    
    /* PF_INET: Internet Protokoll Vesion 4
     * SOCK_STREAM: Typ des Sockets.(Gegensatz: SOCK_DGRAM). Stream Zuverlässiger.
     * 0: Default Transportprotokoll des Sockettyps. Von SOCK_STREAM ist dies TCP.
     */
    
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        perror("Konnte Socket nicht erzeugen\n");
    }
    
    
    struct sockaddr_in server;
    server.sin_family = PF_INET;
    server.sin_port = htons(1357);
    server.sin_addr.s_addr = get_hostname_ip(HOSTNAME, ip);
;
    
    
    //CONNECT 
    if(connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0){
        perror("connect fehlgeschlagen");
        return 1; 
    }
    
    close(sock);
    return 0;
    
  
}


/**
 * Diese Funktion wandelt den Hostname in eine IP-Adresse um.
 * @param hostname von verbindungGameserver als char*
 * @param ip von verbindungGameserver als char*
 * @return die IP Adresse als 
*/

int  get_hostname_ip(char *hostname, char *ip){
    struct hostent *he;
    struct in_addr ip_addr;
    
    if((he = gethostbyname(HOSTNAME)) == NULL){
        perror("gethostbyname fehlgeschlagen");
        return 1;
    } 
    
    ip_addr = *(struct in_addr*) (he ->h_addr);
    printf("Hostname: %s wurde umgewandelt zu: %s\n", HOSTNAME, inet_ntoa(ip_addr));
    
    return 0;
    
}