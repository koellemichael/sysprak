#ifndef konfigDatei                                                     //Wenn das Makro noch nicht existiert
#define konfigDatei                                                     //Makro definieren

//Bibliotheken einbinden
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


//Struct definieren
typedef struct{
    char *hostName;
    int portNumber;
    char *gameKindName;
}configparams;

//Funktionen definieren
char* readConfiguration(char *configFile, char *paramName);
char* findParamValue(char *string, char *delimiter, char **substring);

#endif
