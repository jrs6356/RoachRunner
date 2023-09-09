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
bool verboseMux = true;
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
LCD lcd(&cs_lcd,&rs_lcd,&mux,&dpot);

void setup(){
  Serial.begin(250000);
  while(!Serial){}
  SPI.begin();
  delay(50);

  mux.init();
  imu.begin();
  //dpot.init();
  lcd.init();
  lcd.contrast(25);
  lcd.backlight(4);
  lcd.command(lcd.row1[4]);
  lcd.print("RoachRunner ");
}

void loop(){
  
}
