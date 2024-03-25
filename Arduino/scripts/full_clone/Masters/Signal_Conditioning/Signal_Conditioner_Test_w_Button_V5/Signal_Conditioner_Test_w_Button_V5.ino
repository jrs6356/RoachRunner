#include <SPI.h>
#include "RoachRunner_Digipot.h"
#include "RoachRunner_CS5532.h"

//Pins
byte cs = 39;
byte osc = 7;
byte sdoFlag = 45;

byte shunt1 = 27;
byte shunt2 = 33;

byte led = 25;
byte btn = 24;

// Variables
int fosc = 2e6;

int k = 0;
int m = 0;
int n = 0;
int N = 51;
double offset = 0;
double t1 = 0;
double t2 = 0;
double dt = 0;
float f = 0;
float G = 1;
double offRegVal[50];
double offRegValAve = 0;
volatile bool collect = false;

//uint32_t meas[1200];
//uint32_t measBig[1800];
double measAve = 0;

CS5532 adc1("Q1Z",cs,sdoFlag,osc);

void setup() {
  Serial.begin(115200);
  while(!Serial){}

  pinMode(btn, INPUT_PULLUP);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  attachInterrupt(digitalPinToInterrupt(btn), getMeas, RISING);

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

void loop() {
  for(int j=0;j<N;++j){
    Serial.print(j);
    Serial.println(":");
    probe(1200,240);
  }
  while(1){}
}

void probe(int keep, int cut){
  int tot = keep + cut;
  while(!collect){}
  digitalWrite(led, HIGH);
  measAve = 0.0;
  long meas[keep];
  for(int i=0;i<tot;++i){
    adc1.scanMeas();
    //Serial.print(adc1.convReg.measDec);
    //Serial.print("\t");
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

void getMeas(){
  if(!collect){
    collect = true;
  }
}
