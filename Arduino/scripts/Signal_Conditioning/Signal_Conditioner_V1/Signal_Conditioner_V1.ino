#include <SPI.h>
#include "RoachRunner_MUX.h"
#include "RoachRunner_IMU.h"
#include "RoachRunner_SGSC.h"
#include "RoachRunner_ADC.h"
#include "RoachRunner_LCD.h"
#include "RoachRunner_Digipot.h"

//Pins
byte rx0      = 0;
byte tx0      = 1;
byte gui_sw   = 2;
byte gui_b    = 3;
byte gui_a    = 4;
byte motor_b  = 5;
byte motor_a  = 6;
byte osc2     = 7;
byte esc_tx   = 8;
byte esc_pwm  = 9;
byte lcd2_rs  = 10;
byte lcd2_a0  = 11;
byte le       = 12;
byte lcd2_sw  = 13;
byte a3_1     = 22;
byte a2_1     = 23;
byte a1_1     = 24;
byte a0_1     = 25;
byte rs_lcd   = 26;
byte ud_digi  = 27;
byte inc_digi = 28;
byte a0_0     = 29;
byte a1_0     = 30;
byte a2_0     = 31;
byte a3_0     = 32;
byte e_mux_0  = 33;
byte e_mux_1  = 34;
byte e_mux_3  = 35;
byte sdo0     = 36;
byte sdo1     = 37;
byte sdo2     = 38;
byte sdo3     = 39;
byte sdo4     = 40;
byte sdo5     = 41;
byte sdo6     = 42;
byte sdo7     = 43;
byte sdo8     = 44;
byte sdo9     = 45;
byte sdo10    = 46;
byte sdo11    = 47;
byte sdo_sd   = 48;
byte sdo_imu  = 49;
byte led_clr  = 50;
byte led_clk  = 51;
byte lcd2_b   = 52;
byte lcd2_a   = 53;
byte lcd2_bkl = 68;
byte buzzer   = 69;

//MUX Channels
byte cs_0 = 0;
byte cs_1 = 1;
byte cs_2 = 2;
byte cs_3 = 3;
byte cs_4 = 4;
byte cs_5 = 5;
byte cs_6 = 6;
byte cs_7 = 7;
byte cs_8 = 8;
byte cs_9 = 9;
byte cs_10 = 10;
byte cs_11 = 11;
byte cs_lcd = 12;
byte cs_sd = 13;
byte cs_ag = 14;
byte cs_m = 15;
byte cs_digi = 28;
byte cs_lcd2 = 30;
byte cs_all = 31;

byte e_0 = 16;
byte e_1 = 17;
byte e_2 = 18;
byte e_3 = 19;
byte e_4 = 20;
byte e_5 = 21;
byte e_6 = 22;
byte e_7 = 23;
byte e_8 = 24;
byte e_9 = 25;
byte e_10 = 26;
byte e_11 = 27;
byte e_mux_2 = 29;
byte e_sdo_0 = 48;
byte e_sdo_1 = 49;
byte e_sdo_2 = 50;
byte e_sdo_3 = 51;
byte e_sdo_4 = 52;
byte e_sdo_5 = 53;
byte e_sdo_6 = 54;
byte e_sdo_7 = 55;
byte e_sdo_8 = 56;
byte e_sdo_9 = 57;
byte e_sdo_10 = 58;
byte e_sdo_11 = 59;
byte e_sdo_sd = 60;
byte e_sdo_imu = 61;

byte r2s30 = 64;
byte r2s31 = 65;
byte r2s20 = 66;
byte r2s21 = 67;
byte r2sg = 68;
byte r2sh = 69;
byte r3sg = 70;
byte r3sh = 71;
byte r3s10 = 72;
byte r3s11 = 73;
byte r3s20 = 74;
byte r3s21 = 75;
byte r3s30 = 76;
byte r3s31 = 77;
byte jk = 78;

//Variables
volatile bool btnF = false;
volatile bool sdoF = false;
bool verboseRR = true;
bool verboseMux = false;
bool pinsSet = false;
bool muxReady = false;
bool clockStarted = false;
bool SPIStartedADC = false;
bool ADCSerialInit = false;
bool allClear = false;

byte adcN = 0;
byte adcSetN = 0;

uint32_t CONF = 0;
uint32_t CSR1 = 0;
uint32_t CSR2 = 0;
uint32_t CSR3 = 0;
uint32_t CSR4 = 0;
uint32_t CSR5 = 0;
uint32_t CSR6 = 0;
uint32_t CSR7 = 0;
uint32_t CSR8 = 0;

bool* flagPtrs[6] = {&verboseRR,&pinsSet,&clockStarted,&SPIStartedADC,&ADCSerialInit,&allClear};
byte* bytePtrs[4] = {&adcN,&adcSetN,&cs_all,&osc2};
uint32_t* settings[9] = {&CONF,&CSR1,&CSR2,&CSR3,&CSR4,&CSR5,&CSR6,&CSR7,&CSR8};
byte muxBytes[13] = {le,e_mux_0,e_mux_1,e_mux_2,e_mux_3,a0_0,a1_0,a2_0,a3_0,a0_1,a1_1,a2_1,a3_1};

