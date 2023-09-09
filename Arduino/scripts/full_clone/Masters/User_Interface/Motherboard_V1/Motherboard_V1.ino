#include <SPI.h>
#include <Servo.h>
#include "RoachRunner_LCD.h"
#include "RoachRunner_Digipot.h"
#include "RoachRunner_MUX.h"

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

byte lcd_bckl_off = 32;
byte lcd_bckl_on = 45;
byte lcd_r1_off = 33;
byte lcd_r1_on = 44;
byte lcd_r2_off = 34;
byte lcd_r2_on = 43;
byte lcd_r3_off = 35;
byte lcd_r3_on = 42;
byte lcd_r4_off = 36;
byte lcd_r4_on = 41;
byte lcd_r5_off = 37;
byte lcd_r5_on = 40;
byte lcd_r6_off = 38;
byte lcd_r6_on = 39;

MUX mux(le,e_mux_0,e_mux_1,e_mux_2,e_mux_3,a0_0,a1_0,a2_0,a3_0,a0_1,a1_1,a2_1,a3_1);
Digipot dpot(cs_digi,inc_digi,ud_digi,mux);
LCD lcd(cs_lcd,rs_lcd,mux,dpot);
Servo servo;

//Variables
volatile int dir = 0;
volatile int dirM = 0;
int enc_val = 0;
int enc_valM = 0;
int speedM = 0;
volatile bool bounce = false;
volatile bool sw_dir = 0;
volatile bool scroll = false;
volatile bool scrollM = false;
volatile bool a_state = false;
volatile bool b_state = false;
volatile bool a_stateM = false;
volatile bool b_stateM = false;

float r = 0.0;
uint32_t leds = 0;

void setup() {
  Serial.begin(250000);
  while(!Serial){}
  delay(10);
  SPI.begin();
  SPI.beginTransaction(SPISettings((210000),MSBFIRST,SPI_MODE3));
  mux.init();
  lcd.init();
  lcd.contrast(25);
  lcd.backlight(7);
  lcd.clear();
  lcd.makeEmojis();
  lcd.command(lcd.row1[2]);
  lcd.writeData(3);
  lcd.command(lcd.row1[4]);
  lcd.print("RoachRunner ");
  lcd.writeData(3);
  delay(1000);
  lcd.clear();
  lcd.command(lcd.row1[7]);
  lcd.print("Speed:");

  Serial.println(le);
  Serial.println(lcd.mux.le);
  
  pinMode(gui_a, INPUT_PULLUP);
  pinMode(gui_b, INPUT_PULLUP);
  pinMode(motor_a, INPUT_PULLUP);
  pinMode(motor_b, INPUT_PULLUP);
  pinMode(esc_pwm, OUTPUT);
  pinMode(esc_tx, INPUT);
  //servo.attach(esc_pwm);
  pinMode(gui_sw, INPUT_PULLUP);
  pinMode(led_clr, OUTPUT);
  pinMode(led_clk, OUTPUT);
  pinMode(buzzer, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(gui_a), encA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(gui_b), encB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(gui_sw), ensw, RISING);
  attachInterrupt(digitalPinToInterrupt(motor_a), encAm, CHANGE);
  attachInterrupt(digitalPinToInterrupt(motor_b), encBm, CHANGE);
  
  digitalWrite(led_clr, HIGH);
  digitalWrite(led_clk, HIGH);
  digitalWrite(buzzer, LOW);
  analogWrite(esc_pwm, 0);
  //servo.writeMicroseconds(speedM);
  delay(7000);

  //enc_val = 0;
  //randomSeed(analogRead(0));
  //func2();
  //panel(1417339264);
  //panel(500);
  //mux(lcd_bckl_off,1);
  //mux(lcd_r1_on,1);
  //mux(lcd_r2_off,1);
  //mux(lcd_r3_off,1);
  //mux(lcd_r4_off,1);
  //mux(lcd_r5_off,1);
  //mux(lcd_r6_off,1);

}

void loop() {
  func3();
  //servo.writeMicroseconds(speedM);
  analogWrite(esc_pwm, speedM);
}

