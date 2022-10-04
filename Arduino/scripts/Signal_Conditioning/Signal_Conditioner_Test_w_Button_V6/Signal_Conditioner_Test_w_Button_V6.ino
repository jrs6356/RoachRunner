#include <SPI.h>
#include "RoachRunner_Digipot.h"
#include "RoachRunner_CS5532.h"

//Pins
byte cs = 2;
byte e = 3;
byte em = 28;
byte osc = 7;
byte a0 = 8;
byte a1 = 9;
byte a2 = 10;
byte a3 = 11;
byte a0m = 27;
byte a1m = 26;
byte a2m = 25;
byte a3m = 24;
byte sdoFlag = 12;
byte btn = 22;
byte led = 23;
byte pause = 29;
byte pauseLED = 30;

// Variables
int fosc = 2e6;

int k = 0;
int m = 0;
int n = 0;
int N = 51;
const int cals = 100;
const int ele = 1000;
double offset = 0;
double t1 = 0;
double t2 = 0;
//double dt = 0;
float f = 0;
float G = 1;
double offRegVal[cals];
double offRegValAve = 0;
double gainRegVal[cals];
double gainRegValAve = 0;
uint32_t meas[ele];// = {};
byte flags[ele];
unsigned long t[ele];// = {};
char buffer[50];
volatile bool collect = false;
volatile bool paused = false;
unsigned long ti;
unsigned long tf;
unsigned long dt;

volatile bool a0_state = false;
volatile bool a1_state = false;
volatile bool a2_state = false;
volatile bool a3_state = false;
volatile bool e_state = false;

bool a0_out = false;
bool a1_out = false;
bool a2_out = false;
bool a3_out = false;
bool e_out = false;

//uint32_t meas[1200];
//uint32_t measBig[1800];
double measAve = 0;

// MUX Values
byte RA2_H = 0;
byte RA2_L = 1;
byte RA1_H = 2;
byte RA1_L = 3;
byte RA = 4;
byte SG2 = 5;
byte RB = 6;
byte SG3 = 7;
byte RBC_H = 8;
byte RBC_L = 9;
byte RB1_H = 10;
byte RB1_L = 11;
byte RB2_H = 12;
byte RB2_L = 13;
byte GIA = 14;

CS5532 adc1("Q1Z",cs,sdoFlag,osc);

void setup() {
  //Serial.begin(115200);
  Serial.begin(250000);
  while(!Serial){}
  delay(40);

  pinMode(e, OUTPUT);
  pinMode(em, INPUT_PULLUP);
  pinMode(a0, OUTPUT);
  pinMode(a1, OUTPUT);
  pinMode(a2, OUTPUT);
  pinMode(a3, OUTPUT);
  pinMode(a0m, INPUT_PULLUP);
  pinMode(a1m, INPUT_PULLUP);
  pinMode(a2m, INPUT_PULLUP);
  pinMode(a3m, INPUT_PULLUP);
  pinMode(btn, INPUT_PULLUP);
  pinMode(pause, INPUT_PULLUP);
  pinMode(sdoFlag, INPUT_PULLUP);
  pinMode(pauseLED, OUTPUT);
  pinMode(led, OUTPUT);

  //pinMode(osc, OUTPUT);
  //digitalWrite(osc, HIGH);

  digitalWrite(e, LOW);
  digitalWrite(a0, LOW);
  digitalWrite(a1, LOW);
  digitalWrite(a2, LOW);
  digitalWrite(a3, LOW);
  digitalWrite(pauseLED, LOW);
  digitalWrite(led, LOW);
  attachInterrupt(digitalPinToInterrupt(btn), getMeas, FALLING);
  attachInterrupt(digitalPinToInterrupt(sdoFlag), raiseFlag, FALLING);
  attachInterrupt(digitalPinToInterrupt(em), flipE, FALLING);
  attachInterrupt(digitalPinToInterrupt(a0m), flipA0, FALLING);
  attachInterrupt(digitalPinToInterrupt(a1m), flipA1, FALLING);
  attachInterrupt(digitalPinToInterrupt(a2m), flipA2, FALLING);
  attachInterrupt(digitalPinToInterrupt(a3m), flipA3, FALLING);
  attachInterrupt(digitalPinToInterrupt(pause), pauseHere, FALLING);

  mux(RA2_L);
  delay(10);
  mux(RA1_L);
  delay(10);
  mux(SG2);
  delay(10);
  mux(RB2_L);
  delay(10);
  mux(RB1_L);
  delay(10);
  mux(RBC_L);
  delay(10);
  mux(SG3);
  //delay(1000);
  //mux(GIA);
  //delay(100);
  //mux(14);
  Serial.print("\n\n");

  //startADC();
  Serial.println("Adjust Signal Conditioner as needed");
  while(!collect){
    handMUX();
  }
  collect = false;
  //delay(1000);

  //binSpeed();
  
  startADC2();
  //calOff();
  //        adc1.uploadOffset(Offset1, 3868397824);
  //    adc1.uploadOffset(Offset1, 3908051546);
  //  adc1.uploadOffset(Offset1, 3906343680);
  adc1.uploadOffset(Offset1, 3912222208);
  //calGain();
  //        adc1.uploadGain(Gain1, 48609312);
  //    adc1.uploadGain(Gain1, 25633999);
  //  adc1.uploadGain(Gain1, 25326871);
  adc1.uploadGain(Gain1, 24527489);
  adc1.contMeas(Setup4);
  for(int i=0;i<ele;++i){
    meas[i]=0;
    flags[i]=0;
    t[i]=0;
  }
  //calStrain();
}

