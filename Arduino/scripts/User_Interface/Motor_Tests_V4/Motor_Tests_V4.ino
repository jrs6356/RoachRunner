#include <SPI.h>
#include "RoachRunner_MUX.h"
#include "RoachRunner_LCD.h"

//Pins
byte rx0      = 0;
byte tx0      = 1;
byte gui_sw   = 2;
byte gui_b    = 3;
byte gui_a    = 4;
byte motor_b  = 5;
byte motor_a  = 6;
byte osc2     = 7;
byte fg       = 8;
byte esc_pwm  = 9;
byte esc_e    = 10;
byte esc_dir  = 11;
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
bool verboseRR = false;
bool verboseMux = false;
bool pinsSet = false;
bool muxReady = false;

volatile bool a_state = false;
volatile bool b_state = false;
volatile bool scroll = false;
volatile bool bip = true;
volatile int dir = 0;
volatile int enc_val = 0;
volatile char enc_f = 'A';
volatile double esconIn = 0;
volatile double ti = 0;
volatile double tf = 0;
volatile double tc = 0;
volatile float dt = 0;
float tspan = 2000000.0;
double tend = 0;
volatile float p = -1.0;
double speedIn = 0;
float vb = 0.0;
float gr = 3375.0/64.0;
bool pwr = false;
int s10 = 0;
int s1 = 0;
int s01 = 0;
int sOutI = 0;
float sOutP = 0.0;
int sOut = 0;
int sPos = 2;
String lcdOut = "Buffer";
int loops = 0;

int light = 0;

byte muxBytes[13] = {le,e_mux_0,e_mux_1,e_mux_2,e_mux_3,a0_0,a1_0,a2_0,a3_0,a0_1,a1_1,a2_1,a3_1};

MUX mux(muxBytes,&verboseMux,&pinsSet,&muxReady);
Digipot dpot(&cs_digi,&inc_digi,&ud_digi,&mux);
LCD lcd(&cs_lcd,&rs_lcd,&mux,&dpot);

void setup() {
  Serial.begin(250000);
  while(!Serial){}
  SPI.begin();
  delay(50);

  pinMode(gui_sw,INPUT);
  pinMode(gui_a,INPUT);
  pinMode(gui_b,INPUT);
  pinMode(fg,INPUT_PULLUP);
  pinMode(esc_pwm,OUTPUT);
  pinMode(esc_e,OUTPUT);
  pinMode(esc_dir,OUTPUT);

  /*
  pinMode(le,OUTPUT);
  pinMode(e_mux_0,OUTPUT);
  pinMode(e_mux_1,OUTPUT);
  pinMode(e_mux_3,OUTPUT);
  pinMode(a0_0,OUTPUT);
  pinMode(a1_0,OUTPUT);
  pinMode(a2_0,OUTPUT);
  pinMode(a3_0,OUTPUT);
  pinMode(a0_1,OUTPUT);
  pinMode(a1_1,OUTPUT);
  pinMode(a2_1,OUTPUT);
  pinMode(a3_1,OUTPUT);
*/

  analogWriteResolution(12);
  digitalWrite(esc_e,LOW);
  digitalWrite(esc_dir,HIGH);
  analogWrite(esc_pwm,0);
  attachInterrupt(digitalPinToInterrupt(gui_sw),btn,RISING);
  attachInterrupt(digitalPinToInterrupt(gui_a),encA,CHANGE);
  attachInterrupt(digitalPinToInterrupt(gui_b),encB,CHANGE);
  attachInterrupt(digitalPinToInterrupt(fg),bldcPulse,RISING);
  a_state = digitalRead(gui_a);
  b_state = digitalRead(gui_b);

  mux.init();
  lcd.init();
  lcd.contrast(25);
  lcd.backlight(6);
  lcd.clear();
  lcd.command(lcd.row0[2]);
  lcd.print("PWR:0 OFF");
  lcd.command(lcd.row1[2]);
  lcd.print("PWM: 00.0%");
  lcd.command(lcd.row2[2]);
  lcd.print("Ve: 0.0 RPM");
  lcd.command(lcd.row3[2]);
  lcd.print("Vb: 0.0 cm/s");
  lcd.setCursor(1,1);
  sPos = 0;
  scroll = false;

  /*
  lcd.clear();
  lcd.command(lcd.row1[2]);
  lcd.print("BCKL: 0");
  lcd.command(lcd.row1[8]);
  */

}

void loop() {
  //checkScrl2();
  checkBtn();
  checkScrl();
  //if(btnF){
    //doesBcklWork2();
    //btnF = false;
  //}
  if(loops == 10000){
    checkSpeed();
    loops = 0;
  }
  loops = loops + 1;
}

void doesBcklWork2(){
  lcd.backlight(light);
}

void doesBcklWork(){
  int A0v = 1&light;
  int A1v = 1&(light>>1);
  int A2v = 1&(light>>2);
  int A3v = 1&(light>>3);
  Serial.print(light);
  Serial.print(": ");
  Serial.print(A3v);
  Serial.print(A2v);
  Serial.print(A1v);
  Serial.println(A0v);
  digitalWrite(e_mux_0,HIGH);
  digitalWrite(e_mux_1,HIGH);
  digitalWrite(e_mux_3,HIGH);
  delay(50);
  digitalWrite(a0_0,HIGH);
  digitalWrite(a1_0,LOW);
  digitalWrite(a2_0,HIGH);
  digitalWrite(a3_0,HIGH);
  delay(50);
  //digitalWrite(a0_1,HIGH);
  //digitalWrite(a1_1,LOW);
  //digitalWrite(a2_1,HIGH);
  //digitalWrite(a3_1,HIGH);
  digitalWrite(a0_1,A0v);
  digitalWrite(a1_1,A1v);
  digitalWrite(a2_1,A2v);
  digitalWrite(a3_1,A3v);
  delay(50);
  digitalWrite(e_mux_1,LOW);
  delay(2000);
  digitalWrite(e_mux_1,HIGH);
}

