int btnA = 5;
int btnB = 6;
int btnG = 4;
int A = 32;
int B = 30;
int G = 3;
volatile bool AState = false;
volatile bool BState = false;
volatile bool GState = true;
volatile bool doA = false;
volatile bool doB = false;
volatile bool doG = false;
void setup() {
  Serial.begin(115200);
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(btnA, INPUT_PULLUP);
  pinMode(btnB, INPUT_PULLUP);
  pinMode(btnG, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(btnA), flipA, RISING);
  attachInterrupt(digitalPinToInterrupt(btnB), flipB, RISING);
  attachInterrupt(digitalPinToInterrupt(btnG), flipG, RISING);
  digitalWrite(G, HIGH);
  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
}

void loop() {
  if(doA){
    AState=!AState;
    Serial.print("A:  ");
    Serial.println(AState);
    digitalWrite(A, AState);
    doA=false;
  }
  if(doB){
    BState=!BState;
    Serial.print("B:  ");
    Serial.println(BState);
    digitalWrite(B, BState);
    doB=false;
  }
  if(doG){
    GState=false;
    Serial.print("G:  ");
    Serial.println(GState);
    digitalWrite(G, GState);
    delay(5000);
    GState = true;
    AState = false;
    BState = false;
    //digitalWrite(A,LOW);
    //digitalWrite(B,LOW);
    //delay(100);
    digitalWrite(G, HIGH);
    doG=false;
  }
  
  delay(1000);
}

void flipA(){
  if(!doA){
    doA=true;
  }
}

void flipB(){
  if(!doB){
    doB=true;
  }
}

void flipG(){
  if(!doG){
    doG=true;
  }
}
