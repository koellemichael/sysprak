#include "config.h"

/**
 *@param configFile Die Konfigurations Datei als Pfad
 * Diese Funktion liest aus der Konfigurations Datei die nötigen Parameterwerte aus und legt sie im Struct configparams cp ab
*/

void readConfiguration(char *configFile){
    
   FILE *file = NULL;                                                           //Die gewählte Konfig Datei vom Typ FILE
   char string[200];                                                            //Zeilenweise Strings, die aus der Konfig Datei gelesen werden
   configparams cp;                                                             //Eine Instanz vom Struct configparams namens "cp"
   char *delimiter = "= ";                                                      //Tokens, die die Strings in verschiedene Abschnitte teilen
   char *substring = NULL;                                                      //Substrings, die wir rauslesen wollen
   int port = 0;                                                                //Portnummer mit 0 initialisiert
   char *paramNameHost = "hostName";                                            //Parametername des Hostnamens, nachdem wir filtern
   char *paramNamePort = "portNumber";                                          //Parametername der Portnummer, nach der wir filtern
   char *paramNameGame = "gameKindName";                                        //Parametername des Spiels, nach dem wir filtern

    
    printf("%s\n", configFile);             
    file = fopen(configFile, "r");                                              //Öffnen der Konfigdatei
    if(file != NULL){                                                           //Abfrage, ob sich die Datei öffnen lässt
        printf("Successfully opened the config file \"%s\". \n",configFile);    
        
        while(fgets(string,200,file)) {                                         //Einzelne zeilenweise Abfrage der Datei
            
            if(strstr(string, paramNameHost) != NULL){                          //Filtern nach der Zeile, die den "Hostname" enthält
                printf("%s\n",paramNameHost);
                findParamValue(string, delimiter, &substring);                  //Funktionsaufruf, der den zugehörigen ParameterWert zurückgibt
                cp.hostName = malloc(strlen(substring) * sizeof(char));         //Zuweisung von Speicherpatz an die Struct Variable cp.HostName
                strcpy(cp.hostName, substring);                                 //Zuweisung des richtigen Parameterwerts zur Variable cp.HostName
                printf("%s\n", cp.hostName);
                
            }
             if(strstr(string, paramNamePort) != NULL){                         //Filtern nach der Zeile, die die Portnummer enthält
                printf("%s\n",paramNamePort);       
                findParamValue(string, delimiter, &substring);                  //Funktionsuaufruf, der den zugehörigen Parameterwert zurückgibt
                port = atoi(substring);                                         //Umwandeln zu Integer
                cp.portNumber = port;                                           //Zuweisung der Portnummer zur Struct Variable
                printf("%i\n", cp.portNumber);
                 
            }
             if(strstr(string, paramNameGame) != NULL){                         //Filtern nach der Zeile, die GameName enthält
                printf("%s\n",paramNameGame);
                findParamValue(string, delimiter, &substring);                  //Funktionsauruf, der ParameterWert zurückgibt
                cp.gameKindName = malloc(strlen(substring) * sizeof(char));     //Zuweisung Speicherplatz zu Struct Variable
                strcpy(cp.gameKindName, substring);                             //Zuweisung von Parameterwert zur Struct Variable
                printf("%s\n", cp.gameKindName);
            } 
    
        }
    }
    else{                                                                       //Configfile konnte nicht geöffnet werden 
        perror("Could not open the specified config file. Please make sure the file exists in the specified directory. \n");
    }
    
    if(cp.hostName == NULL){                                                    //Fehler bei Zuweisung hostname
        perror("Error: No hostname could be assigned to struct configparams cp.\n");
    }
    if(cp.portNumber == 0) {                                                    //Fehler bei Zuweisung portnumber
         perror("Error: No portnumber could be assigned to struct configparams cp.\n");
    }
    if(cp.gameKindName == NULL){                                                //Fehler bei zuweisung gamename
         perror("Error: No gamekind could be assigned to struct configparams cp.\n");
    } 
    
    
    fclose (file);                                                              //Schliessen der Konfig Datei
}


/**
 *@Param string als char* ist der zu lesende String
 *@Param delimiter als char* ist das Token, nachdem der String in Abschnitte geteilt wird
 *@Param substring als **char ist der Substring, der aus dem String gesucht wird
 *Der Pointer zerteilt nach dem Token den String in Abschnitte und gibt den letzten Abschnitt (den Parameterwert) zurück
*/

void findParamValue(char *string, char *delimiter, char **substring){
    char* ptr;                                                                  //Pointer
    
    ptr = strtok(string, delimiter);                                            //Zeigt auf das erste Token
        while(ptr != NULL){                                                     //Während der Pointer noch nicht am Ende ist
            *substring = ptr;                                                   //Der Substring ist jeweils der nächste Abschnitt
                                                                                //Bis er letztendlich der letzte Abschnitt ist
            ptr = strtok(NULL, delimiter);                                      //Der Pointer geht eins weiter
        }
    return;
}
