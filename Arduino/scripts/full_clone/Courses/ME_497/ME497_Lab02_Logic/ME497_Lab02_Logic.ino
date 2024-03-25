const byte A = 2;
const byte B = 3;
const byte Y = 4;

void setup() {
  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  pinMode(4,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(A), TTL, HIGH);
  attachInterrupt(digitalPinToInterrupt(B), TTL, HIGH);
}

void loop() {
  //logic();
}
void logic(){
  int Ai = digitalRead(A);
  int Bi = digitalRead(B);
  if(Ai|Bi){
    digitalWrite(Y,HIGH);
  }
  else{
    digitalWrite(Y,LOW);
  }
}
void TTL(){
  digitalWrite(Y,HIGH);
}