void loop() {
  //for(int j=0;j<N;++j){
    //Serial.print(j);
    //Serial.println(":");
    //probe(1200,240);
  //}
  //while(1){}
  //handMUX();
  //livePlot();
  //runBatch();
  calStrain();
  //timeCal();
  delay(50);
}

void startADC2(){
  collect = false;
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW);
  delay(50);
  adc1.init();
  delay(20);
}

void startADC(){
  adc1.init();
  adc1.convReg.Vref = 2.5;
  float offAve = 2.364138E-1;
  adc1.uploadOffset(Offset1,offAve,true);
  int offAveD[24];
  for(int i=0;i<24;++i){
    offAveD[i] = 0;
  }
  uint32_t offOut = 0;
  if(offAve<0){
    offAveD[0] = 1;
  }
  offAve = offAve + offAveD[0]*.5;
  for(int i=1;i<24;++i){
    if(offAve>pow(2,-(i+1))){
      offAveD[i] = 1;
      offAve = offAve - pow(2,-(i+1));
    }
  }
  //Serial.print("\n");
  for(int i=0;i<24;++i){
    offOut = offOut + offAveD[i]*pow(2,23-i);
    //Serial.print(offAveD[i]);
  }
  //Serial.print("\n");
  offOut = offOut<<8;
  //Serial.println(offOut);

  //for(int i=0;i<50;++i){
    //adc1.sysOffCal(Setup1);
    //offRegVal[i] = adc1.offsetReg1.offset;
    //Serial.print("\n");
    //sciNote(offRegVal[i],6);
    //offRegValAve = offRegValAve + offRegVal[i];
  //}
  //offRegValAve = offRegValAve/50;
  //Serial.print("\n");
  //sciNote(offRegValAve,6);
  
  //adc1.sysOffCal(Setup1);
  //adc1.uploadOffset(Offset1,offAve,true);
  adc1.readReg(Gain1);
  adc1.readReg(Offset1);
  adc1.rolecall(Gain1);
  adc1.rolecall(Offset1);
  adc1.contMeas(Setup3);
  
  delay(500);
  collect = false;

  Serial.println("---------------\n\n15s Sample:");
  probe(1800,0);
  offset = measAve;
  G = -2.5130499372955E-05;
  Serial.println(offset);
  Serial.println("\n");
}

