const int ledPin13 = 13; // the number of the LED pin
const int ledPin12 = 12;
const int ledPin11 = 11;
const int ledPin10 = 10;

int val = 0;

int a = 0;
int b = 0;
int c = 0;
int d = 0;

void setup() {
  pinMode(ledPin13, OUTPUT);
  pinMode(ledPin12, OUTPUT);
  pinMode(ledPin11, OUTPUT);
  pinMode(ledPin10, OUTPUT);
}

void loop(){
  counter();
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

