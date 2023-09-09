#include <Servo.h>
Servo myservo;
int pos = 0;
int sensorPin1 = A2;
int sensorPin2 = A3;
int upstate = 0;
int upbutton = 4;
int downbutton 5;
int B1 = 0;
int B2 = 0;
int FFS = 0;
int SFS = 0;
byte control = 0;
int upled = 7;
int downled = 8;
int errorled = 9;



void setup() {
  // put your setup code here, to run once:
  myservo.attach(9);
  Serial.begin(9600);
  pinMode(sensorPin1,INPUT);
  pinMode(sensorPin2,INPUT);
  pinMode(upbutton,INPUT);
  pinMode(downbutton,INPUT);
  pinMode(upled,OUTPUT);
  pinMode(downled,OUTPUT);
  pinMode(errorled,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //up();
  //down();

  //create byte that has all four values in it FFS SFS B1 B2
B1=digitalRead(upbutton);
bitWrite(control,2,B1);       //sets third bit of control to value of button 1
B2=digitalRead(downbutton);
bitWrite(control,3,B2);       //sets fourth bit of control to value of button 2
FFS=analogRead(sensorPin1);
SFS=analogRead(sensorPin2);
if (FFS < 250){     //sets the first bit of control to 1 if 1st floor sensor is triggered (elevator present) and 0 if elevator is gone
   bitWrite(control,0,1);
}
else {
   bitWrite(control,0,0);
}
if (SFS < 250){     //sets the second bit of control to 1 if 2nd floor sensor is triggered (elevator present) and 0 if elevator is gone
   bitWrite(control,1,1);
}
else {
   bitWrite(control,1,0);
}


if (control=1010){
  up()
}
if (control=0101){
  down()
}
if (bitRead(control,0)&bitRead(control,1)|bitRead(control,2)&bitRead(control,3)=1)[
error()
}


void up(){
  int upState = analogRead(sensorPinU);
  Serial.println(upState);
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    digitalWrite(upled,HIGH);
  }
  digitalWrite(upled,LOW);
}


void down(){
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    digitalWrite(downled,LOW);
  }
digitalWrite(downled,LOW);
}


void error(){
  digitalWrite(errorled,HIGH);
  delay(1000);
  digitalWrite(errorled,LOW);
  
}
