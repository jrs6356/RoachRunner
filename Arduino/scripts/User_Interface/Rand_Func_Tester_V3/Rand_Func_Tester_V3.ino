#include "RoachRunner_Digipot.h"

byte inc_H = 2;
byte cs_pot_H = 4;
byte ud_H = 3;
byte inc_L = 9;
byte cs_pot_L = 11;
byte ud_L = 10;

void setup() {
  Serial.begin(115200);
  while(!Serial){}
  delay(100);
  Digipot dpotH(cs_pot_H, inc_H, ud_H);
  Digipot dpotL(cs_pot_L, inc_L, ud_L);
  dpotH.reset(50,0,0,5);
  dpotL.reset(51,0,0,5);

}

void loop() {
  // put your main code here, to run repeatedly:

}
