#ifndef performCon                                                              //Wenn das Makro noch nicht existiert
#define performCon                                                              //Definiere das Makro

//Bibliotheken einbinden
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/select.h>

//Module einbinden
#include "responseHandler.h"
#include "processRequest.h"
#include "parameter.h"

//Variablen deklarieren
int end = 1;                                                                  //Variable in der gespeichert wird ob der Server das Ende des Prologs (+ ENDPLAYERS) gesendet hat
fd_set readfds;           //Set der Filedeskriptoren die gelesen werden sollen
struct timeval tv;        //Struct für die Zeitslots, in denen überwacht wird
int retval;               //return value der Select Methode


//Funktionen deklarieren
void performConnection(int sock, int fd);                                               //Handelt den Prolog mit dem Server ab

#endif
