#include <RedBot.h>
RedBotSensor left = RedBotSensor(A0);   // initialize a left sensor object on A0

double leftThreshold = 240.0;    // threshold value for the left facing sensor
double moveSpeed = 180.0;        // nominal speed for the motors
int LRead;                  // variable for left sensor input
double LError;              // Error of the left sensor reading and it's respective threshhold
double LGain;
double correction = 80.0;

RedBotMotors motors;
double leftSpeed;   // variable used to store the leftMotor speed
double rightSpeed;  // variable used to store the rightMotor speed

void setup()
{
	Serial.begin(9600);
}

void loop()
{
LRead = left.read();

if (10 <= abs(LRead-leftThreshold)) {
  if (leftThreshold < LRead) {          // Indicates we are too close to the wall and must turn right
    LError = ((LRead-leftThreshold)/leftThreshold);
    LGain = LError * correction;
    Serial.println("Close");
    Serial.println(LGain);
    rightSpeed = moveSpeed - LGain;
    leftSpeed = moveSpeed + LGain;
  }
  else {                                // Indicates we are too far from the wall and must turn left
    LError = abs((LRead - leftThreshold)/leftThreshold);
    LGain = LError * correction;
    Serial.println("Far");
    Serial.println(LGain);
    rightSpeed = moveSpeed + LGain;
    leftSpeed = moveSpeed - LGain;
  }
}

else {
  Serial.println("Straight");
  rightSpeed = moveSpeed;
  leftSpeed = moveSpeed;
}

if (255 < rightSpeed){
  rightSpeed = 255; 
}

if (255 < leftSpeed){
  leftSpeed = 255; 
}

motors.leftMotor(leftSpeed);
motors.rightMotor(-rightSpeed);
delay(50);
//motors.leftMotor(moveSpeed);
//motors.rightMotor(-moveSpeed);
//delay(100);

}
