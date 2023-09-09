#define reset_button 3
#define enable_button 2


const int ledPin13 = 13; // the number of the LED pin
const int ledPin12 = 12;
const int ledPin11 = 11;
const int ledPin10 = 10;

//unsigned char pins = {2,3,4,5,6,7,8};

const int ssda = 4;
const int ssdb = 5;
const int ssdc = 6;
const int ssdd = 7;
const int ssde = 8;
const int ssdf = 9;
const int ssdg = 10;

const int ssda_tens = 11;
const int ssdb_tens = 12;
const int ssdc_tens = 13;
const int ssdd_tens = A2;
const int ssde_tens = A3;
const int ssdf_tens = A4;
const int ssdg_tens = A5;

const int opt_int = 0;
int opt_val = 0;


int Clock = LOW;
int val = 0;

long previousMillis = 0;

int a = 0;
int b = 0;
int c = 0;
int d = 0;
int e = 0;
int f = 0;
int g = 1;
int a2 = 0;
int b2 = 0;
int c2 = 0;
int d2 = 0;
int e2 = 0;
int f2 = 0;
int g2 = 1;
 
int i = 0;
int j = 1;
int k = 1;
int m = 0;
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
  pinMode(ssda_tens, OUTPUT);
  pinMode(ssdb_tens, OUTPUT);
  pinMode(ssdc_tens, OUTPUT);
  pinMode(ssdd_tens, OUTPUT);
  pinMode(ssde_tens, OUTPUT);
  pinMode(ssdf_tens, OUTPUT);
  pinMode(ssdg_tens, OUTPUT);
  pinMode(reset_button, INPUT);
  pinMode(enable_button, INPUT);
  pinMode(opt_int,INPUT);
    
  digitalWrite(ssda,a);
  digitalWrite(ssdb,b);
  digitalWrite(ssdc,c);
  digitalWrite(ssdd,d);
  digitalWrite(ssde,e);
  digitalWrite(ssdf,f);
  digitalWrite(ssdg,g);  
  digitalWrite(ssda_tens,a2);
  digitalWrite(ssdb_tens,b2);
  digitalWrite(ssdc_tens,c2);
  digitalWrite(ssdd_tens,d2);
  digitalWrite(ssde_tens,e2);
  digitalWrite(ssdf_tens,f2);
  digitalWrite(ssdg_tens,g2);
  Serial.begin(38400);

  attachInterrupt(digitalPinToInterrupt(reset_button), reset, FALLING);
  attachInterrupt(digitalPinToInterrupt(enable_button), onoff, FALLING);
  //attachInterrupt(reset_button, reset, RISING);
  //attachInterrupt(enable_button, onoff, RISING);
}

void loop(){
  //counter();
  //ssd();
  q3();
  delay(10);
}


void reset(){
  //reset counter to 00
  i=0;
  k=1; 
  a=0;b=0;c=0;d=0;e=0;f=0;g=1;
  a2=0;b2=0;c2=0;d2=0;e2=0;f2=0;g2=1;
  Serial.println("");
  Serial.println("Reset");
}
void onoff(){
  j=!j;
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
void q3(){
  if(j==1){
    int op_val=analogRead(opt_int);
    Serial.println(op_val);
    if(op_val<700){
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
      if(i==10){
        i = 0;
        if(i==0){
          a = 0;b = 0; c = 0; d = 0; e = 0; f = 0; g = 1;
        }
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
      Serial.print(k-1);
      Serial.println(i);
      i=i+1;
    }
    while((op_val<700)&&(j==1)){
      digitalWrite(ssda,a);
      digitalWrite(ssdb,b);
      digitalWrite(ssdc,c);
      digitalWrite(ssdd,d);
      digitalWrite(ssde,e);
      digitalWrite(ssdf,f);
      digitalWrite(ssdg,g);
      digitalWrite(ssda_tens,a2);
      digitalWrite(ssdb_tens,b2);
      digitalWrite(ssdc_tens,c2);
      digitalWrite(ssdd_tens,d2);
      digitalWrite(ssde_tens,e2);
      digitalWrite(ssdf_tens,f2);
      digitalWrite(ssdg_tens,g2);
      op_val=analogRead(opt_int);
    }
    digitalWrite(ssda,a);
    digitalWrite(ssdb,b);
    digitalWrite(ssdc,c);
    digitalWrite(ssdd,d);
    digitalWrite(ssde,e);
    digitalWrite(ssdf,f);
    digitalWrite(ssdg,g);
    digitalWrite(ssda_tens,a2);
    digitalWrite(ssdb_tens,b2);
    digitalWrite(ssdc_tens,c2);
    digitalWrite(ssdd_tens,d2);
    digitalWrite(ssde_tens,e2);
    digitalWrite(ssdf_tens,f2);
    digitalWrite(ssdg_tens,g2);
  }
  while(j==0){
    digitalWrite(ssda,1);
    digitalWrite(ssdb,1);
    digitalWrite(ssdc,1);
    digitalWrite(ssdd,1);
    digitalWrite(ssde,1);
    digitalWrite(ssdf,1);
    digitalWrite(ssdg,1);
    digitalWrite(ssda_tens,1);
    digitalWrite(ssdb_tens,1);
    digitalWrite(ssdc_tens,1);
    digitalWrite(ssdd_tens,1);
    digitalWrite(ssde_tens,1);
    digitalWrite(ssdf_tens,1);
    digitalWrite(ssdg_tens,1);
  }
}
