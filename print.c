#include "print.h"

int main(void){

    char* test = "w@E5";
    int x = 0;
    int y = 0;

    switch (substring(test, 3,4)) {
      case A:   x = 1;
                break;
      case B:   x = 2;
                break;
      case C:   x = 3;
                break;
      case D:   x = 4;
                break;
      case A:   x = 5;
                break;
      case B:   x = 6;
                break;
      case C:   x = 7;
                break;
      case D:   x = 8;
                break;
    }

    printf("Die Zahl ist %i\n", x);
    printf("Die zweite Zahl ist %i\n", substring(test, 4,5));

    int probe[8][8]={0};
    probe[2][2]=1;

    printf("    A B C D E F G H\n");
    printf("  +-----------------+\n");

    for(int i = 0; i<8; i++){
        printf("%i | ",8-i);
        for(int j = 0; j<8; j ++){
            if(probe[j][7-i]==0){
                printf(". ");
            }else printf("w ");
        }
        printf("|%i\n",8-i);
    }

    printf("  +-----------------+\n");
    printf("    A B C D E F G H\n");



return 0;
}

char *substring(char *string, unsigned int from, unsigned int to){
  if(from>=to || to>strlen(string)){                                            //Unsinnige Eingaben abfangen
    return NULL;                                                                //NULL zurückgeben falls
  }else{                                                                        //Falls sinbvolle Eingabe
    char *substring = malloc((to-from)*sizeof(char));                           //Speicher füresponse substring allokalisieren
    strncpy(substring,string+from,(to-from));                                   //Teilstring in substring speichern
    substring[(to-from)]=0;                                                     //Teilstringende festlegen
    return substring;                                                           //Substring zurückgeben
  }
}
