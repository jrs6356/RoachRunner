const int ssda = 4;
const int ssdb = 5;
const int ssdc = 6;
const int ssdd = 7;
const int ssde = 8;
const int ssdf = 9;
const int ssdg = 10;

int a = 0;
int b = 0;
int c = 0;
int d = 0;
int e = 0;
int f = 0;
int g = 1;

int i = 0;

void setup() {
  pinMode(ssda, OUTPUT);
  pinMode(ssdb, OUTPUT);
  pinMode(ssdc, OUTPUT);
  pinMode(ssdd, OUTPUT);
  pinMode(ssde, OUTPUT);
  pinMode(ssdf, OUTPUT);
  pinMode(ssdg, OUTPUT);
    
  digitalWrite(ssda,a);
  digitalWrite(ssdb,b);
  digitalWrite(ssdc,c);
  digitalWrite(ssdd,d);
  digitalWrite(ssde,e);
  digitalWrite(ssdf,f);
  digitalWrite(ssdg,g);  
}

void loop(){
  ssd();
}

void ssd(){
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
  //Serial.print(i);
  //Serial.print("    ");
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
  //Serial.println(dt);
  delay(dt);
}

