#include <Servo.h>
//#define reset_button 3
//#define enable_button 2

Servo myservo;
int pos = 30;

const int uB = 6;
const int dB = 2;
const int ledPin1 = 3;
const int ledPin2 = 4;
const int ledPinE = 5;
int sensorPin1 = A3;
int sensorPin2 = A2;

int b1 = 0;
int b2;
int fFS;
int sFS;
int i = 1;
int j = 0;
int k = 0;

void setup() {
  myservo.attach(9);
  Serial.begin(9600);
  pinMode(uB, INPUT);
  pinMode(dB, INPUT);
  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPinE, OUTPUT);
  myservo.write(pos);
}

void loop() {
  f1();
  delay(100);
}

void button(){
  Serial.println(digitalRead(dB));
}

void f1(){
  j=0;
  k=0;
  delay(100);
  digitalWrite(ledPin1,LOW);
  digitalWrite(ledPin2,LOW);
  digitalWrite(ledPinE,LOW);
  byte states = getStates();
  Serial.print("F1  ");
  Serial.print(bitRead(states,3));
  Serial.print(bitRead(states,2));
  Serial.print(bitRead(states,1));
  Serial.println(bitRead(states,0));
  fFS = bitRead(states,3);
  sFS = bitRead(states,2);
  b1 = bitRead(states,1);
  b2 = bitRead(states,0);
  myservo.write(pos);
  if((fFS==1)&&(sFS==0)&&(b1==0)&&(b2==1)){
    up();
  }
  else if((sFS==1)){
    while(1){
      error();
      byte states = getStates();
      digitalWrite(ledPin1,LOW);
      digitalWrite(ledPin2,LOW);
      digitalWrite(ledPinE,HIGH);
      Serial.print("F1  ");
      Serial.print(bitRead(states,3));
      Serial.print(bitRead(states,2));
      Serial.print(bitRead(states,1));
      Serial.println(bitRead(states,0));
      fFS = bitRead(states,3);
      sFS = bitRead(states,2);
      b1 = bitRead(states,1);
      b2 = bitRead(states,0);
      delay(100);
    }
  }
  else{
    f1();
  }
}
void f2(){
  j=0;
  k=0;
  delay(100);
  digitalWrite(ledPin1,LOW);
  digitalWrite(ledPin2,LOW);
  digitalWrite(ledPinE,LOW);
  byte states = getStates();
  if(i){
    Serial.print("F2  ");
    Serial.print(bitRead(states,3));
    Serial.print(bitRead(states,2));
    Serial.print(bitRead(states,1));
    Serial.println(bitRead(states,0));
  }
  fFS = bitRead(states,3);
  sFS = bitRead(states,2);
  b1 = bitRead(states,1);
  b2 = bitRead(states,0);
  myservo.write(pos);
  if((fFS==0)&&(sFS==1)&&(b1==1)&&(b2==0)){
    down();
  }
  else if((fFS==1)){
    while(1){
      error();
      i = 0;
      digitalWrite(ledPin1,LOW);
      digitalWrite(ledPin2,LOW);
      digitalWrite(ledPinE,HIGH);
      byte states = getStates();
      Serial.print("F2  ");
      Serial.print(bitRead(states,3));
      Serial.print(bitRead(states,2));
      Serial.print(bitRead(states,1));
      Serial.println(bitRead(states,0));
      fFS = bitRead(states,3);
      sFS = bitRead(states,2);
      b1 = bitRead(states,1);
      b2 = bitRead(states,0);
      delay(100);
      f2();
      
    }
  }
  else{
    i = 1;
    f2();
  }
}
void up(){
  digitalWrite(ledPin1,LOW);
  digitalWrite(ledPin2,HIGH);
  digitalWrite(ledPinE,LOW);
  byte states = getStates();
  Serial.print("UP  ");
  Serial.print(bitRead(states,3));
  Serial.print(bitRead(states,2));
  Serial.print(bitRead(states,1));
  Serial.println(bitRead(states,0));
  fFS = bitRead(states,3);
  sFS = bitRead(states,2);
  b1 = bitRead(states,1);
  b2 = bitRead(states,0);
  myservo.write(pos);
  if((fFS==0)&&(sFS==1)){
    f2();
  }
  else if(fFS==1){
    if(j==0){
      for (pos = 40;pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
      }
      j=1;
      up();
    }
    else{
      while(1){
        myservo.write(pos);
        byte states = getStates();
        Serial.print("UP  ");
        Serial.print(analogRead(sensorPin1));
        Serial.print("  ");
        Serial.print(bitRead(states,3));
        Serial.print(bitRead(states,2));
        Serial.print(bitRead(states,1));
        Serial.println(bitRead(states,0));
        fFS = bitRead(states,3);
        sFS = bitRead(states,2);
        b1 = bitRead(states,1);
        b2 = bitRead(states,0);
        error();
      }
    }
  }
  else{
    pos = pos + 1;
    delay(15);
    up();
  }
}
void down(){
  digitalWrite(ledPin1,HIGH);
  digitalWrite(ledPin2,LOW);
  digitalWrite(ledPinE,LOW);
  byte states = getStates();
  Serial.print("Down  ");
  Serial.print(bitRead(states,3));
  Serial.print(bitRead(states,2));
  Serial.print(bitRead(states,1));
  Serial.println(bitRead(states,0));
  fFS = bitRead(states,3);
  sFS = bitRead(states,2);
  b1 = bitRead(states,1);
  b2 = bitRead(states,0);
  myservo.write(pos);
  if((fFS==1)&&(sFS==0)){
    f1();
  }
  else if(sFS==1){
    if(k==0){
      for (pos = 140; pos >= 90; pos -= 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
      }
      k=1;
      down();
    }
    else{
      while(1){
        myservo.write(pos);
        byte states = getStates();
        Serial.print(analogRead(sensorPin2));
        Serial.print(" ");
        Serial.print("Down  ");
        Serial.print(bitRead(states,3));
        Serial.print(bitRead(states,2));
        Serial.print(bitRead(states,1));
        Serial.println(bitRead(states,0));
        fFS = bitRead(states,3);
        sFS = bitRead(states,2);
        b1 = bitRead(states,1);
        b2 = bitRead(states,0);
        error();
      }
    }
  }
  else{
    pos = pos - 1;
    delay(15);
    down();
  }
}
void error(){
  Serial.print("Error!!!  ");
  digitalWrite(ledPin1,LOW);
  digitalWrite(ledPin2,LOW);
  digitalWrite(ledPinE,HIGH);
}
byte getStates(){
  byte states;
  int b1 = digitalRead(uB);
  int b2 = digitalRead(dB);
  int sens1 = analogRead(sensorPin1);
  int sens2 = analogRead(sensorPin2);
  if(sens1>300){
    fFS = 1;
  }
  else{
    fFS = 0;
  }
  if(sens2>300){
    sFS = 1;
  }
  else{
    sFS = 0;
  }
  
  //states = 
  bitWrite(states,0,b2);
  bitWrite(states,1,b1);
  bitWrite(states,2,sFS);
  bitWrite(states,3,fFS);
  return states;
}

