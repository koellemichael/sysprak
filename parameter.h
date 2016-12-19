#ifndef para                                                                    //Wenn das Makro noch nicht existiert
#define para                                                                    //Definiere das Makro

//Konstanten definieren
#define GAMEKINDNAME "Bashni"                                                   //Konstante GAMEKINDNAME definieren
#define PORTNUMBER 1357                                                         //Konstante PORTNUMBER definieren
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"                               //Konstante HOSTNAME definieren
#define BUFFERLENGTH 256                                                        //Konstante für die Puffergröße definieren
#define VERSION "2.3"                                                           //TODO provisorische Konstante

void printLogo(void){
  printf("/////////////////////////////////////////\n");
  printf("/////////////////////////////////////////\n");
  printf("///   ____            _           _   ///\n");
  printf("///  | __ )  __ _ ___| |__  _ __ (_)  ///\n");
  printf("///  |  _ \\ / _` / __| '_ \\| '_ \\| |  ///\n");
  printf("///  | |_) | (_| \\__ \\ | | | | | | |  ///\n");
  printf("///  |____/ \\__,_|___/_| |_|_| |_|_|  ///\n");
  printf("///                                   ///\n");
  printf("/////////////////////////////////////////\n");
  printf("///            Gruppe 20              ///\n");
  printf("///  Laura Haller, Katharina Winter   ///\n");
  printf("///         Michael Koelle            ///\n");
  printf("/////////////////////////////////////////\n\n");
}

#endif
