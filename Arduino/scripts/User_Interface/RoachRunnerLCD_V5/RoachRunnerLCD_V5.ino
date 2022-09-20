#include <SPI.h>
#include "RoachRunner_LCD.h"
#include "Digipot.h"

//Pin Assignments
byte inc = 2;
byte cs_pot = 3;
byte ud = 4;
byte up = 5;
byte down = 6;
byte ledUp = 7;
byte ledDown = 8;
byte sw = 9;
byte a2 = 10;
byte b2 = 11;
byte cs_lcd = 38;
byte rs_lcd = 39;
String message = "  ";

volatile bool scroll = false;
volatile int dir = 0;
volatile int a_state = false;
volatile int b_state = false;

LCD lcd(cs_lcd, cs_pot, rs_lcd, inc, ud);

void setup() {
  Serial.begin(115200);
  while(!Serial){}
  SPI.begin();
  SPI.beginTransaction(SPISettings((210000),MSBFIRST,SPI_MODE3));
  lcd.init();
  pinMode(a2, INPUT_PULLUP);
  pinMode(b2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(a2), encA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(b2), encB, CHANGE);
  //LCD lcd(cs_lcd, cs_pot, rs_lcd, inc, ud);
  //LCD lcd(cs_lcd, cs_pot, rs_lcd, inc, ud, ledUp, ledDown);
  message = "This lib works!";
  //lcd.writeRow(message,1);
  lcd.writeRow("Main Menu",0);
  lcd.writeRow("Speed",1);
  lcd.writeRow("Orientation",2);
  lcd.writeRow("Vibration",3);
  lcd.writeChar('>',1,0);
  lcd.writeChar(lcd.backArrow,0,19);
  lcd.writeChar(lcd.arrowR,1,19);
  lcd.writeChar(lcd.sigma,2,19);
  lcd.writeChar(lcd.pi,3,19);
  Serial.println("Brightness  \tMI");
  //Serial.println(lcd.dpot.MI);
  //delay(1000);
  //Serial.println(lcd.brightness);
  //Serial.println(lcd.dpot.MI);
  //delay(5000);
  //lcd.setCursor(1,1);
  //delay(5000);
  //lcd.setCursor(0,0);
}

void loop() {
  setContrast();

}

void setContrast(){
  if(scroll){
    lcd.contrast(lcd.brightness + dir);
    Serial.print(lcd.brightness);Serial.print("\t\t");Serial.println(lcd.dpot.MI);
    scroll = false;
  }
}

void encA(){
  scroll = true;
  a_state = digitalRead(a2);
  b_state = digitalRead(b2);
  if(a_state==HIGH){
    if(b_state==HIGH){
      dir = -1;
    }
    else{
      dir = 1;
    }
  }
  else{
    if(b_state==HIGH){
      dir = 1;
    }
    else{
      dir = -1;
    }
  }
}

void encB(){
  scroll = true;
  a_state = digitalRead(a2);
  b_state = digitalRead(b2);
  if(b_state==HIGH){
    if(a_state==HIGH){
      dir = 1;
    }
    else{
      dir = -1;
    }
  }
  else{
    if(a_state==HIGH){
      dir = -1;
    }
    else{
      dir = 1;
    }
  }
}