void calOff(){
  Serial.println("\n\nHang empty Mass Basket from beam");
  while(!collect){}
  collect = false;
  Serial.println("\nReady to calibrate Offset...");
  while(!collect){}
  digitalWrite(led, HIGH);
  delay(50);
  for(int i=0;i<cals;++i){
    adc1.sysOffCal(Setup3,false);
    Serial.println(i);
    offRegVal[i] = adc1.offsetReg1.out;
    //print32(adc1.offsetReg1.out);
  }
  delay(50);
  digitalWrite(led, LOW);
  collect = false;
  Serial.print('\n');
  for(int i=0;i<cals;++i){
    //Serial.print("\n");
    Serial.print(i);Serial.print(":\t");
    //sciNote(offRegVal[i],6);
    Serial.println(offRegVal[i]);
    offRegValAve = offRegValAve + offRegVal[i];
  }
  offRegValAve = offRegValAve / float(cals);
  Serial.print("\n\nAverage Offset:\t");
  Serial.println(offRegValAve);
  //sciNote(offRegValAve,6);
}

void calGain(){
  Serial.println("\n\nHang 200g from beam");
  while(!collect){}
  collect = false;
  Serial.println("\nReady to calibrate Gain...");
  while(!collect){}
  digitalWrite(led, HIGH);
  delay(50);
  for(int i=0;i<cals;++i){
    adc1.sysGainCal(Setup3,false);
    Serial.println(i);
    gainRegVal[i] = adc1.gainReg1.out;
    //print32(adc1.offsetReg1.out);
  }
  delay(50);
  digitalWrite(led, LOW);
  collect = false;
  Serial.print('\n');
  for(int i=0;i<cals;++i){
    //Serial.print("\n");
    Serial.print(i);Serial.print(":\t");
    //sciNote(gainRegVal[i],6);
    Serial.println(gainRegVal[i]);
    gainRegValAve = gainRegValAve + gainRegVal[i];
  }
  gainRegValAve = gainRegValAve / float(cals);
  Serial.print("\n\nAverage Gain:\t");
  //sciNote(gainRegValAve,6);
  Serial.println(gainRegValAve);
}

void probe(int keep, int cut){
  int tot = keep + cut;
  while(!collect){}
  digitalWrite(led, HIGH);
  measAve = 0.0;
  long meas[keep];
  for(int i=0;i<tot;++i){
    adc1.scanMeas();
    Serial.print(adc1.convReg.measDec);
    Serial.print("\t");
    if(i>=cut){
      meas[i-cut] = adc1.convReg.measDec - offset;
      measAve = measAve + meas[i-cut];
      Serial.println(meas[i-cut]);
    }
  }
  digitalWrite(led, LOW);
  delay(200);
  measAve = measAve/float(keep);
  Serial.print("\n\tAverage:\t");
  Serial.println(measAve);
  Serial.print("\n\tAverage:\t");
  Serial.println(G*measAve);
  Serial.print("\tAverage:\t");
  Serial.print(1000.0*measAve*adc1.convReg.LSB);
  Serial.println(" mV");
  collect = false;
}

void calStrain(){
  Serial.println("\nGenerate Signal Conditioner Calibration Curve");
  for(int i=0;i<50;++i){
    Serial.print("Element ");Serial.println(i);
    collect = false;
    e_state = false;
    while(!(collect||e_state)){}
    if(collect){
      digitalWrite(led, HIGH);
      Serial.println("#START");
      Serial.println(i);
      for(int j=0;j<ele;++j){
        meas[j] = adc1.quickScan();
      }
      for(int j=0;j<ele;++j){
        //Serial.print(meas[j]);
        //Serial.print("\t,\t");
        //Serial.print(meas[j]>>8);
        //Serial.print("\t,\t");
        Serial.println(-((meas[j]>>31)*pow(2,24))+(meas[j]>>8));
        delay(5);
      }
      Serial.println("#END");
      digitalWrite(led,LOW);
    }
    else{
      if(i>0){
        i=i-2;
        digitalWrite(pauseLED, HIGH);
        delay(100);
        digitalWrite(pauseLED, LOW);
      }
    }
  }
}

