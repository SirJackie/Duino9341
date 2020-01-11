// Breakout/Arduino UNO pin usage:
// LCD Data Bit :   7   6   5   4   3   2   1   0
// Uno dig. pin :   7   6   5   4   3   2   9   8
// Uno port/pin : PD7 PD6 PD5 PD4 PD3 PD2 PB1 PB0
// Mega dig. pin:  29  28  27  26  25  24  23  22
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
#define LCD_REST A4
#define fastDigitalWriteHIGH(Pin) *(portOutputRegister(digitalPinToPort(Pin)))|=digitalPinToBitMask(Pin)  //Faster digitalWrite(Pin,HIGH);
#define fastDigitalWriteLOW(Pin) *(portOutputRegister(digitalPinToPort(Pin)))&=~digitalPinToBitMask(Pin)  //Faster digitalWrite(Pin,LOW);

void LCD_W_BUS(unsigned char d){
  PORTD = d;
  PORTB = d;
  fastDigitalWriteLOW(LCD_WR);
  fastDigitalWriteHIGH(LCD_WR);
}


void LCD_W_COM(unsigned char VH){
  fastDigitalWriteLOW(LCD_RS);
  LCD_W_BUS(VH);
}

void LCD_W_DATA(unsigned char VH){
  fastDigitalWriteHIGH(LCD_RS);
  LCD_W_BUS(VH);
}

void LCD_INIT(void){
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
  pinMode(LCD_REST,OUTPUT);
  digitalWrite(LCD_RD, HIGH);

  //Reset
  digitalWrite(LCD_REST,HIGH);
  delay(5);
  digitalWrite(LCD_REST,LOW);
  delay(15);
  digitalWrite(LCD_REST,HIGH);
  delay(15);

  LCD_W_COM(0xCB);
  LCD_W_DATA(0x39);
  LCD_W_DATA(0x2C);
  LCD_W_DATA(0x00);
  LCD_W_DATA(0x34);
  LCD_W_DATA(0x02);

  LCD_W_COM(0xCF);
  LCD_W_DATA(0x00);
  LCD_W_DATA(0XC1);
  LCD_W_DATA(0X30);

  LCD_W_COM(0xE8);
  LCD_W_DATA(0x85);
  LCD_W_DATA(0x00);
  LCD_W_DATA(0x78);

  LCD_W_COM(0xEA);
  LCD_W_DATA(0x00);
  LCD_W_DATA(0x00);
 
  LCD_W_COM(0xED);
  LCD_W_DATA(0x64);
  LCD_W_DATA(0x03);
  LCD_W_DATA(0X12);
  LCD_W_DATA(0X81);

  LCD_W_COM(0xF7);
  LCD_W_DATA(0x20);
  
  LCD_W_COM(0xC0);    //Power control 
  LCD_W_DATA(0x23);   //VRH[5:0] 
 
  LCD_W_COM(0xC1);    //Power control 
  LCD_W_DATA(0x10);   //SAP[2:0];BT[3:0] 

  LCD_W_COM(0xC5);    //VCM control 
  LCD_W_DATA(0x3e);   //Contrast
  LCD_W_DATA(0x28); 
 
  LCD_W_COM(0xC7);    //VCM control2 
  LCD_W_DATA(0x86);   //--
 
  LCD_W_COM(0x36);    // Memory Access Control 
  LCD_W_DATA(0x48);   

  LCD_W_COM(0x3A);
  LCD_W_DATA(0x55);

  LCD_W_COM(0xB1);
  LCD_W_DATA(0x00);
  LCD_W_DATA(0x18);
 
  LCD_W_COM(0xB6);    // Display Function Control 
  LCD_W_DATA(0x08);
  LCD_W_DATA(0x82);
  LCD_W_DATA(0x27);

  LCD_W_COM(0x11);    //Exit Sleep 
  delay(120);
        
  LCD_W_COM(0x29);    //Display on 
  LCD_W_COM(0x2c);
}

void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2){
  LCD_W_COM(0x2a);
  LCD_W_DATA(x1>>8);
  LCD_W_DATA(x1);
  LCD_W_DATA(x2>>8);
  LCD_W_DATA(x2);
  LCD_W_COM(0x2b);
  LCD_W_DATA(y1>>8);
  LCD_W_DATA(y1);
  LCD_W_DATA(y2>>8);
  LCD_W_DATA(y2);
  LCD_W_COM(0x2c);
}

void LCD_FILL(int x,int y,int width,int height,unsigned int color)
{
  digitalWrite(LCD_CS,LOW);//StartWriting
  Address_set(x,y,x+width-1,y+height-1);
  for(int i=y;i<y+height;i++){
    for(int j=x;j<x+width;j++){
      LCD_W_DATA(color>>8);
      LCD_W_DATA(color);
    }
  }
  digitalWrite(LCD_CS,HIGH);//EndWriting
}

inline unsigned int RGB(unsigned short r,unsigned short g,unsigned short b){
  return (b + (g << 5) + (r << 11));
}

void setup(){
  LCD_INIT();
  LCD_FILL(0,0,239,319,RGB(31,63,31));
  LCD_FILL(10,10,100,100,RGB(31,0,0));
  LCD_FILL(20,20,110,110,RGB(0,63,0));
  LCD_FILL(30,30,120,120,RGB(0,0,31));
  digitalWrite(LCD_CS,LOW);//StartWriting
  LCD_W_COM(0x33);
  //TFA
  LCD_W_DATA(5>>8);
  LCD_W_DATA(5);
  //VSA
  LCD_W_DATA(150>>8);
  LCD_W_DATA(150);
  //BFA
  LCD_W_DATA(5>>8);
  LCD_W_DATA(5);
}

int vsp = 0;

void loop(){
  LCD_W_COM(0x37);
  //VSP
  LCD_W_DATA(vsp>>8);
  LCD_W_DATA(vsp);
  vsp++;
  delay(10);
}
