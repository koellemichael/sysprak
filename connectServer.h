#ifndef connectSrv                                                              //Wenn das Makro noch nicht existiert
#define connectSrv                                                              //Definiere das Makro

//Bibliotheken einbinden
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <sys/time.h>
#include <errno.h>
#include <fcntl.h>

//Konstanten
#define TIMEOUT 3

//Funktionen deklarieren
int connectServer(int, char*);                                                  //Verbindet Client mit Server

#endif
