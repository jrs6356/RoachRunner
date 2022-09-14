#include <SPI.h>
#include "RoachRunner_Digipot.h"
#include "RoachRunner_CS5532.h"

//Pins
byte cs = 2;
byte osc = 7;
byte sdoFlag = 12;
byte sclkSim = 22;
byte mosiSim = 23;
byte misoSim = 24;
byte pause = 29;
byte pauseLED = 30;

//Variables
int fosc = 2e6;
volatile bool paused = false;
float Fclock = 4.9152;
bool clockStarted = false;
float MCLK = 0.0;

CS5532 adc1("Q1Z",cs,sdoFlag,osc);

void setup(){
  Serial.begin(115200);
  while(!Serial){}

  //pinMode(cs, OUTPUT);
  //pinMode(osc, OUTPUT);
  pinMode(sclkSim, OUTPUT);
  pinMode(mosiSim, OUTPUT);
  pinMode(pauseLED, OUTPUT);
  
  //pinMode(sdoFlag, INPUT);
  pinMode(misoSim, INPUT);
  pinMode(pause, INPUT_PULLUP);

  //digitalWrite(cs, HIGH);
  //digitalWrite(osc, LOW);
  digitalWrite(sclkSim, LOW);
  digitalWrite(mosiSim, LOW);
  
  digitalWrite(pauseLED, LOW);

  attachInterrupt(digitalPinToInterrupt(pause), pauseHere, FALLING);
  //startClock();
  adc1.init();
}

void loop(){
  delay(5000);
  digitalWrite(osc, HIGH);
  digitalWrite(sclkSim, HIGH);
  digitalWrite(mosiSim, HIGH);
  delay(5000);
  digitalWrite(osc, LOW);
  digitalWrite(sclkSim, LOW);
  digitalWrite(mosiSim, LOW);
}

void startClock(){
  Serial.print("\tStarting Clock...\n\t");
  int m = 84/Fclock;
  int m2 = m/2;
  int32_t mask_PWM_pin = digitalPinToBitMask(osc);
  MCLK = float(84)/float(m);
  REG_PMC_PCER1 = 1<<4; // activate clock for PWM controller
  REG_PIOC_PDR |= mask_PWM_pin; // activate peripheral functions for pin (disables all PIO functionality)
  REG_PIOC_ABSR |= mask_PWM_pin; // choose peripheral option B
  REG_PWM_CLK = 0; // choose clock rate, 0 → full MCLK as reference 84MHz
  REG_PWM_CMR6 = 0<<9; // select clock and polarity for PWM channel (pin7) → (CPOL = 0)
  REG_PWM_CPRD6 = m; // initialize PWM period → T = value/84MHz (value: up to 16bit), value=8 → 10.5MHz
  REG_PWM_CDTY6 = m2; // initialize duty cycle, REG_PWM_CPRD6 / value = duty cycle, for 8/4 = 50%
  REG_PWM_ENA = 1<<6; // enable PWM on PWM channel (pin 7 = PWML6)
  Serial.print(MCLK,3);
  Serial.println(" MHz MCLK Activated");
  clockStarted = true;
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
