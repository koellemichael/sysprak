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
    server.sin_addr.s_addr = get_hostname_ip("sysprak.priv.lab.nm.ifi.lmu.de", ip);
;
    
    
    //CONNECT 
    if(connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0){
        perror("connect fehlgeschlagen");
        return 1; 
    }
    
    close(sock);
    return 0;
    
  
}


int  get_hostname_ip(char *hostname, char *ip){
    struct hostent *he;
    
    if((he = gethostbyname("sysprak.priv.lab.nm.ifi.lmu.de")) == NULL){
        perror("gethostbyname fehlgeschlagen");
        return 1;
    } else {
        unsigned int i = 0;
        printf("%s", he->h_name);
        while(he -> h_addr_list[i] != NULL){
            printf("%s", inet_ntoa( *(struct in_addr*)(he -> h_addr_list[i])));
            i++;
        }
        return 0;
    }
    
    return 1;
    
}