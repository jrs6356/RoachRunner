int sensorPin = 2;
double val = 0;
int i = 1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(i<25){
    val = analogRead(sensorPin);
    Serial.println(val);
    delay(100);
    i = i+1;
  }
}
