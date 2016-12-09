#ifndef performCon                                                              //Wenn das Makro noch nicht existiert
#define performCon                                                              //Definiere das Makro

//Bibliotheken einbinden
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

//Module einbinden
#include "responseHandler.h"
#include "processRequest.h"
#include "parameter.h"


//Funktionen deklarieren
void performConnection(int *fd);                                                //Handelt den Prolog mit dem Server ab

#endif
