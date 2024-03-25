//#define opInt 2

//volatile int opIntVal

const int ledPin13 = 13; // the number of the LED pin
const int ledPin12 = 12;
const int ledPin11 = 11;
const int ledPin10 = 10;

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
int a2 = LOW;
int b2 = LOW;
int c2 = LOW;
int d2 = LOW;
int e2 = LOW;
int f2 = LOW;
int g2 = LOW;

int i = 0;
int k = 0;

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
  //counter();
  //ssd();
  q3();
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
void q3(){
  //if(opint == 1){
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
    i=i+1;
    if(i==10){
      i = 0;
      if(k==0){
        a2 = 0;b2 = 0; c2 = 0; d2 = 0; e2 = 0; f2 = 0; g2 = 1;
      }
      else if(k==1){
        a2 = 1;b2 = 0; c2 = 0; d2 = 1; e2 = 1; f2 = 1; g2 = 1;
      }
      else if(k==2){
        a2 = 0;b2 = 0; c2 = 1; d2 = 0; e2 = 0; f2 = 1; g2 = 0;
      }
      else if(k==3){
        a2 = 0;b2 = 0; c2 = 0; d2 = 0; e2 = 1; f2 = 1; g2 = 0;
      }
      else if(k==4){
        a2 = 1;b2 = 0; c2 = 0; d2 = 1; e2 = 1; f2 = 0; g2 = 0;
      }
      else if(k==5){
        a2 = 0;b2 = 1; c2 = 0; d2 = 0; e2 = 1; f2 = 0; g2 = 0;
      }
      else if(k==6){
        a2 = 0;b2 = 1; c2 = 0; d2 = 0; e2 = 0; f2 = 0; g2 = 0;
      }
      else if(k==7){
        a2 = 0;b2 = 0; c2 = 0; d2 = 1; e2 = 1; f2 = 1; g2 = 1;
      }
      else if(k==8){
        a2 = 0;b2 = 0; c2 = 0; d2 = 0; e2 = 0; f2 = 0; g2 = 0;
      }
      else if(k==9){
        a2 = 0;b2 = 0; c2 = 0; d2 = 0; e2 = 1; f2 = 0; g2 = 0;
      }
      k=k+1;
      if(k==10){
        k = 0;
      }
    }
    Serial.print(k);
    Serial.println(i);
  digitalWrite(ssda,a);
  digitalWrite(ssdb,b);
  digitalWrite(ssdc,c);
  digitalWrite(ssdd,d);
  digitalWrite(ssde,e);
  digitalWrite(ssdf,f);
  digitalWrite(ssdg,g);
  //digitalWrite(ssda,a2);
  //digitalWrite(ssdb,b2);
  //digitalWrite(ssdc,c2);
  //digitalWrite(ssdd,d2);
  //digitalWrite(ssde,e2);
  //digitalWrite(ssdf,f2);
  //digitalWrite(ssdg,g2);
  delay(1000);
}

