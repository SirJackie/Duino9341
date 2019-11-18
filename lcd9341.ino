// Breakout/Arduino UNO pin usage:
// LCD Data Bit :   7   6   5   4   3   2   1   0
// Uno dig. pin :   7   6   5   4   3   2   9   8
// Uno port/pin : PD7 PD6 PD5 PD4 PD3 PD2 PB1 PB0
// Mega dig. pin:  29  28  27  26  25  24  23  22
#define LCD_RD   A0
#define LCD_WR   A1
#define LCD_RS   A2
#define LCD_CS   A3
#define LCD_REST A4

void Lcd_Writ_Bus(unsigned char d){
  PORTD = (PORTD & B00000011) | ((d) & B11111100);
  PORTB = (PORTB & B11111100) | ((d) & B00000011);
  *(portOutputRegister(digitalPinToPort(LCD_WR))) &=  ~digitalPinToBitMask(LCD_WR);
  *(portOutputRegister(digitalPinToPort(LCD_WR)))|=  digitalPinToBitMask(LCD_WR);
}


void Lcd_Write_Com(unsigned char VH){
  *(portOutputRegister(digitalPinToPort(LCD_RS))) &=  ~digitalPinToBitMask(LCD_RS);//LCD_RS=0;
  Lcd_Writ_Bus(VH);
}

void Lcd_Write_Data(unsigned char VH){
  *(portOutputRegister(digitalPinToPort(LCD_RS)))|=  digitalPinToBitMask(LCD_RS);//LCD_RS=1;
  Lcd_Writ_Bus(VH);
}

void Lcd_Init(void){
  for(int p=0;p<10;p++)
  {
    pinMode(p,OUTPUT);
  }
  pinMode(A0,OUTPUT);
  pinMode(A1,OUTPUT);
  pinMode(A2,OUTPUT);
  pinMode(A3,OUTPUT);
  pinMode(A4,OUTPUT);
  digitalWrite(A0, HIGH);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, HIGH);
  digitalWrite(A3, HIGH);
  digitalWrite(A4, HIGH);
  
  digitalWrite(LCD_REST,HIGH);
  delay(5);
  digitalWrite(LCD_REST,LOW);
  delay(15);
  digitalWrite(LCD_REST,HIGH);
  delay(15);

  digitalWrite(LCD_CS,HIGH);
  digitalWrite(LCD_WR,HIGH);
  digitalWrite(LCD_CS,LOW);  //CS

  Lcd_Write_Com(0xCB);
  Lcd_Write_Data(0x39);
  Lcd_Write_Data(0x2C);
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0x34);
  Lcd_Write_Data(0x02);

  Lcd_Write_Com(0xCF);
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0XC1);
  Lcd_Write_Data(0X30);

  Lcd_Write_Com(0xE8);
  Lcd_Write_Data(0x85);
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0x78);

  Lcd_Write_Com(0xEA);
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0x00);
 
  Lcd_Write_Com(0xED);
  Lcd_Write_Data(0x64);
  Lcd_Write_Data(0x03);
  Lcd_Write_Data(0X12);
  Lcd_Write_Data(0X81);

  Lcd_Write_Com(0xF7);
  Lcd_Write_Data(0x20);
  
  Lcd_Write_Com(0xC0);    //Power control 
  Lcd_Write_Data(0x23);   //VRH[5:0] 
 
  Lcd_Write_Com(0xC1);    //Power control 
  Lcd_Write_Data(0x10);   //SAP[2:0];BT[3:0] 

  Lcd_Write_Com(0xC5);    //VCM control 
  Lcd_Write_Data(0x3e);   //Contrast
  Lcd_Write_Data(0x28); 
 
  Lcd_Write_Com(0xC7);    //VCM control2 
  Lcd_Write_Data(0x86);   //--
 
  Lcd_Write_Com(0x36);    // Memory Access Control 
  Lcd_Write_Data(0x48);   

  Lcd_Write_Com(0x3A);
  Lcd_Write_Data(0x55);

  Lcd_Write_Com(0xB1);
  Lcd_Write_Data(0x00);
  Lcd_Write_Data(0x18);
 
  Lcd_Write_Com(0xB6);    // Display Function Control 
  Lcd_Write_Data(0x08);
  Lcd_Write_Data(0x82);
  Lcd_Write_Data(0x27);

  Lcd_Write_Com(0x11);    //Exit Sleep 
  delay(120);
        
  Lcd_Write_Com(0x29);    //Display on 
  Lcd_Write_Com(0x2c);
}

void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2){
  Lcd_Write_Com(0x2a);
  Lcd_Write_Data(x1>>8);
  Lcd_Write_Data(x1);
  Lcd_Write_Data(x2>>8);
  Lcd_Write_Data(x2);
  Lcd_Write_Com(0x2b);
  Lcd_Write_Data(y1>>8);
  Lcd_Write_Data(y1);
  Lcd_Write_Data(y2>>8);
  Lcd_Write_Data(y2);
  Lcd_Write_Com(0x2c);
}

void LCD_Fill(int x,int y,int width,int height,unsigned int color)
{
  digitalWrite(LCD_CS,LOW);//StartWriting
  Address_set(x,y,x+width,y+height);
  for(int i=y;i<y+height;i++){
    for(int j=x;j<x+width;j++){
      Lcd_Write_Data(color>>8);
      Lcd_Write_Data(color);
    }
  }
  digitalWrite(LCD_CS,HIGH);//EndWriting
}

inline unsigned int RGB(unsigned short r,unsigned short g,unsigned short b){
  return (b + (g << 5) + (r << 11));
}

void setup(){
  Lcd_Init();
  LCD_Fill(0,0,240,320,RGB(31,63,31));

  int x = 10,y = 10,width = 100,height = 100;
  int color = RGB(0,0,0);
  int color2 = RGB(31,63,31);
  digitalWrite(LCD_CS,LOW);//StartWriting
  Address_set(x,y,x+width,y+height);
  for(int i=y;i<y+height/2;i++){
    for(int j=x;j<x+width;j++){
      Lcd_Write_Data(color>>8);
      Lcd_Write_Data(color);
    }
    for(int j=x;j<x+width;j++){
      Lcd_Write_Data(color2>>8);
      Lcd_Write_Data(color2);
    }
  }
  digitalWrite(LCD_CS,HIGH);//EndWriting
}

void loop(){
  
}