volatile bool a_state = false;
volatile bool b_state = false;
volatile bool scroll = false;
volatile int dir = 0;
volatile int enc_val = 0;
volatile char enc_f = 'A';

MUX mux(muxBytes,&verboseMux,&pinsSet,&muxReady);
IMU imu(cs_ag, cs_m, e_sdo_imu, &mux);
Digipot dpot(&cs_digi,&inc_digi,&ud_digi,&mux);
//LCD lcd(&cs_lcd,&rs_lcd,&mux,&dpot);
SGSC sc0("X0",&cs_0,&e_0,&e_sdo_0,&sdoF,bytePtrs,flagPtrs,settings,&mux);
SGSC sc1("Y0",&cs_4,&e_4,&e_sdo_4,&sdoF,bytePtrs,flagPtrs,settings,&mux);
SGSC sc2("Z0",&cs_8,&e_8,&e_sdo_8,&sdoF,bytePtrs,flagPtrs,settings,&mux);

void setup(){
  Serial.begin(250000);
  while(!Serial){}
  SPI.begin();
  delay(50);

  pinMode(gui_sw,INPUT_PULLUP);
  pinMode(gui_a,INPUT);
  pinMode(gui_b,INPUT);
  pinMode(sdo_imu, INPUT);
  pinMode(sdo0,INPUT_PULLUP);
  pinMode(sdo8,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(gui_sw),btn,RISING);
  attachInterrupt(digitalPinToInterrupt(sdo0),sdoFisr,FALLING);
  attachInterrupt(digitalPinToInterrupt(gui_a),encA,CHANGE);
  attachInterrupt(digitalPinToInterrupt(gui_b),encB,CHANGE);

  mux.init();

  imu.begin();
  //if(!imu.begin()){
    //Serial.println("Oops ... unable to initialize the LSM9DS1. Check your wiring!");
    //while (1);
  //}
  imu.setupAccel(imu.LSM9DS1_ACCELRANGE_2G);
  imu.setupGyro(imu.LSM9DS1_GYROSCALE_245DPS);
  delay(10);
  
  sc0.init(0);
  sc1.init(0);
  sc2.init(0);  //Gia needs to be 10 for Z

  //sc2.setBridge(r2sh,r3s11);
  //sc2.setBridge(r2s30,r3sh);

  sc0.adc.SPIS(1);
  sc0.adc.SRS(1);
  //sc2.adc.SPIS(0);
  //sc2.adc.SRS(0);

  sc1.adc.readReg(Config);
  sc2.adc.readReg(Config);
  
  sc0.adc.VRSinit(1);
  sc0.adc.CSRinit(1);
  //sc2.adc.VRSinit(0);
  //sc2.adc.CSRinit(0);

  //verboseRR = false;

  sc1.adc.follow();
  sc2.adc.follow();

  //verboseRR = true;

  sc2.adc.setup1.G = G8;
  sc2.adc.setup1.merge();
  sc2.adc.writeReg(Setup1,0);
  sc2.adc.setup2.G = G8;
  sc2.adc.setup2.merge();
  sc2.adc.writeReg(Setup2,0);
  sc2.adc.readReg(Setup1);
  sc2.adc.readReg(Setup2);

  sc0.adc.setup1.G = G8;
  sc0.adc.setup1.merge();
  sc0.adc.writeReg(Setup1,0);
  sc0.adc.setup2.G = G8;
  sc0.adc.setup2.merge();
  sc0.adc.writeReg(Setup2,0);
  sc0.adc.readReg(Setup1);
  sc0.adc.readReg(Setup2);

  verboseRR = true;

  sc0.adc.sysOffCal(Setup1,1);
  sc1.adc.readReg(Offset1);
  sc2.adc.readReg(Offset1);

  //sc2.adc.sysOffCal(Setup3,0);

  sc0.adc.flightCheck();
  sc1.adc.flightCheck();
  sc2.adc.flightCheck();

  delay(10);

  sc0.adc.contMeas(Setup2,1);
  //sc2.adc.contMeas(Setup4,0);

  //verboseRR = false;

  a_state = digitalRead(gui_a);
  b_state = digitalRead(gui_b);
  
  Serial.println("#Start");
}

void loop(){
  checkScroll();
  checkBtn();
  measure();
  accel();
}

