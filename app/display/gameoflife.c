#include "esp_common.h"
#include "math.h"

#include "gameoflife.h"

// Ported from https://github.com/kodera2t/GoL/blob/master/UltraZERO_GoL.ino

bool new_pix[128][64]; //new pixels
bool current_pix[128][64]; //current pixels
int counter=0;

void draw_pixel(){
  int xpos,ypos;
  int i, j;
  for(i=0;i<128;i++){
    for(j=0;j<64;j++){
      //xpos=i+32;
      //ypos=j+32;
      xpos=i;
      ypos=j;
      if(current_pix[i][j]==1){
        //display.drawPixel(xpos,ypos,WHITE);
        oled_drawpixel(xpos, ypos, 1);
      }  
  }  
}
  //display.display();
}

void delete_pixel(){
  int xpos,ypos;
  int i, j;
  for(i=0;i<128;i++){
    for(j=0;j<64;j++){
      //xpos=i+32;
      //ypos=j+32;
      xpos=i;
      ypos=j;
      if(current_pix[i][j]==1){
        //display.drawPixel(xpos,ypos,BLACK);
        oled_drawpixel(xpos, ypos, 1);
      }  
  }  
}
}


void renewal(){
  int i, j;
   for(i=0;i<128;i++){ 
     for(j=0;j<64;j++){
       current_pix[i][j]=new_pix[i][j];
     }
   }
}

void born_or_dead(){
 int live_num;
 int i, j, k;
 for(i=1;i<128;i++){ //checking point is (i,j)
     //live_num=0;
   for(j=1;j<64;j++){
     live_num=0;
     
     for(k=-1;k<2;k++){
       
       if(current_pix[i-1][j+k]==1){
         live_num++;
       }
       
       if(current_pix[i+1][j+k]==1){
         live_num++;
       } 
       
     }
     
     
     
     if(current_pix[i][j+1]==1){
         live_num++;  
     }
     if(current_pix[i][j-1]==1){
       live_num++;
     }
     
///////GAME RULE//////////////   
     if(current_pix[i][j]==0){
        if(live_num==3){
              new_pix[i][j]=1;
        }
     }else{
     switch(live_num){
     case 0:
       new_pix[i][j]=0;
       break;
     case 1:  
       new_pix[i][j]=0;
         //     goto hoge;
       break;
     case 2:
       new_pix[i][j]=1;
         //     goto hoge;
       break;
     case 3:
       new_pix[i][j]=1;
        //      goto hoge;
       break;
     case 4:
       new_pix[i][j]=0;
        //      goto hoge;
       break;
     case 5:
       new_pix[i][j]=0;
        //      goto hoge;
       break;
     case 6:
       new_pix[i][j]=0;
        //      goto hoge;
       break;
     case 7:
       new_pix[i][j]=0;
        //      goto hoge;
       break;
     case 8:
       new_pix[i][j]=0;
        //      goto hoge;
       break;
     default:
      break;
     }

   }   
 }
}
}

void reset_randompixel(){
    //int j=8;
  int i, j;
  for(i=2;i<127;i++){   //initial pixel 
    for(j=2;j<63;j++){
      uint8_t pix=(uint8_t)os_random();
      if(pix>100){
      current_pix[i][j]=pix;
      new_pix[i][j]=pix;
      }
    }    
  }
}

void gameoflife_init(void) {
  int i, j;
  for(i=0;i<128;i++){ // initialize all pixels are off
    for(j=0;j<64;j++){
      new_pix[i][j]=0;
      current_pix[i][j]=0;
    }    
  } 

  reset_randompixel();
  draw_pixel();
}

void gameoflife_render(void) {
  counter++;
  born_or_dead();
  delete_pixel();
  renewal();
  draw_pixel();
  if(counter==750){ //force reset on 300th generation
    reset_randompixel();
    counter=0;
  }
}