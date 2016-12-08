#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "processRequest.h"
#include "format.h"

int pro = 1; 

char *format(char *input){

  char *out = malloc(256*sizeof(char));  

    if(pro==1 && match(input, "MNM Gameserver .+accepting connections")){
      strcpy(out, "The MNM Gameserver ");
      strcat(out, substring(input, 14,strlen(input)-21));
      strcat(out, "accepted the connection.");                                               
      pro++;             


    }else if(pro==2 && match(input, "Client version accepted - please send Game-ID to join")){
      strcpy(out,"The client version was accepted. Now please send Game-ID to join the game.");                                                
      pro++;                                                                 
    


    }else if(pro==3 && match(input, "PLAYING .+")){
      strcpy(out, "We are playing the game ");
      strcat(out, substring(input, 8,strlen(input)));                                                       
      pro++;   
    
        

    }else if(pro==4 && match(input, ".+")){
          strcpy(out, "The game's name is ");
          strcat(out, input);                                               
          pro++; 
    
                                                                          
    }else if(pro==5 && match(input, "YOU .+ .+")){
          strcpy(out, substring(input, 6,strlen(input)));
          strcat(out, " you are player number ");
          strcat(out, substring(input, 4, 5));                                                          
          pro++;
    
                                                                           
    }else if(pro==6 && match(input, "TOTAL .+")){
          strcpy(out, substring(input,6,strlen(input)));
          strcat(out, " player will take part in this game.");                                                        
          pro++;  
    
    
    }else if(match(input, "ENDPLAYERS")){
         strcpy(out, "ENDPLAYERS - The prolog is finished!");
    
    
    }else if(pro>=7 && match(input, ".+ .+ .+")){
          strcpy(out, substring(input, 2,strlen(input)-2)); 
          strcat(out," (");                                                   
          strcat(out, substring(input, 0,1));
          strcat(out,") is ");
          

          if(atoi(substring(input, strlen(input)-2, strlen(input)-1))==1){
            strcat(out, "ready");
          }
          else if(atoi(substring(input, strlen(input)-2, strlen(input)-1))==0){
            strcat(out, "not ready");
          }

          pro++;  


    }else{                                                                      
      strcpy(out,"Unknown request");                                       
    }



    if(input!=NULL){                                                         
      strcat(out,"\n");                                                   
    }
    return out;                                                            
}

