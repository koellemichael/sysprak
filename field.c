#include "field.h"

/**
 *Prüft ob ein Stein an der Stelle (i,j) weiß ist.
 *@param i Zeilenindex
 *@param i Spaltenindex
 *@return 1 wenn weiß, 0 wenn schwarz, -1 sonst
 */
int isWhite(int i, int j){
  char piece = serverinfo->field[i][j][strlen(serverinfo->field[i][j])-1];
  if(piece =='w' || piece =='W'){
    return 1;
  }else if(piece =='b' || piece =='B'){
    return 0;
  }else{
    return -1;
  }
}

/**
 *Prüft ob ein Stein an der Stelle (i,j) schwarz ist.
 *@param i Zeilenindex
 *@param i Spaltenindex
 *@return 0 wenn weiß, 1 wenn schwarz, -1 sonst
 */
int isBlack(int i, int j){
    char piece = serverinfo->field[i][j][strlen(serverinfo->field[i][j])-1];
    if(piece =='w' || piece =='W'){
      return 0;
    }else if(piece =='b' || piece =='B'){
      return 1;
    }else{
      return -1;
    }
}

/**
 *Prüft ob ein Stein an der Stelle (i,j) ist.
 *@param i Zeilenindex
 *@param i Spaltenindex
 *@return 1 Feld frei, 0 wenn besetzt
 */
int isFieldEmpty(int i, int j){
  if((!strcmp(serverinfo->field[i][j],"")){
    return 1;
  } else {
    return 0;
  }
}

/**
 *Prüft ob ein Stein an der Stelle (i,j) ist eine Dame ist.
 *@param i Zeilenindex
 *@param i Spaltenindex
 *@return 1 wenn Dame, 0 wenn keine Dame, -1 sonst
 */
int isDraught(int i, int j){
  char piece = serverinfo->field[i][j][strlen(serverinfo->field[i][j])-1];
    if(piece =='w' || piece =='b'){
      return 0;
    }else if(piece =='W' || piece =='b'){
      return 1;
    }else{
      return -1;
    }
}

/**
 * Rechnet die A-H Indizes in Zahlen um.
 */
int columntoint(char column){
  switch (column){
    case 65: return 0;
    case 66: return 1;
    case 67: return 2;
    case 68: return 3;
    case 69: return 4;
    case 70: return 5;
    case 71: return 6;
    case 72: return 7;
    default: return -1;
  }
}

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
/*
  char w[] = " \u26C0";
  char W[] = " \u26C1";
  char b[] = " \u26C2";
  char B[] = " \u26C3";
  char bField[] = " \u25FC";
  char wField[] = " \u25FB";
*/
  char w[] = " w";
  char W[] = " W";
  char b[] = " b";
  char B[] = " B";
  char bField[] = " -";
  char wField[] = " .";

  strcat(out, "\n");
  strcat(out,"    A B C D E F G H\n");                                                                //Spielfeld Beschriftung oben
  strcat(out,"  +-----------------+\n");
  for(int i=0; i<ROWS; i++){
    sprintf(buf," %i|", 8-i);                                                                         //Spielfeld-Nummerierung links
    strcat(out,buf);
    for(int j = 0; j<COLUMNS; j++){
      if((!strcmp(serverinfo->field[i][j],""))&&((i%2==0&&j%2==0)||(i%2==1&&j%2==1))){                //Spielfeld-Quadrate als Punkte
        strcat(out,wField);
      }else if((!strcmp(serverinfo->field[i][j],""))&&((i%2==1&&j%2==0)||(i%2==0&&j%2==1))){          //Spielfeld-Quadrate als Striche
        strcat(out,bField);
      }else{
        if(serverinfo->field[i][j][strlen(serverinfo->field[i][j])-1] =='w'){                         //Weißer Stein wenn 'w' im Array
          strcat(out, w);
        }else if(serverinfo->field[i][j][strlen(serverinfo->field[i][j])-1] =='W'){                   //Weiße Dame wenn 'W' im Array
          strcat(out, W);
        }else if(serverinfo->field[i][j][strlen(serverinfo->field[i][j])-1] =='b'){                   //Schwarzer Stein wenn 'b' im Array
          strcat(out, b);
        }else if(serverinfo->field[i][j][strlen(serverinfo->field[i][j])-1] =='B'){                   //Schwarze Dame wenn 'B' im Array
          strcat(out, B);
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
      if((serverinfo->field[a][b][strlen(serverinfo->field[a][b])-1]=='w')||(serverinfo->field[a][b][strlen(serverinfo->field[a][b])-1]=='W')){   //Letzer Char vor @ ein 'w'
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

  printf("%s",out);                                                                                 //Ausgabe des gesamten Strings

}
