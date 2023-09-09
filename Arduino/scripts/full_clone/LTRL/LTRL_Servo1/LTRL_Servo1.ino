
#include <Stepper.h>

#define STEPS 150

//Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
//long duration;
//long distance;
int trigPin = 5;
int echoPin = 6;

Stepper stepper(STEPS, 3, 4);

void setup() {
  Serial.begin(9600);
  //myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  stepper.setSpeed(60);
}

void loop() {
  motor();
}
void motor(){
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  long duration = pulseIn(echoPin,HIGH);
  long distance = (duration/2)/29.1;
  Serial.println(distance);
  stepper.step(STEPS);
  stepper.step(-STEPS);
}