void runBatch(){
  Serial.println("\n\n Run Batch:");
  while(!collect){}
  Serial.println("\n Starting Collection...\n#START");  //put following under meas[i] line in for loop <meas[i] = adc1.quickScan();>
  Serial.println("t,F,flags");
  digitalWrite(led,HIGH);
  collect = false;
//  ti = micros();
//  for(int i=0;i<ele;++i){
//    adc1.scanMeas();
//    meas = adc1.convReg.measDec;
//    tf = micros();
//    dt = tf - ti;
//    //Serial.println(1000.0*(adc1.convReg.measDec)*adc1.convReg.LSB,6);
//    Serial.print(meas);
//    Serial.print("\t,\t");
//    Serial.println(dt);
//  }
  for(int i=0;i<ele;++i){
    meas[i] = adc1.quickScan();
    t[i] = micros();
  }
  collect = false;
  digitalWrite(led,LOW);
  for(int i=0;i<ele;++i){
    flags[i] = meas[i]&7;
    meas[i] = meas[i]>>8;
  }
  for(int i=0;i<ele;++i){
    //Serial.print(t[i]);
    //Serial.print("\t,\t");
    //Serial.print(meas[i]);
    //Serial.print("\t,\t");
    //Serial.println(flags[i]);
    sprintf(buffer, "%lu,%lu,%d",t[i],meas[i],flags[i]);
    Serial.println(buffer);
    delay(8);
  }
  delay(50);
  Serial.println("#END");
}

void livePlot(){
  Serial.println("\n\n Live Plot:\n");
  while(!collect){}
  Serial.println("\n Starting Collection...\n");
  digitalWrite(led,HIGH);
  collect = false;
  ti = micros();
  //while(!collect){
  for(int i=0;i<ele;++i){
    //adc1.scanMeas();
    //meas[i] = adc1.convReg.measDec;
    meas[i] = adc1.quickScan();
    t[i] = micros();
    //dt = tf - ti;
    //Serial.println(1000.0*(adc1.convReg.measDec)*adc1.convReg.LSB,6);
    //Serial.print(adc1.convReg.measDec);
    //Serial.print("\t,\t");
    //Serial.println(dt);
  }
  for(int i=0;i<ele;++i){
    meas[i] = meas[i]>>8;
  }
  collect = false;
  digitalWrite(led,LOW);
  for(int i=0;i<ele;++i){
    Serial.print(meas[i]);
    Serial.print("\t,\t");
    Serial.println(t[i]);
    delay(10);
    //dt = tf - ti;
    //Serial.println(1000.0*(adc1.convReg.measDec)*adc1.convReg.LSB,6);
    //Serial.print(adc1.convReg.measDec);
    //Serial.print("\t,\t");
    //Serial.println(dt);
  }
}

void timeCal(){
  while(!collect){}
  collect = false;
  int i = 0;
  int T = 1000/120;
  digitalWrite(a1, HIGH);
  delay(1000);
  digitalWrite(a2, HIGH);
  delay(1000);
  digitalWrite(a3, HIGH);
  delay(1000);
  digitalWrite(a1, LOW);
  digitalWrite(a2, LOW);
  digitalWrite(a3, LOW);
  digitalWrite(led,HIGH);
  ti = micros();
  for(i=0;i<5000;++i){
    delay(T);
    t[i] = micros();
    //++i;
  }
  digitalWrite(led,LOW);
  collect = false;
  for(int j=0;j<i;++j){
    Serial.println(t[j]-ti);
    delay(10);
  }
}

void handMUX(){
  if(a0_state){
    a0_out = !a0_out;
    digitalWrite(a0,a0_out);
    a0_state = false;
  }
  if(a1_state){
    a1_out = !a1_out;
    digitalWrite(a1,a1_out);
    a1_state = false;
  }
  if(a2_state){
    a2_out = !a2_out;
    digitalWrite(a2,a2_out);
    a2_state = false;
  }
  if(a3_state){
    a3_out = !a3_out;
    digitalWrite(a3,a3_out);
    a3_state = false;
  }
  if(e_state){
    mux();
    e_state = false;
  }
}

