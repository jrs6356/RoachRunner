/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int loops = 1;
int led = 10;
int brightness = 0;
int fadeAmount = 5;

void setup() {
  myservo.attach(2);  // attaches the servo on pin 9 to the servo object
  pinMode(led,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  float t0 = millis();
  Serial.print("Start time (millis): ");
  Serial.println(t0);
  for(loops = 1; loops<=15; loops+=1){
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(5);                       // waits 15ms for the servo to reach the position
      //if((45==pos)||(90==pos)||(135==pos)){
        //delay(150);
      //}
    }
    for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(5);                       // waits 15ms for the servo to reach the position
      //if((45==pos)||(90==pos)||(135==pos)){
        //delay(150);
      //}
    }
    Serial.print("Loop number: ");
    Serial.println(loops);
  }
  float t1 = millis();
  float dt = t1-t0;
  float servoSpeed = 2000*loops/(dt);
  Serial.print("End time (millis): ");
  Serial.println(dt);
  Serial.print("Servo speed is: ");
  Serial.print(servoSpeed);
  Serial.println(" sweeps/sec");
  analogWrite(led,brightness);
  brightness += fadeAmount;
  if (brightness <= 0 || brightness >=255){
    fadeAmount = -fadeAmount;
  }
}

