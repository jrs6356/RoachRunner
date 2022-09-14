#include "RoachRunner_LCD.h"
#include <SPI.h>
#include "Digipot.h"

LCD::LCD(int _cs_lcd, int _cs_pot, int _rs, int _inc, int _ud){
  
  cs = _cs_lcd;
  rs = _rs;

  dpot.cs = _cs_pot;
  dpot.inc = _inc;
  dpot.ud = _ud;

  //Buffers
  buf = 0x00;
  ch = 0x00;
  addr = 0x00;
  addr_com = 0x00;
  len = 0;

  //Command Variables
  ID  = INCR;
  SH  = SHIFT_DIS;
  D   = D_ON;
  C   = C_OFF;
  B   = B_OFF;
  SC  = SC_OFF;
  RL  = RIGHT;
  DL  = DL_8;
  N   = N_2;
  F   = F_8;

  //Command Variables Combined
  fcn_set = ((DL | N) | F) & fcn;
  disp_set_1 = ((D_OFF | C_OFF) | B_OFF) & disp;
  ems_set = (ID | SH) & ems;
  disp_set_2 = ((D | C) | B) & disp;
  crsr_set = (SC | RL) & crsr;

  //Counters, Switch States, and Variables
  j = 1;
  con_init = 30;
  row_length = 20;
  dt = 10;
  row = 0;
  col = 0;

}

LCD::LCD(int _cs_lcd, int _cs_pot, int _rs, int _inc, int _ud, int _upLED, int _downLED){
  
  cs = _cs_lcd;
  rs = _rs;

  dpot.cs = _cs_pot;
  dpot.inc = _inc;
  dpot.ud = _ud;
  dpot.upLED = _upLED;
  dpot.downLED = _downLED;

  //Buffers
  buf = 0x00;
  ch = 0x00;
  addr = 0x00;
  addr_com = 0x00;
  len = 0;

  //Command Variables
  ID  = INCR;
  SH  = SHIFT_DIS;
  D   = D_ON;
  C   = C_OFF;
  B   = B_OFF;
  SC  = SC_OFF;
  RL  = RIGHT;
  DL  = DL_8;
  N   = N_2;
  F   = F_8;

  //Command Variables Combined
  fcn_set = ((DL | N) | F) & fcn;
  disp_set_1 = ((D_OFF | C_OFF) | B_OFF) & disp;
  ems_set = (ID | SH) & ems;
  disp_set_2 = ((D | C) | B) & disp;
  crsr_set = (SC | RL) & crsr;

  //Counters, Switch States, and Variables
  j = 1;
  con_init = 30;
  row_length = 20;
  dt = 10;
  row = 0;
  col = 0;

}

void LCD::init(){
  pinMode(cs,OUTPUT);
  pinMode(rs,OUTPUT);

  digitalWrite(cs, HIGH);
  digitalWrite(rs, LOW);

  dpot.init();

  dpot.reset(con_init,0,0,5);
  brightness = 100 - dpot.MI;

  delay(100);
  
  for(int i=0;i<3;++i){
    command(fcn_set);
    command(disp_set_1);
    clear();
    command(ems_set);
    command(disp_set_2);
    home();
  }
  makeExtraChars();
  Serial.println("LCD Initialized");
}

void LCD::contrast(int cont){
  if(brightness<cont){
    for(int i=brightness;i<cont;++i){
      dpot.decr(1,0);
    }
  }
  else if(brightness>cont){
    for(int i=brightness;i>cont;--i){
      dpot.incr(1,0);
    }
  }
  brightness = 100 - dpot.MI;
}

void LCD::clear(){
  command(clr);
  delay(12);
}

void LCD::home(){
  command(rtn);
  delay(12);
}

void LCD::command(byte val){
  digitalWrite(cs, LOW);
  digitalWrite(rs, LOW);

  SPI.transfer(val);

  digitalWrite(cs, HIGH);
  delay(3);
}

void LCD::setCursor(bool csr, bool blnk){
  if(csr){
    C = C_ON;
  }else{
    C = C_OFF;
  }
  if(blnk){
    B = B_ON;
  }else{
    B = B_OFF;
  }
  addr = ((D | C) | B) & disp;
  command(addr);
}

void LCD::writeData(unsigned char val){
  digitalWrite(cs, LOW);
  digitalWrite(rs, HIGH);

  SPI.transfer(val);

  digitalWrite(cs, HIGH);
  delay(1);
}

void LCD::writeRow(String txt, int r){
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
  }
}

