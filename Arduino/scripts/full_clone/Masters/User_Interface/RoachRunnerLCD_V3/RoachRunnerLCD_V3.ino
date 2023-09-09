#include <SPI.h>

const byte cs_lcd = 38;
const byte rs_lcd = 39;

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
uint8_t C   = C_ON;
uint8_t B   = B_ON;
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
//uint8_t row0[20] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13};
//uint8_t row1[20] = {0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52, 0x53};
//uint8_t row2[20] = {0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27};
//uint8_t row3[20] = {0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67};

uint8_t row0[20] = {0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93};
uint8_t row1[20] = {0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3};
uint8_t row2[20] = {0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7};
uint8_t row3[20] = {0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF, 0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7};

int row = 0;
int col = 0;

//Buffers
uint8_t buf = 0x00;
uint8_t ch = 0x00;
uint8_t addr = 0x00;
uint8_t addr_com = 0x00;
int len = 0;

void setup() {
  Serial.begin(9600);
  while(!Serial){}
  SPI.begin();
  Serial.println("\n-------------------------------------");
  Serial.print("EMS:\t");byteWrite(ems_set);Serial.print("\n");
  Serial.print("DISP1:\t");byteWrite(disp_set_1);Serial.print("\n");
  Serial.print("CRSR:\t");byteWrite(crsr_set);Serial.print("\n");
  Serial.print("FCN:\t");byteWrite(fcn_set);Serial.print("\n");
  Serial.print("DISP2:\t");byteWrite(disp_set_2);Serial.print("\n");
  SPI.beginTransaction(SPISettings((210000),MSBFIRST,SPI_MODE3));
  pinMode(cs_lcd,OUTPUT);
  pinMode(rs_lcd,OUTPUT);
  digitalWrite(cs_lcd, HIGH);
  digitalWrite(rs_lcd, LOW);
  delay(100);
  //initialize();
  init2();
  init2();
  init2();
  thisOneWorks();
  command(row1[0]);
  command(disp_set_2);
  
  //writeData('A');
  //command(row2[19]);
  //command(crsr_set);
  //writeData('M');
  command(row1[3]);
  writeData('R');
  //command(row2[9]);
  //writeData('@');
  
  //delay(5000);
  //command(disp_set_1);
  //delay(5000);
  //command(((D_ON | C_OFF) | B_OFF) & disp);
  lcdPrint("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789~!@#$%^&*()-_+=[]{}|:;'<>?,./");
  //lcdOut();
}

void loop() {
  command(crsr_set);
  delay(500);

}

void thisOneWorks(){
  writeData('T');
  writeData('h');
  writeData('i');
  writeData('s');
  writeData(' ');
  writeData('r');
  writeData('o');
  writeData('w');
  writeData(' ');
  writeData('w');
  writeData('o');
  writeData('r');
  writeData('k');
  writeData('s');
  command(row2[0]);
  writeData('S');
  writeData('o');
  writeData(' ');
  writeData('d');
  writeData('o');
  writeData('e');
  writeData('s');
  writeData(' ');
  writeData('t');
  writeData('h');
  writeData('i');
  writeData('s');
  writeData(' ');
  writeData('o');
  writeData('n');
  writeData('e');
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
  delay(200);
}

void initialize(){
  command(fcn_set);
  //delay(5);
  command(disp_set_2);
  //delay(5);
  command(clr);
  delay(7);
  command(ems_set);
  //delay(5);
  command(rtn);
  //delay(5);
  
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

void lcdOut(){
  writeData(0x41);
  writeData(0x42);
}

void lcdPrint(String txt){
  Serial.print("Phrase:\t");Serial.println(txt);
  len = txt.length();
  Serial.println(len);
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
    Serial.print("Character ");
    Serial.print(i);
    Serial.print(":\t");
    Serial.print(txt[i]);
    Serial.print("\t");
    Serial.print(int(txt[i]));
    Serial.print("\t");
    byteWrite(addr);
    Serial.print("\t");
    Serial.print(addr, HEX);
    Serial.print("\tRow: ");
    Serial.print(row);
    Serial.print("\tColumn: ");
    Serial.print(col);
    Serial.print("\n");
  }
  //Serial.print("Character 1:\t");Serial.print(txt[0]);Serial.print("\t");Serial.println(int(txt[0]));
}

void byteWrite(int val){
  int d7;int d6;int d5;int d4;int d3;int d2;int d1;int d0;
  d7=d6=d5=d4=d3=d2=d1=d0=0;
  if(val>=pow(2,7)){
    d7 = 1;
    val = val - pow(2,7);
  }
  if(val>=pow(2,6)){
    d6 = 1;
    val = val - pow(2,6);
  }
  if(val>=pow(2,5)){
    d5 = 1;
    val = val - pow(2,5);
  }
  if(val>=pow(2,4)){
    d4 = 1;
    val = val - pow(2,4);
  }
  if(val>=pow(2,3)){
    d3 = 1;
    val = val - pow(2,3);
  }
  if(val>=pow(2,2)){
    d2 = 1;
    val = val - pow(2,2);
  }
  if(val>=pow(2,1)){
    d1 = 1;
    val = val - pow(2,1);
  }
  if(val>=pow(2,0)){
    d0 = 1;
    val = val - pow(2,0);
  }
  Serial.print(d7);Serial.print(d6);Serial.print(d5);Serial.print(d4);Serial.print(d3);Serial.print(d2);Serial.print(d1);Serial.print(d0);
}
