#pragma once

#define LCD_D0   8
#define LCD_D1   9
#define LCD_D2   2
#define LCD_D3   3
#define LCD_D4   4
#define LCD_D5   5
#define LCD_D6   6
#define LCD_D7   7
#define LCD_RD   A0
#define LCD_WR   A1
#define LCD_RS   A2
#define LCD_CS   A3
#define LCD_RST  A4
#define fastDigitalWriteHIGH(Pin) *(portOutputRegister(digitalPinToPort(Pin)))|=digitalPinToBitMask(Pin)  //Faster digitalWrite(Pin,HIGH);
#define fastDigitalWriteLOW(Pin) *(portOutputRegister(digitalPinToPort(Pin)))&=~digitalPinToBitMask(Pin)  //Faster digitalWrite(Pin,LOW);
#define RGB(r,g,b) ((b&31)+((g&63)<<5)+((r&31)<<11))

void LcdWriteCommand(unsigned char d){
  //Write Command Mode On
  fastDigitalWriteLOW(LCD_RS);
  //Write Datas to LCD_D0 to LCD_D7
  PORTD = (PORTD & B00000011) | ((d) & B11111100); 
  PORTB = (PORTB & B11111100) | ((d) & B00000011); 
  //Enable Datas
  fastDigitalWriteLOW(LCD_WR);
  fastDigitalWriteHIGH(LCD_WR);
}

void LcdWriteData(unsigned char d){
  //Write Command Mode On
  fastDigitalWriteHIGH(LCD_RS);
  //Write Datas to LCD_D0 to LCD_D7
  PORTD = (PORTD & B00000011) | ((d) & B11111100);
  PORTB = (PORTB & B11111100) | ((d) & B00000011);
  //Enable Datas
  fastDigitalWriteLOW(LCD_WR);
  fastDigitalWriteHIGH(LCD_WR);
}

void LcdInit(void){
  //Initialize Data Pins
  pinMode(LCD_D0,OUTPUT);
  pinMode(LCD_D1,OUTPUT);
  pinMode(LCD_D2,OUTPUT);
  pinMode(LCD_D3,OUTPUT);
  pinMode(LCD_D4,OUTPUT);
  pinMode(LCD_D5,OUTPUT);
  pinMode(LCD_D6,OUTPUT);
  pinMode(LCD_D7,OUTPUT);

  //Initialize Command Pins
  pinMode(LCD_RD,OUTPUT);
  pinMode(LCD_WR,OUTPUT);
  pinMode(LCD_RS,OUTPUT);
  pinMode(LCD_CS,OUTPUT);
  pinMode(LCD_RST,OUTPUT);
  digitalWrite(LCD_CS,LOW);
  digitalWrite(LCD_RD,HIGH);
  digitalWrite(LCD_WR,LOW);

  //Reset
  digitalWrite(LCD_RST,HIGH);
  delay(5);
  digitalWrite(LCD_RST,LOW);
  delay(15);
  digitalWrite(LCD_RST,HIGH);
  delay(15);

  LcdWriteCommand(0xCB);
  LcdWriteData(0x39);
  LcdWriteData(0x2C);
  LcdWriteData(0x00);
  LcdWriteData(0x34);
  LcdWriteData(0x02);

  LcdWriteCommand(0xCF);
  LcdWriteData(0x00);
  LcdWriteData(0XC1);
  LcdWriteData(0X30);

  LcdWriteCommand(0xE8);
  LcdWriteData(0x85);
  LcdWriteData(0x00);
  LcdWriteData(0x78);

  LcdWriteCommand(0xEA);
  LcdWriteData(0x00);
  LcdWriteData(0x00);
 
  LcdWriteCommand(0xED);
  LcdWriteData(0x64);
  LcdWriteData(0x03);
  LcdWriteData(0X12);
  LcdWriteData(0X81);

  LcdWriteCommand(0xF7);
  LcdWriteData(0x20);
  
  LcdWriteCommand(0xC0);    //Power control 
  LcdWriteData(0x23);   //VRH[5:0] 
 
  LcdWriteCommand(0xC1);    //Power control 
  LcdWriteData(0x10);   //SAP[2:0];BT[3:0] 

  LcdWriteCommand(0xC5);    //VCM control 
  LcdWriteData(0x3e);   //Contrast
  LcdWriteData(0x28); 
 
  LcdWriteCommand(0xC7);    //VCM control2 
  LcdWriteData(0x86);   //--
 
  LcdWriteCommand(0x36);    // Memory Access Control 
  LcdWriteData(0x48);   

  LcdWriteCommand(0x3A);
  LcdWriteData(0x55);

  LcdWriteCommand(0xB1);
  LcdWriteData(0x00);
  LcdWriteData(0x18);
 
  LcdWriteCommand(0xB6);    // Display Function Control 
  LcdWriteData(0x08);
  LcdWriteData(0x82);
  LcdWriteData(0x27);

  LcdWriteCommand(0x11);    //Exit Sleep 
  delay(120);
        
  LcdWriteCommand(0x29);    //Display on 
  LcdWriteCommand(0x2c);
}

void LcdOpenWindow(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2){
  LcdWriteCommand(0x2a);
  LcdWriteData(x1>>8);
  LcdWriteData(x1);
  LcdWriteData(x2>>8);
  LcdWriteData(x2);
  LcdWriteCommand(0x2b);
  LcdWriteData(y1>>8);
  LcdWriteData(y1);
  LcdWriteData(y2>>8);
  LcdWriteData(y2);
  LcdWriteCommand(0x2c);
}

void LcdFill(int x,int y,int width,int height,unsigned int color)
{
  LcdOpenWindow(x,y,x+width-1,y+height-1);
  for(int i=y;i<y+height;i++){
    for(int j=x;j<x+width;j++){
      LcdWriteData(color>>8);
      LcdWriteData(color);
    }
  }
}

void LcdSetPixel(int x, int y, unsigned int color){
  LcdOpenWindow(x,y,x,y);
  LcdWriteData(color>>8);
  LcdWriteData(color);
}

void LcdDrawLine(int x1, int y1, int x2, int y2, unsigned int color){
  float dx = x2 - x1;
  float dy = y2 - y1;
  int steps = abs(y2-y1);
  if(fabs(dx) > fabs(dy)){
    steps = abs(x2-x1);
  }
  float tmpx = x1;
  float tmpy = y1;
  float xadd = dx/steps;
  float yadd = dy/steps;
  for(int i = 0; i < steps; i++){
    LcdSetPixel(tmpx, tmpy, color);
    tmpx += xadd;
    tmpy += yadd;
  }
}
