/***********************************************************************
 * Exp9_SerialDrive -- RedBot Experiment 9
 * 
 * The first step to controlling the RedBot remotely is to first drive it 
 * from the Serial Monitor in a tethered setup. 
 *
 * Hardware setup:
 * After uploading this sketch, keep the RedBot tethered to your computer with 
 * the USB cable. Open up the Seral Monitor to send commands to the RedBot to 
 * drive. 
 * 
 * This sketch was written by SparkFun Electronics, with lots of help from 
 * the Arduino community. This code is completely free for any use.
 * 
 * 15 Dec 2014 B. Huang 
 * 
 * This experiment was inspired by Paul Kassebaum at Mathworks, who made
 * one of the very first non-SparkFun demo projects and brought it to the
 * 2013 Open Hardware Summit in Boston. Thanks Paul!
 ***********************************************************************/

#include <RedBot.h>   // include redbot library

RedBotMotors motors;    // declare motors
RedBotSensor IR = RedBotSensor(A0);   // declare IR sensor
int leftPower;  // left motor speed
int rightPower; // right motor speed
int leftSig = 1;  // identifier for left motor
int rightSig = 2;   // identifier for right motor
int Sig;  // current data string
int dist;   // IR distance
int data;  // variable for holding incoming data from PC to Arduino

void setup(void)
{
  Serial.begin(9600);
  Serial.print("Enter in left and right motor power values and click [Send]."); 
  Serial.print("Separate values with a space or non-numeric character.");
  Serial.println();
  Serial.print("Positive values spin the motor CW, and negative values spin the motor CCW.");
}

void loop(void)
{
  IRread();   // check distance
  // if there is data coming in on the Serial monitor, do something with it.
  if(Serial.available() > 0)
  {
    Sig = Serial.parseInt();  // check for motor identifier
    //Serial.println(Sig);
    if(Sig==leftSig){   // left motor data identified
      leftPower = Serial.parseInt();  // read in the next numeric value
      leftPower = constrain(leftPower, -255, 255);  // constrain the data to -255 to +255
    }
    if(Sig==rightSig){    // right motor data identified
      rightPower = Serial.parseInt();   // read in the next numeric value
      rightPower = constrain(rightPower, -255, 255);  // constrain the data to -255 to +255
    }

    motors.leftMotor(leftPower);    // output left motor
    motors.rightMotor(rightPower);  // output right motor
  }  
}
void IRread(){              // IR dist check
  dist = IR.read()-400;     // trim readings into workable range
  Serial.println(dist);     // print distance to serial monitor
    if(120<dist){           // stopping distance detected
    motors.leftMotor(-100); // slow motor down for a moment
    motors.rightMotor(100); // ^^
    delay(100);             // moment
    motors.leftMotor(-75);  // slow motor down further for another moment
    motors.rightMotor(75);  // ^^
    delay(100);             // another moment
    motors.leftMotor(100);  // reverse direction to completely stop moving forward
    motors.rightMotor(-100);//^^
    delay(150);             // final moment
    motors.leftMotor(0);    // kill motor
    motors.rightMotor(0);   // kill motor
    while(1);               // lock everything up
  }
}

