#include <RedBot.h>

RedBotMotors motors;
RedBotSensor IR = RedBotSensor(A0);
RedBotSensor IR2 = RedBotSensor(A1);

int dist;
int leftVel;
int rightVel;
int bodyVel = 200;
float r = 1.5;

void setup() {
  Serial.begin(9600);
}

void loop() {
  dist = IR.read();
  leftVel = bodyVel*(1-1/(2*r*dist^2));
  rightVel = 2*bodyVel - leftVel;
  motors.leftMotor(leftVel);
  motors.rightMotor(-rightVel);
  delay(10);
}
