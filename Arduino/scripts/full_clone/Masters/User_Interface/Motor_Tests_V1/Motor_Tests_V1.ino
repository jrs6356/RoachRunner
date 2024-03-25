#include <Servo.h>

byte esc_tx = 8;
byte esc_pwm = 9;
byte pot = A0;

int val = 1500;
int potVal = 0;

Servo servo;

void setup() {
  Serial.begin(250000);
  servo.attach(esc_pwm);
  servo.writeMicroseconds(1500);
  Serial.println("\t\t Plug in now\n");
  delay(7000);
  Serial.println("\t\t Starting\n");

}

void loop() {
  potVal = analogRead(pot);
  val = map(potVal, 0, 1023, 1100, 1900);
  servo.writeMicroseconds(val);
  Serial.println(val);

}
