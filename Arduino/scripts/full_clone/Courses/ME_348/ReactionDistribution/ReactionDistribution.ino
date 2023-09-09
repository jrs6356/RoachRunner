const byte react = 2;

int green = 6;
int red = 7;
int t;
int tstart;
int tend;
int dt;
int n = 1;
volatile byte k = HIGH;

void setup() {
  pinMode(green,OUTPUT);
  pinMode(red,OUTPUT);
  digitalWrite(red,HIGH);
  digitalWrite(green,LOW);
  attachInterrupt(digitalPinToInterrupt(react),reaction,RISING);
  Serial.begin(9600);
}

void loop() {
  if(n<31){
    stimulus();
  }
  else{
    digitalWrite(green,HIGH);
    digitalWrite(red,HIGH);
    delay(1500);
    digitalWrite(green,LOW);
    digitalWrite(red,LOW);
    while(1==1){
    }
  }
}
void reaction(){
  if(k==LOW){
    k=HIGH;
    tend = millis();
    dt = tend - tstart;
    Serial.print(n);
    Serial.print("\t");
    Serial.println(dt);
    digitalWrite(red,HIGH);
    digitalWrite(green,LOW);
    n=n+1;
    delay(50);
  }
}
void stimulus(){
  t = random(1000,5000);
  delay(t);
  digitalWrite(red,LOW);
  digitalWrite(green,HIGH);
  k = LOW;
  tstart = millis();
  while(k==LOW){
  }
}
