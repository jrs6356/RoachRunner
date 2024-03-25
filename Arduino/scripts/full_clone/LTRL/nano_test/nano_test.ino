int led1 = 3;
int led2 = 4;


void setup() {
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
}

void loop() {
  digitalWrite(led1,HIGH);
  delayMicroseconds(1000);
  digitalWrite(led1,LOW);
  delayMicroseconds(500);
  digitalWrite(led2,HIGH);
  delayMicroseconds(1000);
  digitalWrite(led2,LOW);
  delayMicroseconds(500);  
}

