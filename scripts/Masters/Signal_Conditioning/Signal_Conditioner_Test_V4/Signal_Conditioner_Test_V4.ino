#include <SPI.h>
#include "RoachRunner_Digipot.h"
#include "RoachRunner_CS5532.h"

byte cs = 39;
byte osc = 7;
byte sdoFlag = 45;

byte shunt1 = 27;
byte shunt2 = 33;

//byte cs2 = 2;
//byte osc2 = 3;
//byte sdoFlag2 = 4;

//byte inc_H = 2;
//byte cs_pot_H = 4;
//byte ud_H = 3;
//byte inc_L = 9;
//byte cs_pot_L = 11;
//byte ud_L = 10;
//int d8[8];
//int d16[16];

int fosc = 2e6;

int k = 0;
int m = 0;
int n = 0;
double t1 = 0;
double t2 = 0;
double dt = 0;
float f = 0;

CS5532 adc1("Q1Z",cs,sdoFlag,osc);

//Digipot dpotH;
//Digipot dpotL;
//CS5532 adc2("Q2Z",cs2,sdoFlag2);

void setup() {
  Serial.begin(115200);
  while(!Serial){}
  //Serial.println("Serial Initialized...");
  delay(100);

  pinMode(shunt1, OUTPUT);
  pinMode(shunt2, OUTPUT);

  digitalWrite(shunt1, LOW);
  digitalWrite(shunt2, LOW);

  //digitalWrite(shunt1, HIGH);
  //digitalWrite(shunt2, HIGH);

  //while(1);
  /*
  dpotH.cs = cs_pot_H;
  dpotH.inc = inc_H;
  dpotH.ud = ud_H;
  dpotL.cs = cs_pot_L;
  dpotL.inc = inc_L;
  dpotL.ud = ud_L;
  dpotL.init();
  dpotH.init();
  */

  //Offset Calibration
  //dpotH.reset(50,0,0,5);
  //dpotL.reset(51,0,0,5);

 // uint32_t o = 0xFFD94300;
  
  delay(5000);
  adc1.init();
  adc1.convReg.Vref = 2.5;//4.471;
  adc1.convReg.Va = 5.0;
  adc1.sysOffCal(Setup1);
  //adc1.uploadOffset(Offset1,o);

  //Gain Calibration
  //dpotH.reset(99,0,0,5);
  //dpotL.reset(1,0,0,5);
  delay(10000);
  adc1.sysGainCal(Setup2);

  //Single Measurement
  m = 30;
  n = 70;
  //dpotH.reset(m,0,0,5);
  //dpotL.reset(n,0,0,5);
  delay(5000);
  adc1.singMeas(Setup3);
  Serial.print("\nMeas 1:\t");
  Serial.println(adc1.convReg.measDec);
  Serial.print(adc1.convReg.measV,6);
  Serial.println(" V");
  //adc1.singMeas(Setup4);
  //Serial.print("\nMeas 2:\t");
  //Serial.println(adc1.convReg.measDec);
  //Serial.print(adc1.convReg.measV,6);

  //while(1){}
  
  adc1.contMeas(Setup3);
  t1 = micros();
}

void loop() {
  ++k;
  //Serial.print(1000.0*adc1.scanMeas(0),6);
  sciNote(adc1.scanMeas(0)/10.0,6);
  t2 = micros();
  dt = t2 - t1;
  f = pow(10,6)*float(k)/dt;
  Serial.print("\t");
  Serial.print(adc1.convReg.measDec);
  Serial.print("\t");
  Serial.print(k);
  Serial.print("\t");
  Serial.print(f);
  Serial.print("\tH: ");
  Serial.print(m);
  Serial.print("\tL: ");
  Serial.print(n);
  Serial.print("\tOF: ");
  Serial.println(adc1.convReg.of);
  /*
  if(k==400){
    k=0;
    m = random(1,99);
    n = random(1,99);
    //digitalWrite(shunt2, HIGH);
    //dpotH.reset(m,0,0,5);
    //dpotL.reset(n,0,0,5);
    delay(1000);
    t1 = micros();
  }
  */
}

void sciNot(float x,int d){
  float a;
  int b = 0;
  if(x<1.0){
    a = 0.0;
    while(a<1.0){
      ++b;
      a = x * float(pow(10,b));
    }
    Serial.print("x = ");
    Serial.print(a,d);
    Serial.print(" E-");
    Serial.println(b);
  }
  else{
    a = 100.0;
    while(a>10){
      ++b;
      a = x * float(pow(10,-b));
    }
    Serial.print("x = ");
    Serial.print(a,d);
    Serial.print(" E");
    Serial.println(b);
  }
}

/*
void read8adc(int val){
  int d8[8];
  for(int i=0;i<8;++i){
    adc1.d8[i] = 0;
  }
  for(int i=0;i<8;++i){
    if(val>=pow(2,7-i)){
      adc1.d8[i] = 1;
      val = val - pow(2,7-i);
    }
    Serial.print(adc1.d8[i]);
  }
}
*/
void read8(int val){
  int d8[8];
  for(int i=0;i<8;++i){
    d8[i] = 0;
  }
  for(int i=0;i<8;++i){
    if(val>=pow(2,7-i)){
      d8[i] = 1;
      val = val - pow(2,7-i);
    }
    Serial.print(d8[i]);
  }
}

void read16(int val){
  int d16[16];
  for(int i=0;i<16;++i){
    d16[i] = 0;
  }
  for(int i=0;i<16;++i){
    if(val>=pow(2,15-i)){
      d16[i] = 1;
      val = val - pow(2,15-i);
    }
    Serial.print(d16[i]);
  }
}

/*
void read32(long val){
  int d32[32];
  for(int i=0;i<32;++i){
    adc1.d32[i] = 0;
  }
  for(int i=0;i<32;++i){
    if(val>=pow(2,31-i)){
      adc1.d32[i] = 1;
      val = val - pow(2,31-i);
    }
    Serial.print(adc1.d32[i]);
  }
}
*/
