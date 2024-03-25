#include <SPI.h>

//Pin Assignments
const byte inc = 2;
const byte cs_pot = 3;
const byte ud = 4;
const byte up = 5;
const byte down = 6;
const byte ledUp = 7;
const byte ledDown = 8;
const byte sw = 9;
const byte a2 = 10;
const byte b2 = 11;
const byte cs_lcd = 38;
const byte rs_lcd = 39;
const int v0 = A0;

//Command Headers
#define clr    0x01  //0b00000001;   // Clear Display
#define rtn    0x02  //0b00000010;   // Return Home
#define ems    0x07  //0b00000111;   // Entry Mode Set
#define disp   0x0f  //b00001111;    // Display On/Off
#define crsr   0x1c  //0b00011100;   // Cursor or Display Shift
#define fcn    0x3c  //0b00111100;   // Function Set
#define cgram  0x7f  //0b01111111;   // Set CGRAM Address
#define ddram  0xff  //0b11111111;   // Set DDRAM Address
#define data   0xff  //0b11111111;   // Write Data to RAM

//Command Options
#define INCR       0x06  //0b10;     // Increment by 1
#define DECR       0x04  //0b00;     // Decrement by 1
#define SHIFT_EN   0x05  //0b1;      // Enable Full Display Shift
#define SHIFT_DIS  0x04  //0b0;      // Disable Full Display Shift
#define D_ON       0x0c  //0b100;    // Display On
#define D_OFF      0x08  //0b000;    // Display Off
#define C_ON       0x0a  //0b10;     // Cursor On
#define C_OFF      0x08  //0b00;     // Cursor Off
#define B_ON       0x09  //0b1;      // Blink On
#define B_OFF      0x08  //0b0;      // Blink Off
#define SC_ON      0x18  //0b1000;   // Set Cursor Control On
#define SC_OFF     0x10  //0b0000;   // Set Cursor Control Off
#define RIGHT      0x14  //0b100;    // Set Shift Direction Right
#define LEFT       0x10  //0b000;    // Set Shift Direction Left
#define DL_8       0x30  //0b10000;  // Data Length 8-Bit
#define DL_4       0x20  //0b00000;  // Data Length 4-Bit
#define N_2        0x28  //0b1000;   // Numbers of Display Line = 2, (1/16 Duty)
#define N_1        0x20  //0b0000;   // Numbers of Display Line = 1, (1/8 or 1/11 Duty)
#define F_11       0x24  //0b100;    // Font: 5x11
#define F_8        0x20  //0b000;    // Font: 5x8

//Command Variables
uint8_t ID  = INCR;
uint8_t SH  = SHIFT_DIS;
uint8_t D   = D_ON;
uint8_t C   = C_OFF;
uint8_t B   = B_OFF;
uint8_t SC  = SC_OFF;
uint8_t RL  = RIGHT;
uint8_t DL  = DL_8;
uint8_t N   = N_2;
uint8_t F   = F_8;

//Command Variables Combined
uint8_t fcn_set = ((DL | N) | F) & fcn;
uint8_t disp_set_1 = ((D_OFF | C_OFF) | B_OFF) & disp;
uint8_t ems_set = (ID | SH) & ems;
uint8_t disp_set_2 = ((D | C) | B) & disp;
uint8_t crsr_set = (SC | RL) & crsr;      // 00010100

//Character Map
uint8_t row0[20] = {0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93};
uint8_t row1[20] = {0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3};
uint8_t row2[20] = {0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7};
uint8_t row3[20] = {0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF, 0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7};
int row = 1;
int col = 10;

//Buffers
uint8_t buf = 0x00;
uint8_t ch = 0x00;
uint8_t addr = 0x00;
uint8_t addr_com = 0x00;
int len = 0;
char blrb[18] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};

//Sample Text
String quotes[] = {"Back","Speed","Orientation","Vibration","Force","Random"};

//Counters, Switch States, and Variables
volatile bool bounceUp = false;
volatile bool bounceDown = false;
volatile bool bounce = false;
volatile bool scroll = false;
volatile int dir = 0;
volatile int a_state = HIGH;
volatile int b_state = LOW;
int k = 1;
int j = 1;
int sel = 1;
float V0_b = 0.0;
float V0 = 0.0;
float res = 4096.0;
float vref = 3.3;
int dt = 10;
int lcd_con_init = 30;
int row_length = 20;

