const int dB = 2;
int b1 = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(dB, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
   button();
   delay(100);
}
void button(){
  Serial.println(digitalRead(dB));
}
