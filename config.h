#ifndef konfigDatei
#define konfigDatei

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

typedef struct{
    char *hostName;
    int portNumber;
    char *gameKindName;
}configparams;


void readConfiguration(char *configFile);
void findParamValue(char *string, char *delimiter, char **substring);

#endif
