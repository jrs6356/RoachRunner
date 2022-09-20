//pin 1 only outputs HIGH
#define button 2

int in1 = 3;
int in2 = 4;
int in3 = 5;
int in4 = 6;
int in5 = 7;
int in6 = 8;

int en = 11;

int pot = A0;
int potVal;
int rotSpeed = 0;
int stage = 1;
int a1; int a2;
int b1; int b2;
int c1; int c2;
int a;
int b;
int c;
int cycle = 1;

void setup() {
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  pinMode(in5,OUTPUT);
  pinMode(in6,OUTPUT);
  pinMode(en,OUTPUT);
  pinMode(pot,INPUT);

  attachInterrupt(digitalPinToInterrupt(button), buttonMag, FALLING);
  Serial.begin(9600);
  switchSet();
}

void loop(){
  wye();
}

void buttonState(){
  //state = !state;
  //Serial.println("Flip");
}

void buttonMag(){
  // 0:n/c , 1:low , 2:high
  stage = stage + 1;
  if(stage==7){
    stage = 1;
  }
  switchSet();
  delay(100);
}

void wye(){
  digitalWrite(in1,a1);
  digitalWrite(in2,a2);
  digitalWrite(in3,b1);
  digitalWrite(in4,b2);
  digitalWrite(in5,c1);
  digitalWrite(in6,c2);
  
  potVal = analogRead(pot);
  rotSpeed = map(potVal, 0, 1023, 0, 255);
  analogWrite(en,rotSpeed);
  if(cycle==1){
    stage = stage + 1;
    if(stage==7){
      stage = 1;
    }
  switchSet();
  delay(750);
  }
}

void dink(){
  //digitalWrite(tran1,HIGH);
}

void bink(){
  //digitalWrite(tran1,HIGH);
  //Serial.println("On");
  //delay(1000);
  //digitalWrite(tran1,LOW);
  //Serial.println("Off");
  //delay(1000);
}

void pink(){
  if(a==1){
    a1=0;a2=1;
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
  }
  else{
    a1=1;a2=0;
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
  }
  if(b==1){
    b1=0;b2=1;
    digitalWrite(in3,LOW);
    digitalWrite(in4,HIGH);
  }
  else{
    b1=1;b2=0;
    digitalWrite(in3,HIGH);
    digitalWrite(in4,LOW);
  }
  if(c==1){
    c1=0;c2=1;
    digitalWrite(in5,LOW);
    digitalWrite(in6,HIGH);
  }
  else{
    c1=1;c2=0;
    digitalWrite(in5,HIGH);
    digitalWrite(in6,LOW);
  }
  potVal = analogRead(pot);
  rotSpeed = map(potVal, 0, 1023, 0, 255);
  analogWrite(en,rotSpeed);
  //Serial.print("Pot:  ");
  //Serial.println(potVal);
  //Serial.print("PWM:  ");
  Serial.print(rotSpeed);Serial.print("\t");
  Serial.print(a1);Serial.print(a2);
  Serial.print(b1);Serial.print(b2);
  Serial.print(c1);Serial.println(c2);
  //Serial.println("");
  delay(500);
}
void switchSet(){
  if(stage==1){
    a=2;
    b=1;
    c=0;
  }
  else if(stage==2){
    a=2;
    b=0;
    c=1;
  }
  else if(stage==3){
    a=0;
    b=2;
    c=1;
  }
  else if(stage==4){
    a=1;
    b=2;
    c=0;
  }
  else if(stage==5){
    a=1;
    b=0;
    c=2;
  }
  else{
    a=0;
    b=1;
    c=2;
  }
  if(a==2){
    a1=1;a2=0;
  }
  else if(a==1){
    a1=0;a2=1;
  }
  else{
    a1=0;a2=0;
  }
  if(b==2){
    b1=1;b2=0;
  }
  else if(b==1){
    b1=0;b2=1;
  }
  else{
    b1=0;b2=0;
  }
  if(c==2){
    c1=1;c2=0;
  }
  else if(c==1){
    c1=0;c2=1;
  }
  else{
    c1=0;c2=0;
  }
  Serial.print("Stage: ");Serial.print(stage);Serial.print("  ");
  Serial.print("abc: ");Serial.print(a);Serial.print(b);Serial.print(c);Serial.print("  ");
  Serial.print("Sigs: ");Serial.print(a1);Serial.print(a2);Serial.print(b1);Serial.print(b2);Serial.print(c1);Serial.println(c2);
}
