String gps;

void setup() {
  Serial.begin(115200);
}
void loop() {
  Serial.write(Serial.read());
  //gps = Serial.read();
  //Serial.println(gps);
  //delay(100);
}
