#include "Duino9341.h"
int x1 = 120,y1 = 160,x2 = 239,y2 = 0;

void setup(){
  LcdInit();
  LcdFill(0,0,239,319,RGB(255,255,255));
}

int color = RGB(0,0,0)-1000;

void loop(){
  LcdDrawLine(x1,y1,x2,y2,color);
  if(x2 == 239 && y2 == 0){
    color += 1000;
    y2 += 1;
  }
  else if(y2 >= 0 && y2 <  319 && x2 == 239){
    y2 += 1;
  }
  else if(x2 >  0 && x2 <= 239 && y2 == 319){
    x2-= 1;
  }
  else if(y2 >  0 && y2 <= 319 && x2 == 0){
    y2 -= 1; 
  }
  else if(x2 >= 0 && x2 <  239 && y2 == 0){
    x2 += 1;
  }
}
