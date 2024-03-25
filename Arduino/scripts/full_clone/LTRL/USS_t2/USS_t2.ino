int red = 1;
int blue = 2;
int trig = 4;
int echo = 5;

void setup() {
  pinMode(red,OUTPUT);
  pinMode(blue,OUTPUT);
  
}

void loop() {
  LEDstuff();
}
void LEDstuff(){
  digitalWrite(red,HIGH);
  Serial.println("Red");
  delay(500);
  digitalWrite(red,LOW);
  delay(10);
  digitalWrite(blue,HIGH);
  Serial.println("Blue");
  delay(500);
  digitalWrite(blue,LOW);
  delay(500);
}

