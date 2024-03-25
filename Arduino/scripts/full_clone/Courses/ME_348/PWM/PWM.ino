
int out = 11;
int x = 0;
int fadeAmount = 5;
int t ;

void setup() {
  // put your setup code here, to run once:

Serial.begin(9600);
pinMode(out,OUTPUT);
}

void loop() {
  analogWrite(out, x);
  x = x + fadeAmount;
  if (x <= 0 || x >= 255) {
    fadeAmount = -fadeAmount;
  }
  delay(30);
  int sensorValue = analogRead(A0);
  float voltage = sensorValue * (5.0 / 1023.0);
  Serial.println(voltage);
}
