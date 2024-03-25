byte cs = 2;
byte e = 3;

void setup() {
  Serial.begin(115200);
  while(!Serial){}

  pinMode(cs, OUTPUT);
  pinMode(e, OUTPUT);

  digitalWrite(cs, HIGH);
  digitalWrite(e, HIGH);

}

void loop() {
  // put your main code here, to run repeatedly:

}
