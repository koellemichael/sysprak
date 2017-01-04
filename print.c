#include "print.h"


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

int printfield(){

  printf("\n");
  printf("    A B C D E F G H\n");
  printf("  +-----------------+\n");

  for(int i=0; i<ROWS; i++){
    printf(" %i|", 8-i);
    for(int j = 0; j<COLUMNS; j++){ 
      if((!strcmp(serverinfo->field[i][j],""))&&((i%2==0&&j%2==0)||(i%2==1&&j%2==1))){
        printf(" .");
      }else if((!strcmp(serverinfo->field[i][j],""))&&((i%2==1&&j%2==0)||(i%2==0&&j%2==1))){
        printf(" -");
      }else{
        printf(" %c",serverinfo->field[i][j][strlen(serverinfo->field[i][j])-1]);
      }
    
    }
    printf(" |%i\n", 8-i);
  }

  printf("  +-----------------+\n");
  printf("    A B C D E F G H\n");
  printf("\n\n");

  printf("White Towers\n");
  printf("============\n");
  for (int a = ROWS; a > 0; a--)
  {
    for (int b = 0; b < COLUMNS; b++)
    {
      if((serverinfo->field[a][b][strlen(serverinfo->field[a][b])-1]=='w')||(serverinfo->field[a][b][strlen(serverinfo->field[a][b])-1]=='w')){
        printf("%c%i: %s\n", inttocolumn(b), 8-a, serverinfo->field[a][b]);
      }
    }
  }
  printf("\n\n");

  printf("Black Towers\n");
  printf("============\n");
  for (int a = ROWS; a >= 0; a--)
  {
    for (int b = 0; b < COLUMNS; b++)
    {
      if((serverinfo->field[a][b][strlen(serverinfo->field[a][b])-1]=='b')||(serverinfo->field[a][b][strlen(serverinfo->field[a][b])-1]=='B')){
        printf("%c%i: %s\n", inttocolumn(b), 8-a, serverinfo->field[a][b]);
      }
    }
  }
  printf("\n\n");

  return 1;
}


/*int printfield(){

  char* ausgabe = malloc(BUFFERLENGTH*sizeof(char));

  strcpy(ausgabe, "\n");
  strcat(ausgabe, "    A B C D E F G H\n");
  strcat(ausgabe, "  +-----------------+\n ");

  for(int i=0; i<ROWS; i++){
    strcat(ausgabe, 8-i);
    strcat("|");
    for(int j = 0; j<COLUMNS; j++){ 
      strcat(ausgabe, " ");
      if((!strcmp(serverinfo->field[i][j],""))&&((i%2==0&&j%2==0)||(i%2==1&&j%2==1))){
        strcat(ausgabe,".");
      }else if((!strcmp(serverinfo->field[i][j],""))&&((i%2==1&&j%2==0)||(i%2==0&&j%2==1))){
        strcat(ausgabe,"-");
      }else{
        strcat(ausgabe, serverinfo->field[i][j][strlen(serverinfo->field[i][j])-1]);
      }
    
    }
    strcat(ausgabe, " |");
    strcat(ausgabe, 8-i);
    strcat(ausgabe, "\n");
  }

  printf("%s", ausgabe);

  printf("  +-----------------+\n");
  printf("    A B C D E F G H\n");
  printf("\n\n");

  printf("White Towers\n");
  printf("============\n");
  for (int a = ROWS; a > 0; a--)
  {
    for (int b = 0; b < COLUMNS; b++)
    {
      if((serverinfo->field[a][b][strlen(serverinfo->field[a][b])-1]=='w')||(serverinfo->field[a][b][strlen(serverinfo->field[a][b])-1]=='w')){
        printf("%c%i: %s\n", inttocolumn(b), 8-a, serverinfo->field[a][b]);
      }
    }
  }
  printf("\n\n");

  printf("Black Towers\n");
  printf("============\n");
  for (int a = ROWS; a >= 0; a--)
  {
    for (int b = 0; b < COLUMNS; b++)
    {
      if((serverinfo->field[a][b][strlen(serverinfo->field[a][b])-1]=='b')||(serverinfo->field[a][b][strlen(serverinfo->field[a][b])-1]=='B')){
        printf("%c%i: %s\n", inttocolumn(b), 8-a, serverinfo->field[a][b]);
      }
    }
  }
  printf("\n\n");

  return 1;
}*/