void checkScrl2(){
  if(scroll){
    light = light + dir;
    if(light>7){
      light = 7;
    }
    else if(light<0){
      light = 0;
    }
    lcd.command(lcd.row1[8]);
    lcd.print(String(light)+" ");
    scroll = false;
    lcd.command(lcd.row1[8]);
  }
}

void checkSpeed(){
  if(bip){
    if(pwr){
      if(tf>tend){
        speedIn = 1000000.0*30.0*p/(tf-ti);
        vb = (4.4*3.14159*speedIn)/(60.0*gr);
        lcdOut = String(speedIn,0)+" RPM   ";
        lcd.command(lcd.row2[6]);
        lcd.print(lcdOut);
        lcdOut = String(vb, 2)+" cm/s";
        lcd.command(lcd.row3[6]);
        lcd.print(lcdOut);
        Serial.print(p);
        Serial.print("\t");
        Serial.print(tf-ti);
        Serial.print("\t");
        Serial.println(speedIn);
        ti = micros();
        tend = ti+tspan;
        p = 0.0;
      }
    }
    else{
      lcd.command(lcd.row2[6]);
      lcd.print("0 RPM     ");
      lcd.command(lcd.row3[6]);
      lcd.print("0.0 cm/s   ");
      bip = false;
    }
    lcd.command(lcd.row1[7+sPos]);
    if(sPos==4){
      lcd.command(lcd.row0[6]);
    }
  }
}

void checkScrl(){
  if(scroll){
    switch(sPos){
      case 0:
        s10 = s10 + dir;
        if(s10>9){
          s10 = 9;
        }
        else if(s10<0){
          s10 = 0;
        }
        break;
      case 1:
        s1 = s1 + dir;
        if(s1>9){
          if(s10<9){
            s1 = 0;
            s10 = s10 + 1;
          }
          else{
            s1 = 9;
          }
        }
        else if(s1<0){
          if(s10>0){
            s1 = 9;
            s10 = s10 - 1;
          }
          else{
            s1 = 0;
          }
        }
        break;
      case 3:
        s01 = s01 + dir;
        if(s01>9){
          if(s1<9){
            s01 = 0;
            s1 = s1 + 1;
          }
          else{
            if(s10<9){
              s10 = s10 + 1;
              s1 = 0;
              s01 = 0;
            }
            else{
              s01 = 9;
            }
          }
        }
        else if(s01<0){
          if(s1>0){
            s01 = 9;
            s1 = s1 - 1;
          }
          else{
            if(s10>0){
              s10 = s10 - 1;
              s1 = 9;
              s01 = 9;
            }
            else{
              s01 = 0;
            }
          }
        }
        break;
      case 4:
        pwr = !pwr;
        lcd.command(lcd.row0[6]);
        if(pwr){
          lcd.print("1 ON ");
        }
        else{
          lcd.print("0 OFF");
        }
        digitalWrite(esc_e,pwr);
        break;
    }
    lcdOut = String(s10)+String(s1)+"."+String(s01);
    lcd.command(lcd.row1[7]);
    lcd.print(lcdOut);
    lcd.command(lcd.row1[7+sPos]);
    if(sPos==4){
      lcd.command(lcd.row0[6]);
    }
    sOutI = (100*s10)+(10*s1)+s01;
    sOutP = ((float)sOutI)/1000.0;
    sOut = sOutP*4096.0;
    //Serial.print("\nPWM: ");Serial.print(s10);Serial.print(s1);Serial.print(".");Serial.print(s01);Serial.print("%");
    //Serial.print("\t");Serial.print(sOutI);Serial.print("\t");Serial.print(sOutP,3);Serial.print("\t");Serial.print(sOut);
    scroll = false;
    analogWrite(esc_pwm,sOut);
  }
}

void checkBtn(){
  if(btnF){
    if(sPos==4){
      sPos = 0;
    }
    else{
      sPos = sPos + 1;
      if(sPos==2){
        sPos = 3;
      }
    }
    lcd.command(lcd.row1[7+sPos]);
    if(sPos==4){
      lcd.command(lcd.row0[6]);
    }
    btnF = false;
    scroll = false;
  }
}

void btn(){
  if(!btnF){
    btnF = true;
  }
}

void encA(){
  enc_f = 'A';
  //a_state = !a_state;
  //if(a_state == b_state){
    //dir = -1;
  //}
  //else{
    //dir = 1;
  //}
  //scroll = true;
  //
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
  //
}

void encB(){
  enc_f = 'B';
  //b_state = !b_state;
  //if(a_state == b_state){
    //dir = 1;
  //}
  //else{
    //dir = -1;
  //}
  //scroll = true;
  //
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
  //
}

void bldcPulse(){
  //esconIn = esconIn + 1;
  //bip = true;
  if(pwr){
    tc = tf;
    tf = micros();
    dt = tf - tc;
    if((dt<990)||(dt>1005)){
      p = p + 1;
      bip = true;
    }
  }
}
