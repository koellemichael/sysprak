#include "printField.h"

/**
 * Rechnet die Źahlen in A-H Indizes um.
 */
char inttocolumn(int col){
  switch (col){
    case 0: return 'A';
    case 1: return 'B';
    case 2: return 'C';
    case 3: return 'D';
    case 4: return 'E';
    case 5: return 'F';
    case 6: return 'G';
    case 7: return 'H';
    default: return 'x';
  }
}

/**
 *Gibt das aktuelle Spielfeld mit Informationen aus dem Shared Memory aus.
 *Ein kompletter String um Unterbrechungen zu vermeiden.
 */

void printfield(void){
  char out[BUFFERLENGTH];
  char buf[BUFFERLENGTH];

  strcat(out, "\n");
  strcat(out,"    A B C D E F G H\n");                                                                //Spielfeld Beschriftung oben
  strcat(out,"  +-----------------+\n");
  for(int i=0; i<ROWS; i++){
    sprintf(buf," %i|", 8-i);                                                                         //Spielfeld-Nummerierung links
    strcat(out,buf);
    for(int j = 0; j<COLUMNS; j++){
      if((!strcmp(serverinfo->field[i][j],""))&&((i%2==0&&j%2==0)||(i%2==1&&j%2==1))){                //Spielfeld-Quadrate als Punkte
        strcat(out," .");
      }else if((!strcmp(serverinfo->field[i][j],""))&&((i%2==1&&j%2==0)||(i%2==0&&j%2==1))){          //Spielfeld-Quadrate als Striche
        strcat(out," -");
      }else{
        if(serverinfo->field[i][j][strlen(serverinfo->field[i][j])-1] =='w'){                         //Weißer Stein wenn 'w' im Array
          strcat(out, " \u26C0");
        }else if(serverinfo->field[i][j][strlen(serverinfo->field[i][j])-1] =='W'){                   //Weiße Dame wenn 'W' im Array
          strcat(out, " \u26C1");
        }else if(serverinfo->field[i][j][strlen(serverinfo->field[i][j])-1] =='b'){                   //Schwarzer Stein wenn 'b' im Array
          strcat(out, " \u26C2");
        }else if(serverinfo->field[i][j][strlen(serverinfo->field[i][j])-1] =='B'){                   //Schwarze Dame wenn 'B' im Array
          strcat(out, " \u26C3");
        }
      }

    }
    sprintf(buf," |%i\n", 8-i);                                                                       //Spielfeld-Nummerierung rechts
    strcat(out,buf);
  }
	strcat(out,"  +-----------------+\n");
  strcat(out,"    A B C D E F G H\n");                                                                //Spielfeld Beschriftung unten
  strcat(out,"\n");

  strcat(out,"White Towers\n");                                                                       //Liste der Türme mit weißem Stein oben
  strcat(out,"============\n");
  for (int a = ROWS-1; a >= 0; a--){
    for (int b = 0; b < COLUMNS; b++){
      if((serverinfo->field[a][b][strlen(serverinfo->field[a][b])-1]=='w')||(serverinfo->field[a][b][strlen(serverinfo->field[a][b])-1]=='w')){   //Letzer Char vor @ ein 'w'
        sprintf(buf,"%c%i: %s\n", inttocolumn(b), 8-a, serverinfo->field[a][b]);                      //Ausgabe z.B. E5: w
        strcat(out,buf);
      }
    }
  }
  strcat(out,"\n");

  strcat(out,"Black Towers\n");                                                                       //Liste der Türme mit schwarzem Stein oben, analog zu weiß
  strcat(out,"============\n");
  for (int a = ROWS-1; a >= 0; a--){
    for (int b = 0; b < COLUMNS; b++){
      if((serverinfo->field[a][b][strlen(serverinfo->field[a][b])-1]=='b')||(serverinfo->field[a][b][strlen(serverinfo->field[a][b])-1]=='B')){
        sprintf(buf,"%c%i: %s\n", inttocolumn(b), 8-a, serverinfo->field[a][b]);
        strcat(out,buf);
      }
    }
  }
  strcat(out,"\n");

  printf("%s",out);                                                                                   //Ausgabe des gesamten Strings
}