void LCD::writeChar(char c, int r, int col){
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

void LCD::print(String txt){
  len = txt.length();
  for(int i=0; i<len; ++i){
    writeData(txt[i]);
  }
}

void LCD::scroll(String txt, int r, int c, int y, int x){
  switch(y){
    case 0:
      addr = row0[x];
      break;
    case 1:
      addr = row1[x];
      break;
    case 2:
      addr = row2[x];
      break;
    case 3:
      addr = row3[x];
      break;
    default:
      addr = row0[x];
      break;
  }
  command(addr);
  for(int i=r;i<r+c;++i){
    writeData(txt[i]);
  }
}

void LCD::makeExtraChars(){
  if(!symbolLib){
    symbolLib = true;
    loadingLib = false;
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
                          {1, 0, 1, 0, 1},    //46
                          {0, 0, 0, 0, 0},    //47
                          {0, 0, 0, 0, 0},    //48-----------------
                          {0, 0, 0, 0, 0},    //49
                          {0, 0, 1, 0, 0},    //50
                          {0, 1, 0, 1, 0},    //51
                          {1, 0, 0, 0, 1},    //52    7
                          {0, 0, 1, 0, 0},    //53
                          {0, 1, 0, 1, 0},    //54
                          {1, 0, 0, 0, 1},    //55
                          {0, 0, 0, 0, 0},    //56-----------------
                          {0, 0, 0, 0, 0},    //57
                          {1, 0, 0, 0, 1},    //58
                          {0, 1, 0, 1, 0},    //59
                          {0, 0, 1, 0, 0},    //60    8
                          {1, 0, 0, 0, 1},    //61
                          {0, 1, 0, 1, 0},    //62
                          {0, 0, 1, 0, 0},    //63
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
}

void LCD::loadingBar(){
  if(!loadingLib){
    loadingLib = true;
    symbolLib = false;
    int xChars[64][5] = { {0, 0, 0, 0, 0},    //1
                          {1, 1, 1, 1, 1},    //2
                          {0, 0, 0, 0, 0},    //3
                          {0, 0, 0, 0, 0},    //4     1
                          {0, 0, 0, 0, 0},    //5
                          {0, 0, 0, 0, 0},    //6
                          {1, 1, 1, 1, 1},    //7
                          {0, 0, 0, 0, 0},    //8
                          {0, 0, 0, 0, 0},    //1
                          {1, 1, 1, 1, 1},    //2
                          {1, 0, 0, 0, 0},    //3
                          {1, 0, 0, 0, 0},    //4     2
                          {1, 0, 0, 0, 0},    //5
                          {1, 0, 0, 0, 0},    //6
                          {1, 1, 1, 1, 1},    //7
                          {0, 0, 0, 0, 0},    //8
                          {0, 0, 0, 0, 0},    //1
                          {1, 1, 1, 1, 1},    //2
                          {1, 1, 0, 0, 0},    //3
                          {1, 1, 0, 0, 0},    //4     3
                          {1, 1, 0, 0, 0},    //5
                          {1, 1, 0, 0, 0},    //6
                          {1, 1, 1, 1, 1},    //7
                          {0, 0, 0, 0, 0},    //8
                          {0, 0, 0, 0, 0},    //1
                          {1, 1, 1, 1, 1},    //2
                          {1, 1, 1, 0, 0},    //3
                          {1, 1, 1, 0, 0},    //4     4
                          {1, 1, 1, 0, 0},    //5
                          {1, 1, 1, 0, 0},    //6
                          {1, 1, 1, 1, 1},    //7
                          {0, 0, 0, 0, 0},    //8
                          {0, 0, 0, 0, 0},    //1
                          {1, 1, 1, 1, 1},    //2
                          {1, 1, 1, 1, 0},    //3
                          {1, 1, 1, 1, 0},    //4     5
                          {1, 1, 1, 1, 0},    //5
                          {1, 1, 1, 1, 0},    //6
                          {1, 1, 1, 1, 1},    //7
                          {0, 0, 0, 0, 0},    //8
                          {0, 0, 0, 0, 0},    //1
                          {1, 1, 1, 1, 1},    //2
                          {1, 1, 1, 1, 1},    //3
                          {1, 1, 1, 1, 1},    //4     6
                          {1, 1, 1, 1, 1},    //5
                          {1, 1, 1, 1, 1},    //6
                          {1, 1, 1, 1, 1},    //7
                          {0, 0, 0, 0, 0},    //8
                          {0, 0, 0, 0, 0},    //1
                          {1, 1, 1, 1, 1},    //2
                          {1, 1, 0, 0, 1},    //3
                          {1, 1, 0, 0, 1},    //4     7
                          {1, 1, 0, 0, 1},    //5
                          {1, 1, 0, 0, 1},    //6
                          {1, 1, 1, 1, 1},    //7
                          {0, 0, 0, 0, 0},    //8
                          {0, 0, 0, 0, 0},    //1
                          {1, 1, 1, 1, 1},    //2
                          {1, 1, 1, 0, 1},    //3
                          {1, 1, 1, 0, 1},    //4     8
                          {1, 1, 1, 0, 1},    //5
                          {1, 1, 1, 0, 1},    //6
                          {1, 1, 1, 1, 1},    //7
                          {0, 0, 0, 0, 0}};   //8

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
}