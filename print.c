#include "print.h"

int print(){

    char** test = malloc(sizeof(char)*96);
    int x = 0;
    int y = 0;
    test[0]="W@E5";
    test[1]="b@A4";
    
    int feld[8][8]={0};

    for(int a=0; a<2; a++){

      char* buchstabe = substring(test[a], strlen(test[a])-2, strlen(test[a])-1);
      char* figur = substring(test[a], strlen(test[a])-4, strlen(test[a])-3);
      int fall = 0;

      if(*buchstabe == 'A'){
       x = 0;
      }else if(*buchstabe == 'B'){
       x = 1;
      }else if(*buchstabe == 'C'){
       x = 2;
      }else if(*buchstabe == 'D'){
       x = 3;
      }else if(*buchstabe == 'E'){
        x = 4;
      }else if(*buchstabe == 'F'){
        x = 5;
      }else if(*buchstabe == 'G'){
        x = 6;
      }else if(*buchstabe == 'H'){
        x = 7;
      }
    
      y = atoi(substring(test[a], strlen(test[a])-1, strlen(test[a])))-1;

      if(*figur =='w'){
       fall = 1;
      } else if(*figur == 'b'){
       fall = 2;
      } else if(*figur == 'W'){
       fall = 3;
      } else if(*figur == 'B'){
       fall = 4;
      }


      feld[x][y]=fall;
      fall=0;
    }

    printf("\n");
    printf("    A B C D E F G H\n");
    printf("  +-----------------+\n");

    for(int i=0; i<8; i++){
      printf(" %i|", 8-i);
      for(int j=0; j<8; j++){                    //j entspricht Buchstabe von Feld
        if((feld[j][7-i]==0)&&((i%2==0&&j%2==0)||(i%2==1&&j%2==1))){
          printf(" .");
        }else if((feld[j][7-i]==0)&&((i%2==1&&j%2==0)||(i%2==0&&j%2==1))){
          printf(" -");
        }else if(feld[j][7-i]==1){
          printf(" w");
        }else if(feld[j][7-i]==2){
          printf(" b");
        }else if(feld[j][7-i]==3){
          printf(" W");
        }else if(feld[j][7-i]==3){
          printf(" B");
        }
      }
      printf(" |%i\n", 8-i);
    }

    printf("  +-----------------+\n");
    printf("    A B C D E F G H\n");
    printf("\n");


free(test);
return 0;
}