void measure(){
  sc0.adc.startSPI();
  mux.on(cs_0);
  uint32_t m0 = sc0.adc.scanMeas();
  uint32_t m1 = sc1.adc.scanMeas();
  uint32_t m2 = sc2.adc.scanMeas();
  uint32_t t = sc0.adc.timestamp;
  sc0.adc.stopSPI();
  double td = ((double)t)/pow(10,6);
  int m0i = sc0.adc.conv2Dec();
  int m1i = sc1.adc.conv2Dec();
  int m2i = sc2.adc.conv2Dec();
  double m0d = ((double)m0i)/pow(10,6);
  double m1d = ((double)m1i)/pow(10,6);
  double m2d = ((double)m2i)/pow(10,6);
  sdoF = false;
  //mux.on(cs_0);
  //mux.on(cs_8);
  
  Serial.print(td,4);Serial.print(",");
  Serial.print(m0d,6);Serial.print(",");
  Serial.print(m1d,6);Serial.print(",");
  Serial.print(m2d,6);Serial.print(",");
  
  //print32(m2);Serial.print("\n");
  
  //print32(sc0.adc.conv2Dec());Serial.print("\n");
  //Serial.println(m0>>8);//Serial.print(",");
  //Serial.print(m1>>8);Serial.print(",");
  //Serial.println(m2>>8);
  //delay(50);
}

void accel(){
  imu.read();
  sensors_event_t a, m, g, temp;

  imu.getEvent(&a, &m, &g, &temp); 

  //Serial.print(a.timestamp);
  //Serial.print(",");
  Serial.print(a.acceleration.x,4);
  Serial.print(",");
  Serial.print(a.acceleration.y,4);
  Serial.print(",");
  Serial.println(a.acceleration.z,4);
}

void print32(uint32_t _b){
  for(int i=0;i<32;++i){
    if((i>0)&&((i%4)==0)){
      Serial.print(" | ");
    }
    Serial.print(1&(_b>>(31-i)));
  }
}

void checkScroll(){
  if(scroll){
    enc_val = enc_val + dir;
    if(verboseRR){
      Serial.println("-------------------------");
      Serial.print(enc_f);Serial.println(" Change");
      Serial.print(enc_val);Serial.print("\t(");Serial.print(dir);Serial.println(")");
      Serial.print("A: ");Serial.print(digitalRead(gui_a));Serial.print(" ");Serial.println(a_state);
      Serial.print("B: ");Serial.print(digitalRead(gui_b));Serial.print(" ");Serial.println(b_state);
    }
    scroll = false;
  }
}

void checkBtn(){
  if(btnF){
    //Serial.println("JK");
    if(enc_val>3){
      enc_val = 3;
    }
    else if(enc_val<1){
      enc_val = 1;
    }
    if(verboseRR){
      Serial.print(dir);Serial.print("\t");
      Serial.print(enc_val);Serial.print("\t");
      Serial.print(a_state);Serial.println(b_state);
    }
    switch(enc_val){
      case 1:
        sc0.jk();
        //Serial.println("SC0 JK");
        break;
      case 2:
        sc1.jk();
        //Serial.println("SC1 JK");
        break;
      case 3:
        sc2.jk();
        //Serial.println("SC2 JK");
        break;
    }
    //sc0.jk();
    //sc1.jk();
    //sc2.jk();
    btnF = false;
  }
}

void ptrTest(byte i){
  //byte* p = &i;
  Serial.println(i);
  Serial.println((long) &i, HEX);
}

void ptrTest2(byte* i){
  Serial.println(*i);
  Serial.println((long) i, HEX);
  *i = 5;
}

void bridgeCycle(){
  Serial.println("20 , 11");
  delay(2000);
  //sc0.setBridge(r2s20,r3s11);
  Serial.println("30 , 20");
  delay(2000);
  //sc0.setBridge(r2s30,r3s20);
  Serial.println("30 , 30");
  delay(2000);
  //sc0.setBridge(r2s30,r3s30);
  Serial.println("SH , SH");
  delay(2000);
  //sc0.setBridge(r2sh,r3sh);
  Serial.println("21 , 21");
  delay(2000);
  //sc0.setBridge(r2s21,r3s21);
  Serial.println("31 , 31");
  delay(2000);
  //sc0.setBridge(r2s31,r3s31);
  Serial.println("30 , 30");
  delay(2000);
  //sc0.setBridge(r2s30,r3s30);
  Serial.println("20 , 20");
  delay(2000);
  //sc0.setBridge(r2s20,r3s20);
  Serial.println("SG , SG");
  delay(2000);
  //sc0.setBridge(r2sg,r3sg);
}

void btn(){
  if(!btnF){
    btnF = true;
  }
}

void sdoFisr(){
  if(!sdoF){
    sdoF = true;
  }
}

void encA(){
  enc_f = 'A';
  if(!scroll){
    a_state = !a_state;
    if(a_state == b_state){
      dir = -1;
    }
    else{
      dir = 1;
    }
    scroll = true;
  }
}

void encB(){
  if(!scroll){
    enc_f = 'B';
    b_state = !b_state;
    if(a_state == b_state){
      dir = 1;
    }
    else{
      dir = -1;
    }
    scroll = true;
  }
}