void mux(int _gate){
  int d[4];
  Serial.print(_gate);
  Serial.print("\t");
  for(int i=0;i<4;++i){
    if(_gate>pow(2,3-i)){
      d[3-i] = 1;
      _gate = _gate - pow(2,3-i);
    }
    else{
      d[3-i] = 0;
    }
  }
  char buffer[40];
  sprintf(buffer,"%d\t%d\t%d\t%d",d[0],d[1],d[2],d[3]);
  Serial.println(buffer);
  //sprintf("%d\t",d[0]);
  //sprintf("%d\t",d[1]);
  //sprintf("%d\t",d[2]);
  //sprintf("%d",d[3]);
  
  digitalWrite(a0,d[0]);
  digitalWrite(a1,d[1]);
  digitalWrite(a2,d[2]);
  digitalWrite(a3,d[3]);
  delay(50);
  digitalWrite(e,HIGH);
  delay(50);
  digitalWrite(e,LOW);
  digitalWrite(a0,LOW);
  digitalWrite(a1,LOW);
  digitalWrite(a2,LOW);
  digitalWrite(a3,LOW);
}

void mux(){
  delay(100);
  digitalWrite(e,HIGH);
  delay(2000);
  digitalWrite(e,LOW);
  digitalWrite(a0,LOW);
  digitalWrite(a1,LOW);
  digitalWrite(a2,LOW);
  digitalWrite(a3,LOW);
  a0_state = false;
  a1_state = false;
  a2_state = false;
  a3_state = false;
}