void setup() {
  Serial.begin(115200);
  while(!Serial){}
  SPI.begin();
  SPI.beginTransaction(SPISettings((210000),MSBFIRST,SPI_MODE3));
  analogReadResolution(12);

  pinMode(cs_lcd,OUTPUT);
  pinMode(rs_lcd,OUTPUT);
  pinMode(inc, OUTPUT);
  pinMode(cs_pot, OUTPUT);
  pinMode(ud, OUTPUT);
  pinMode(up, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(ledUp, OUTPUT);
  pinMode(ledDown, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(up), up_btn, RISING);
  attachInterrupt(digitalPinToInterrupt(down), down_btn, RISING);
  
  digitalWrite(cs_lcd, HIGH);
  digitalWrite(rs_lcd, LOW);
  digitalWrite(cs_pot, HIGH);
  digitalWrite(inc, HIGH);
  digitalWrite(ud, LOW);
  digitalWrite(ledUp, LOW);
  digitalWrite(ledDown, LOW);
  
  delay(100);
  init2();
  init2();
  init2();
  resetPot(lcd_con_init,1,1);
  makeExtraChars();
  lcdWriteChar(255,row,col);
  
}

void loop() {
  if(scroll){
    col = col+dir;
    lcdWriteChar(255,row,col);
    scroll = !scroll;
  }

  delay(50);

}

void command(byte val){
  digitalWrite(cs_lcd, LOW);
  digitalWrite(rs_lcd, LOW);

  SPI.transfer(val);

  digitalWrite(cs_lcd, HIGH);
  delay(10);
}

void writeData(unsigned char val){
  digitalWrite(cs_lcd, LOW);
  digitalWrite(rs_lcd, HIGH);

  SPI.transfer(val);

  digitalWrite(cs_lcd, HIGH);
  delay(50);
}

void init2(){
  command(fcn_set);
  command(disp_set_1);
  command(clr);
  delay(5);
  command(ems_set);
  command(disp_set_2);
  command(rtn);
}

void clearDisp(){
  command(clr);
  delay(15);
}

void lcdWriteRow(String txt, int r){
  len = txt.length();
  if(len>18){
    j = 17;
  }
  else{
    j = len;
  }
  switch(r){
    case 0:
      addr = row0[0];
      break;
    case 1:
      addr = row1[0];
      break;
    case 2:
      addr = row2[0];
      break;
    case 3:
      addr = row3[0];
      break;
    default:
      addr = row0[0];
      break;
  }
  command(addr);
  writeData(' ');
  for(int i=0;i<j;++i){
    blrb[i] = txt[i];
  }
  for(int i=0;i<j;++i){
    writeData(blrb[i]);
    delay(100);
  }
}

void lcdWriteChar(char c, int r, int col){
  if(col>19){
    Serial.println("Column must be between 0 and 19");
  }
  else{
    switch(r){
      case 0:
        addr = row0[col];
        break;
      case 1:
        addr = row1[col];
        break;
      case 2:
        addr = row2[col];
        break;
      case 3:
        addr = row3[col];
        break;
      default:
        addr = row0[col];
        break;
    }
    command(addr);
    writeData(c);
  }
}

void lcdPrint(String txt){
  len = txt.length();
  for(int i=0; i<len; ++i){
    row = floor(i/20);
    col = i%20;
    switch(row){
      case 0:
        addr = row0[col];
        break;
      case 1:
        //command(crsr_set);
        addr = row1[col];
        break;
      case 2:
        addr = row2[col];
        break;
      case 3:
        addr = row3[col];
        break;
      default:
        addr = row0[0];
        break;
    }
    addr_com = (addr | 0x80) & ddram;
    command(addr);
    writeData(txt[i]);
  }
}

void byteWrite(int val){
  int d[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  for(int i=0; i<8; ++i){
    if(val>=pow(2,7-i)){
      d[i] = 1;
      val = val - pow(2,7-i);
    }
    Serial.print(d[i]);
  }
}

void btnStep(){
  if(bounceUp){
    potIncr(15,1);
    bounceUp = false;
    delay(300);
  }
  else if(bounceDown){
    potDecr(15,1);
    bounceDown = false;
    delay(300);
  }
}

void potIncr(int dur, bool r){
  if(k<99){
    ++k;
  }
  digitalWrite(ledUp, HIGH);
  digitalWrite(cs_pot, LOW);
  delay(1);
  digitalWrite(ud, HIGH);
  delay(1);
  digitalWrite(inc, LOW);
  delay(1);
  digitalWrite(inc, HIGH);
  delay(1);
  digitalWrite(cs_pot, HIGH);
  delay(dur*dt);
  digitalWrite(ledUp, LOW);
  delay(dur*dt);
  if(r){
    Serial.println("-------------------------");
    Serial.println("Up");
    Serial.print("k = ");Serial.println(k);
  }
}

void potDecr(int dur, bool r){
  if(k>0){
      --k;
  }
  digitalWrite(ledDown, HIGH);
  digitalWrite(cs_pot, LOW);
  delay(1);
  digitalWrite(ud, LOW);
  delay(1);
  digitalWrite(inc, LOW);
  delay(1);
  digitalWrite(inc, HIGH);
  delay(1);
  digitalWrite(cs_pot, HIGH);
  delay(dur*dt);
  digitalWrite(ledDown, LOW);
  delay(dur*dt);
  if(r){
    Serial.println("-------------------------");
    Serial.println("Down");
    Serial.print("k = ");Serial.println(k);
  }
}

void resetPot(int MI1, bool a, bool b){
  for(int i=0;i<110;++i){
    potDecr(1,a);
  }
  for(int i=0;i<MI1;++i){
    potIncr(1,b);
  }
}

void encA(){
  scroll = true;
  a_state = digitalRead(a2);
  b_state = digitalRead(b2);
  if(a_state==HIGH){
    if(b_state==HIGH){
      dir = -1;
    }
    else{
      dir = 1;
    }
  }
  else{
    if(b_state==HIGH){
      dir = 1;
    }
    else{
      dir = -1;
    }
  }
}

void encB(){
  scroll = true;
  a_state = digitalRead(a2);
  b_state = digitalRead(b2);
  if(b_state==HIGH){
    if(a_state==HIGH){
      dir = 1;
    }
    else{
      dir = -1;
    }
  }
  else{
    if(a_state==HIGH){
      dir = -1;
    }
    else{
      dir = 1;
    }
  }
}

void up_btn(){
  if(!bounceUp){
    bounceUp = true;
  }
}

void down_btn(){
  if(!bounceDown){
    bounceDown = true;
  }
}

void ensw(){
  if(!bounce){
    bounce = true;
  }
}

void peepeepoopoo(){
  command((0x00 | 0x40) & cgram);
  writeData(0b00000100);
  command((0x01 | 0x40) & cgram);
  writeData(0b00001010);
  command((0x02 | 0x40) & cgram);
  writeData(0b00001110);
  command((0x03 | 0x40) & cgram);
  writeData(0b00001010);
  command((0x04 | 0x40) & cgram);
  writeData(0b00001010);
  command((0x05 | 0x40) & cgram);
  writeData(0b00010101);
  command((0x06 | 0x40) & cgram);
  writeData(0b00001010);
  command((0x07 | 0x40) & cgram);
  writeData(0b00000000);

  command((0x08 | 0x40) & cgram);
  writeData(0b00000000);
  command((0x09 | 0x40) & cgram);
  writeData(0b00000010);
  command((0x0a | 0x40) & cgram);
  writeData(0b00000100);
  command((0x0b | 0x40) & cgram);
  writeData(0b00001010);
  command((0x0c | 0x40) & cgram);
  writeData(0b00001110);
  command((0x0d | 0x40) & cgram);
  writeData(0b00010001);
  command((0x0e | 0x40) & cgram);
  writeData(0b00011111);
  command((0x0f | 0x40) & cgram);
  writeData(0b00000000);

  //command(row0[13]);
  //writeData(0x00);
  //command(row0[15]);
  //writeData(0x01);
  command(clr);
  delay(2000);
  command(rtn);
  writeData('p');
  writeData('e');
  writeData('e');
  writeData('p');
  writeData('e');
  writeData('e');
  writeData(' ');
  writeData(0x00);
  writeData(' ');
  writeData('p');
  writeData('o');
  writeData('o');
  writeData('p');
  writeData('o');
  writeData('o');
  writeData(' ');
  writeData(0x01);
  writeData(' ');
  delay(1000);
  writeData(';');
  writeData(')');
}

void makeExtraChars(){
  int xChars[64][5] = { {0, 1, 1, 1, 0},    //1
                        {1, 0, 1, 0, 1},    //2
                        {1, 0, 1, 0, 1},    //3
                        {1, 0, 1, 0, 1},    //4     1
                        {1, 0, 0, 0, 1},    //5
                        {1, 0, 0, 0, 1},    //6
                        {0, 1, 1, 1, 0},    //7
                        {0, 0, 0, 0, 0},    //8------------------
                        {0, 1, 1, 1, 0},    //9
                        {1, 0, 1, 0, 1},    //10
                        {1, 0, 1, 0, 1},    //11
                        {1, 0, 1, 1, 1},    //12    2
                        {1, 0, 0, 0, 1},    //13
                        {1, 0, 0, 0, 1},    //14
                        {0, 1, 1, 1, 0},    //15
                        {0, 0, 0, 0, 0},    //16-----------------
                        {0, 1, 1, 1, 0},    //17
                        {1, 0, 1, 0, 1},    //18
                        {1, 0, 1, 0, 1},    //19
                        {1, 0, 1, 0, 1},    //20    3
                        {1, 0, 1, 0, 1},    //21
                        {1, 0, 0, 0, 1},    //22
                        {0, 1, 1, 1, 0},    //23
                        {0, 0, 0, 0, 0},    //24-----------------
                        {0, 1, 1, 1, 0},    //25
                        {1, 0, 1, 0, 1},    //26
                        {1, 0, 1, 0, 1},    //27
                        {1, 1, 1, 0, 1},    //28    4
                        {1, 0, 0, 0, 1},    //29
                        {1, 0, 0, 0, 1},    //30
                        {0, 1, 1, 1, 0},    //31
                        {0, 0, 0, 0, 0},    //32-----------------
                        {0, 0, 0, 0, 0},    //33
                        {0, 0, 1, 0, 0},    //34
                        {0, 1, 1, 1, 0},    //35
                        {1, 1, 1, 1, 1},    //36    5
                        {0, 0, 1, 0, 0},    //37
                        {1, 1, 1, 0, 0},    //38
                        {0, 0, 0, 0, 0},    //39
                        {0, 0, 0, 0, 0},    //40-----------------
                        {0, 0, 0, 0, 0},    //41
                        {0, 0, 0, 0, 0},    //42
                        {0, 0, 0, 0, 0},    //43
                        {0, 0, 0, 0, 0},    //44    6
                        {0, 0, 0, 0, 0},    //45
                        {0, 0, 0, 0, 0},    //46
                        {0, 0, 0, 0, 0},    //47
                        {0, 0, 0, 0, 0},    //48-----------------
                        {0, 0, 0, 0, 0},    //49
                        {0, 0, 0, 1, 0},    //50
                        {0, 0, 1, 0, 0},    //51
                        {0, 1, 0, 1, 0},    //52    7
                        {1, 1, 1, 1, 1},    //53
                        {1, 0, 0, 0, 1},    //54
                        {1, 1, 1, 1, 1},    //55
                        {0, 0, 0, 0, 0},    //56-----------------
                        {0, 0, 1, 0, 0},    //57
                        {0, 1, 0, 1, 0},    //58
                        {0, 1, 1, 1, 0},    //59
                        {0, 1, 0, 1, 0},    //60    8
                        {0, 1, 0, 1, 0},    //61
                        {1, 0, 1, 0, 1},    //62
                        {0, 1, 0, 1, 0},    //63
                        {0, 0, 0, 0, 0}};   //64-----------------
  for(int i=0;i<64;++i){
    buf = 0x00;
    for(int c=0;c<5;++c){
      buf = (xChars[i][4-c] << c) | buf;
    }
    addr = (i | 0x40) & cgram;
    command(addr);
    writeData(buf);
  }
}
