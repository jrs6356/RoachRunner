#include <RedBot.h>
#include "notes.h"

RedBotMotors motors;
int leftSpeed;   // variable used to store the leftMotor speed
int rightSpeed;  // variable used to store the rightMotor speed

RedBotBumper lBumper = RedBotBumper(3);  // initialzes bumper object on pin 3
RedBotBumper rBumper = RedBotBumper(11); // initialzes bumper object on pin 11
RedBotSensor left = RedBotSensor(A3);   // initialize a left sensor object on A3
RedBotSensor center = RedBotSensor(A6); // initialize a center sensor object on A6
RedBotSensor right = RedBotSensor(A7);  // initialize a right sensor object on A7

const int buzzerPin = 9;
const int buttonPin = 12;

#define LINETHRESHOLD 800
#define SPEED 100  // sets the nominal speed. Set to any number from 0 - 255.
#define SHIFT 20

int lBumperState;  // state variable to store the bumper value
int rBumperState;  // state variable to store the bumper value

void setup()
{
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP); // configures the button as an INPUT
  // INPUT_PULLUP defaults it to HIGH.
  pinMode(buzzerPin, OUTPUT);  // configures the buzzerPin as an OUTPUT
  while(digitalRead(buttonPin)==HIGH)
  {
    Serial.println("Waiting...");
    delay(100);
  }
  startSound();
}

void loop(){
  totDrive();
}

void totDrive(){
  //motors.drive(255);
  lineFollow();
  bumperCheck();
}
void bumperCheck(){
  lBumperState = lBumper.read();  // default INPUT state is HIGH, it is LOW when bumped
  rBumperState = rBumper.read();  // default INPUT state is HIGH, it is LOW when bumped

    if (lBumperState == LOW) // left side is bumped/
  { 
    smash();
    reverse();    // backs up
    turnRight();  // turns
  }

  if (rBumperState == LOW) // right side is bumped/
  { 
    smash();
    reverse();   // backs up
    turnLeft();  // turns
  }
}
void lineFollow(){
  if(center.read() > LINETHRESHOLD)
  {
    leftSpeed = -SPEED; 
    rightSpeed = SPEED;
  }
  
  // if the line is under the right sensor, adjust relative speeds to turn to the right
  else if(right.read() > LINETHRESHOLD)
  {
    leftSpeed = -(SPEED + SHIFT);
    rightSpeed = SPEED - SHIFT;
  }

  // if the line is under the left sensor, adjust relative speeds to turn to the left
  else if(left.read() > LINETHRESHOLD)
  {
    leftSpeed = -(SPEED - SHIFT);
    rightSpeed = SPEED + SHIFT;
  }
  
  // if all sensors are on black or up in the air, stop the motors.
  // otherwise, run motors given the control speeds above.
  if((left.read() > LINETHRESHOLD) && (center.read() > LINETHRESHOLD) && (right.read() > LINETHRESHOLD) )
  {
    motors.stop();
  }
  else
  {
    motors.leftMotor(leftSpeed);
    motors.rightMotor(rightSpeed);
    
  }
  delay(0);  // add a delay to decrease sensitivity.
}
void smash(){
  Serial.println("Whoops!");
  playNote(noteA4, QN);
  playNote(noteG4, QN);
  playNote(noteC4, QN);
}
void startSound(){
  Serial.println("Go!");
  playNote(noteA4, WN);
  delay(100);
  playNote(noteA4, WN);
  delay(100);
  playNote(noteA5, WN);
}
void playNote(int note, int duration)
{
  tone(buzzerPin, note, duration);
  delay(duration);
}
// reverse() function -- backs up at full power
void reverse()
{
  motors.drive(-255);
  delay(500);
  motors.brake();
  delay(100);  // short delay to let robot fully stop
}

// turnRight() function -- turns RedBot to the Right
void turnRight()
{
  motors.leftMotor(-150);  // spin CCW
  motors.rightMotor(-150); // spin CCW
  delay(500);
  motors.brake();
  delay(100);  // short delay to let robot fully stop
}

// turnRight() function -- turns RedBot to the Left
void turnLeft()
{
  motors.leftMotor(+150);  // spin CW
  motors.rightMotor(+150); // spin CW
  delay(500);
  motors.brake();
  delay(100);  // short delay to let robot fully stop
}
