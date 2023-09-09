int b1 = 0;

void setup() {
  pinMode(b1,INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(analogRead(b1));
  delay(100);
}