void binSpeed(){
  Serial.print("\n\nTesting Conversion Speeds...\n\n");
  //            00000000000000000000000000000000
  int d32[32] = {};
  float dtF = 0.0;
  float fM = 0.0;
  uint32_t b = 0;
  //           0x12345678   // 1010 | 0011 | 0111 | 1110 | 0010 | 1001 | 1101 | 0010
  uint32_t s = 0xA37E29D2;  // 1010 | 0011 | 0111 | 1110 | 0010 | 1001 | 1101 | 0010
  uint16_t a16 = 0xA37E;
  uint16_t b16 = 0x29D2;
  uint32_t ab = 0;

  k = 0;
  String q = "\t";
  Serial.print("---------------------\nIterative Differencing with Print\n");
  
  delay(20);
  ti = micros();
  for(int i=0;i<32;++i){
    b = pow(2,31-i);
    if(s<10000000){
      q = "\t\t";
    }
    else{
      q = "\t";
    }
    if(k==4){
      Serial.print(" | ");
      k = 0;
    }
    if(s>=b){
      d32[31-i] = 1;
      s = s - b;
    }
    else{
      d32[31-i] = 0;
    }
    Serial.print(d32[31-i]);
    ++k;
  }
  tf = micros();
  delay(20);
  
  dt = tf - ti;
  dtF = float(dt/1000000.0);
  fM = 1.0/dtF;

  Serial.print("\n\nRun Time:\t");
  Serial.print(dt);
  Serial.print(" us = ");
  Serial.print(dtF,5);
  Serial.print(" s\t =>\tMax Freq = ");
  Serial.print(fM,3);
  Serial.print(" Hz\n");

  s = 0xA37E29D2;
  k = 0;
  Serial.print("---------------------\nIterative Differencing without Print\n");
  
  delay(20);
  ti = micros();
  for(int i=0;i<32;++i){
    b = pow(2,31-i);
    if(s>=b){
      d32[31-i] = 1;
      s = s - b;
    }
    else{
      d32[31-i] = 0;
    }
  }
  tf = micros();
  delay(20);
  
  for(int i=0;i<32;++i){
    if(k==4){
      Serial.print(" | ");
      k = 0;
    }
    Serial.print(d32[31-i]);
    ++k;
  }
  dt = tf - ti;
  dtF = float(dt/1000000.0);
  fM = 1.0/dtF;

  Serial.print("\n\nRun Time:\t");
  Serial.print(dt);
  Serial.print(" us = ");
  Serial.print(dtF,5);
  Serial.print(" s\t =>\tMax Freq = ");
  Serial.print(fM,3);
  Serial.print(" Hz\n");

  s = 0xA37E29D2;
  k = 0;
  Serial.print("---------------------\nBitshift with Print\n");
  
  delay(20);
  ti = micros();
  for(int i=0;i<32;++i){
    d32[i] = 1&(s >> i);
    if(k==4){
      Serial.print(" | ");
      k = 0;
    }
    Serial.print(d32[31-i]);
    ++k;
  }
  tf = micros();
  delay(20);
  
  dt = tf - ti;
  dtF = float(dt/1000000.0);
  fM = 1.0/dtF;

  Serial.print("\n\nRun Time:\t");
  Serial.print(dt);
  Serial.print(" us = ");
  Serial.print(dtF,5);
  Serial.print(" s\t =>\tMax Freq = ");
  Serial.print(fM,3);
  Serial.print(" Hz\n");

  s = 0xA37E29D2;
  k = 0;
  Serial.print("---------------------\nBitshift without Print\n");
  
  delay(20);
  ti = micros();
  for(int i=0;i<32;++i){
    d32[i] = 1&(s >> i);
  }
  tf = micros();
 delay(20);
  
  for(int i=0;i<32;++i){
    if(k==4){
      Serial.print(" | ");
      k = 0;
    }
    Serial.print(d32[31-i]);
    ++k;
  }
  dt = tf - ti;
  dtF = float(dt/1000000.0);
  fM = 1.0/dtF;

  Serial.print("\n\nRun Time:\t");
  Serial.print(dt);
  Serial.print(" us = ");
  Serial.print(dtF,5);
  Serial.print(" s\t =>\tMax Freq = ");
  Serial.print(fM,3);
  Serial.print(" Hz\n");

  Serial.print("\n");
  s = 0x0000FFFF;
  k = 0;
  for(int i=0;i<32;++i){
    d32[i] = 1&(s >> i);
  }
  for(int i=0;i<32;++i){
    if(k==4){
      Serial.print(" | ");
      k = 0;
    }
    Serial.print(d32[31-i]);
    ++k;
  }

  Serial.print("\n");
  s = s << 16;
  k = 0;
  for(int i=0;i<32;++i){
    d32[i] = 1&(s >> i);
  }
  for(int i=0;i<32;++i){
    if(k==4){
      Serial.print(" | ");
      k = 0;
    }
    Serial.print(d32[31-i]);
    ++k;
  }

  s = 0xA37E29D2;
  k = 0;
  Serial.print("\n");
  for(int i=0;i<32;++i){
    d32[i] = 1&(s >> i);
  }
  for(int i=0;i<32;++i){
    if(k==4){
      Serial.print(" | ");
      k = 0;
    }
    Serial.print(d32[31-i]);
    ++k;
  }

  ab = (a16 << 16) | b16;
  k = 0;
  Serial.print("\n");
  for(int i=0;i<32;++i){
    d32[i] = 1&(ab >> i);
  }
  for(int i=0;i<32;++i){
    if(k==4){
      Serial.print(" | ");
      k = 0;
    }
    Serial.print(d32[31-i]);
    ++k;
  }

}

void getMeas(){
  if(!collect){
    collect = true;
  }
}

void raiseFlag(){
  if(!adc1.flag){
    adc1.flag = true;
  }
}

void flipA0(){
  if(!a0_state){
    a0_state = true;
  }
}

void flipA1(){
  if(!a1_state){
    a1_state = true;
  }
}

void flipA2(){
  if(!a2_state){
    a2_state = true;
  }
}

void flipA3(){
  if(!a3_state){
    a3_state = true;
  }
}

void flipE(){
  if(!e_state){
    e_state = true;
  }
}

void pauseHere(){
  if(paused){
    paused = false;
  }
  else{
    paused = true;
    digitalWrite(pauseLED, HIGH);
    Serial.println("\nScript Paused");
    while(!digitalRead(pause)){
    }
    digitalWrite(pauseLED, LOW);
    Serial.println("Script Resumed");
  }
}

void print32(uint32_t val){
  int d32[32];
  Serial.print("\n");
  for(int i=0;i<32;++i){
    d32[i] = 0;
  }
  for(int i=0;i<32;++i){
    if(val>=pow(2,31-i)){
      d32[i] = 1;
      val = val - pow(2,31-i);
    }
  Serial.print(d32[i]);
  }
  Serial.print("\n");
}
