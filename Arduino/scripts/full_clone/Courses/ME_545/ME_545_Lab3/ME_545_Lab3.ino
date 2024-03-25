const int ledPin13 = 13; // the number of the LED pin
const int ledPin12 = 12;
const int ledPin11 = 11;
const int ledPin10 = 10;

//unsigned char pins = {2,3,4,5,6,7,8};

const int ssda = 2;
const int ssdb = 3;
const int ssdc = 4;
const int ssdd = 5;
const int ssde = 6;
const int ssdf = 7;
const int ssdg = 8;

int Clock = LOW;
int val = 0;

long previousMillis = 0;

int a = LOW;
int b = LOW;
int c = LOW;
int d = LOW;
int e = LOW;
int f = LOW;
int g = LOW;

int i = 0;

int t = 0;
long interval = 1000;

void setup() {
  pinMode(ledPin13, OUTPUT);
  pinMode(ledPin12, OUTPUT);
  pinMode(ledPin11, OUTPUT);
  pinMode(ledPin10, OUTPUT);
  pinMode(ssda, OUTPUT);
  pinMode(ssdb, OUTPUT);
  pinMode(ssdc, OUTPUT);
  pinMode(ssdd, OUTPUT);
  pinMode(ssde, OUTPUT);
  pinMode(ssdf, OUTPUT);
  pinMode(ssdg, OUTPUT);
  
  digitalWrite(ssda,HIGH);
  digitalWrite(ssdb,HIGH);
  digitalWrite(ssdc,HIGH);
  digitalWrite(ssdd,HIGH);
  digitalWrite(ssde,HIGH);
  digitalWrite(ssdf,HIGH);
  digitalWrite(ssdg,HIGH);
  Serial.begin(9600);
}

void loop(){
  counter();
  //ssd();
}
void given(){
  a = Clock;
  if (a == HIGH){
    t = 1;
  }//
  if ((a == LOW)&&(t==1)){
    b = !b;
    t = 0;
  }
  digitalWrite(ledPin12,a);
  digitalWrite(ledPin13,b);
  if (millis() -previousMillis > interval) {
    previousMillis = millis();
    //
    if(Clock == LOW)
      Clock = HIGH;
    else
      Clock = LOW;
  }
}
void counter(){
  a = bitRead(val,3);
  b = bitRead(val,2);
  c = bitRead(val,1);
  d = bitRead(val,0);
  Serial.print(val);
  Serial.print("    ");
  Serial.print(a);
  Serial.print(b);
  Serial.print(c);
  Serial.println(d);
  digitalWrite(ledPin13,a);
  digitalWrite(ledPin12,b);
  digitalWrite(ledPin11,c);
  digitalWrite(ledPin10,d);
  if(val<15){
    val = val+1;
  }
  else{
    val=0;
  }
  delay(5000);
}
void ssd(){
  //0
  if(i==0){
    a = 0;b = 0; c = 0; d = 0; e = 0; f = 0; g = 1;
  }
  else if(i==1){
    a = 1;b = 0; c = 0; d = 1; e = 1; f = 1; g = 1;
  }
  else if(i==2){
    a = 0;b = 0; c = 1; d = 0; e = 0; f = 1; g = 0;
  }
  else if(i==3){
    a = 0;b = 0; c = 0; d = 0; e = 1; f = 1; g = 0;
  }
  else if(i==4){
    a = 1;b = 0; c = 0; d = 1; e = 1; f = 0; g = 0;
  }
  else if(i==5){
    a = 0;b = 1; c = 0; d = 0; e = 1; f = 0; g = 0;
  }
  else if(i==6){
    a = 0;b = 1; c = 0; d = 0; e = 0; f = 0; g = 0;
  }
  else if(i==7){
    a = 0;b = 0; c = 0; d = 1; e = 1; f = 1; g = 1;
  }
  else if(i==8){
    a = 0;b = 0; c = 0; d = 0; e = 0; f = 0; g = 0;
  }
  else if(i==9){
    a = 0;b = 0; c = 0; d = 0; e = 1; f = 0; g = 0;
  }
  Serial.print(i);
  Serial.print("    ");
  i=i+1;
  if(i==10){
    i = 0;
  }
  digitalWrite(ssda,a);
  digitalWrite(ssdb,b);
  digitalWrite(ssdc,c);
  digitalWrite(ssdd,d);
  digitalWrite(ssde,e);
  digitalWrite(ssdf,f);
  digitalWrite(ssdg,g);
  int pot = analogRead(0);
  float frac = pot/1023.;
  int dt = (pot/1023.)*1500 + 500;
  Serial.println(dt);
  delay(dt);
  
}