void func(){
  if(scroll){
    scroll = false;
    digitalWrite(led_clk, HIGH);
    delay(5);
    digitalWrite(led_clk, LOW);
    //beep();
    enc_val = enc_val + dir;
    Serial.println(enc_val);
    lcd.contrast(enc_val);
    lcd.command(lcd.row2[9]);
    lcd.print(String(lcd.brightness));
    lcd.print(" ");
    //mux.out(enc_val,1);
    //if(sw_dir){
      //mux(0,0);
      //mux(enc_val,1);
    //}
  }
  if(bounce){
    bounce = false;
    beep(100);
    //mux.out(enc_val,1);
    digitalWrite(led_clr, HIGH);
    delay(5);
    digitalWrite(led_clr, LOW);
    //if(sw_dir){
      //sw_dir = 0;
      //mux(0,0);
    //}
    //else{
      //sw_dir = 1;
      //mux(0,0);
      //mux(enc_val,1);
    //}
  }
}

void func2(){
  r = random(0,100)/100.0;
  Serial.print(r);
  Serial.print('\t');
  leds = 4294967295;
  leds = leds*r;
  Serial.println(leds);
  panel(leds);
}

void func3(){
  if(scrollM){
    scrollM = false;
    enc_valM = enc_valM + dirM;
    Serial.println(enc_valM);
  }
  if(scroll){
    scroll = false;
    enc_val = enc_val + dir;
    Serial.println(enc_val);
    if(enc_val<0){
      speedM = 0;
    }
    else if(enc_val>255){
      speedM = 255;
    }
    else{
      speedM = enc_val;
    }
    lcd.command(lcd.row2[9]);
    lcd.print(String(speedM));
    lcd.print(" ");
    //analogWrite(esc_pwm,speedM);
  }
}

void encA(){
  scroll = true;
  a_state = digitalRead(gui_a);
  b_state = digitalRead(gui_b);
  if(a_state){
    if(b_state){
      dir = -1;
    }
    else{
      dir = 1;
    }
  }
  else{
    if(b_state){
      dir = 1;
    }
    else{
      dir = -1;
    }
  }
}

void encB(){
  scroll = true;
  a_state = digitalRead(gui_a);
  b_state = digitalRead(gui_b);
  if(b_state){
    if(a_state){
      dir = 1;
    }
    else{
      dir = -1;
    }
  }
  else{
    if(a_state){
      dir = -1;
    }
    else{
      dir = 1;
    }
  }
}

void encAm(){
  scrollM = true;
  a_stateM = digitalRead(motor_a);
  b_stateM = digitalRead(motor_b);
  if(a_stateM){
    if(b_stateM){
      dirM = -1;
    }
    else{
      dirM = 1;
    }
  }
  else{
    if(b_stateM){
      dirM = 1;
    }
    else{
      dirM = -1;
    }
  }
}

void encBm(){
  scrollM = true;
  a_stateM = digitalRead(motor_a);
  b_stateM = digitalRead(motor_b);
  if(b_stateM){
    if(a_stateM){
      dirM = 1;
    }
    else{
      dirM = -1;
    }
  }
  else{
    if(a_stateM){
      dirM = -1;
    }
    else{
      dirM = 1;
    }
  }
}

void ensw(){
  if(!bounce){
    bounce = true;
  }
}

void beep(int _t){
  digitalWrite(buzzer, HIGH);
  delay(_t);
  digitalWrite(buzzer, LOW);
}

void panel(uint32_t _state){
  digitalWrite(led_clr, HIGH);
  delay(5);
  digitalWrite(led_clr, LOW);
  for(uint32_t _i=0;_i<_state;++_i){
    digitalWrite(led_clk, HIGH);
    delayMicroseconds(10);
    digitalWrite(led_clk, LOW);
    delayMicroseconds(10);
    //REG_PIOA_SODR |= (0x01 << 0);
    //REG_PIOA_CODR |= (0x01 << 0);
    //PIOA -> PIO_SODR = 1;
    //delayMicroseconds(1);
    //PIOA -> PIO_CODR = 1;
    //delayMicroseconds(1);
  }
}
