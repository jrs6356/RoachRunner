#include <PID_v1.h>

const int encoderPinA = 2; // Encoder channel A input pin
const int encoderPinB = 3; // Encoder channel B input pin
const int pwmPin = 9; // Motor PWM output pin

volatile int encoderCount = 0; // Keeps track of encoder counts
int prevEncoderCount = 0; // Previous encoder count for calculating speed
unsigned long prevTime = 0; // Previous time for calculating speed
double desiredSpeed = 100; // Desired motor speed (in counts per second)
double inputSpeed = 0; // Measured motor speed (in counts per second)
double outputPWM = 0; // Motor PWM output
double Kp = 1, Ki = 0.1, Kd = 0.01; // PID gains
PID pid(&inputSpeed, &outputPWM, &desiredSpeed, Kp, Ki, Kd, DIRECT);

void setup() {
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoderPinA), countEncoder, CHANGE);
  prevTime = micros();
  pid.SetMode(AUTOMATIC);
}

void loop() {
  unsigned long currTime = micros();
  unsigned long deltaTime = currTime - prevTime;

  // Calculate motor speed
  int currEncoderCount = encoderCount;
  inputSpeed = (currEncoderCount - prevEncoderCount) * 1000000.0 / deltaTime;
  prevEncoderCount = currEncoderCount;
  prevTime = currTime;

  // Compute PID output
  pid.Compute();

  // Set motor PWM output
  int pwmValue = map(constrain(outputPWM, 0, 100), 0, 100, 255, 0); // Convert PID output to PWM value (0-100%)
  analogWrite(pwmPin, pwmValue);
}

void countEncoder() {
  // Update encoder count based on quadrature encoding
  if (digitalRead(encoderPinA) == digitalRead(encoderPinB)) {
    encoderCount++;
  } else {
    encoderCount--;
  }
}
