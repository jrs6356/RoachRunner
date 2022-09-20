#include <math.h>

const byte inc = 2;
const byte cs = 3;
const byte ud = 4;
const byte up = 5;
const byte down = 6;
const byte ledUp = 7;
const byte ledDown = 8;
int v0 = A0;

volatile bool bounceUp = false;
volatile bool bounceDown = false;

float Vc = 4.586;
float Vref = 3.3;
float V0 = 0.0;
float V0_des = 1.7;
float V0_error = 0.0;
float V0_sum = 0.0;
float V0_ave = 0.0;
int V0_b = 0;
int V0_b_old = 0;
int V0_b_del = 0;
float R1 = 984.0;
float Rpot = 0.0;
float Rp_max = 0.0;
float Rp_sum = 0.0;
float Rw_meas = 0.0;

float Rp_max_calc = 892.7;
float Rw_calc = 25.9;

float res = 4096;
float k = 1;
float j = 1;
int td = 10;

int k_des = 0;

void setup() {
  Serial.begin(115200);
  k_des = round(99*((R1*V0_des/(Vc-V0_des)) - Rw_calc)/(Rp_max_calc - Rw_calc))-1;
  Serial.println(k_des);
  //while(1){}
  analogReadResolution(12);
  
  pinMode(inc, OUTPUT);
  pinMode(cs, OUTPUT);
  pinMode(ud, OUTPUT);
  pinMode(up, INPUT);//_PULLUP);
  pinMode(down, INPUT);//_PULLUP);
  pinMode(ledUp, OUTPUT);
  pinMode(ledDown, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(up), up_btn, RISING);
  attachInterrupt(digitalPinToInterrupt(down), down_btn, RISING);
  
  digitalWrite(cs, HIGH);
  digitalWrite(inc, HIGH);
  digitalWrite(ud, LOW);
  digitalWrite(ledUp, LOW);
  digitalWrite(ledDown, LOW);
  
  Serial.println("\n\n\n-------------------------------------------------\nNew Digipot");
  V0_b = analogRead(v0);
  V0_b_old = V0_b;
  V0 = Vref*(V0_b/res);
  //Serial.print("Vout_b = ");Serial.println(V0_b);
  //Serial.print("Vout = ");Serial.println(V0);
}

void loop() {
  btnStep();
  //resSweep();
  //resMin();
  //resMax();
  //voltSet();
}

void voltSet(){
  digitalWrite(ledUp, HIGH);
  digitalWrite(cs, LOW);
  digitalWrite(ud, HIGH);
  delay(1);
  for(int i=1; i<k_des; i++){
    digitalWrite(inc, LOW);
    delay(td);
    digitalWrite(inc, HIGH);
    delay(td);
    V0_b = analogRead(v0);
    V0_b_del = V0_b - V0_b_old;
    V0_b_old = V0_b;
    V0 = Vref*(V0_b/res);
    Serial.println(V0);
    Rpot = R1*V0/(Vc - V0);
    //Serial.println(Rpot);
  }
  digitalWrite(cs, HIGH);
  digitalWrite(ledUp, LOW);
  while(1){
    V0_b = analogRead(v0);
    V0_b_del = V0_b - V0_b_old;
    V0_b_old = V0_b;
    V0 = Vref*(V0_b/res);
    V0_sum = V0_sum + V0;
    V0_ave = V0_sum/j;
    Serial.println(V0_ave,3);
    Rpot = R1*V0/(Vc - V0);
    Rp_sum = Rpot + Rp_sum;
    Rp_max = Rp_sum/j;
    //Serial.println(Rp_max);
    j++;
    delay(2*td);
  }
}

void resMin(){
  digitalWrite(ledDown, HIGH);
  digitalWrite(cs, LOW);
  digitalWrite(ud, LOW);
  delay(1);
  for(int i=1; i<150; i++){
    digitalWrite(inc, LOW);
    delay(td);
    digitalWrite(inc, HIGH);
    delay(td);
    V0_b = analogRead(v0);
    V0_b_del = V0_b - V0_b_old;
    V0_b_old = V0_b;
    V0 = Vref*(V0_b/res);
    Serial.println(V0);
    Rpot = R1*V0/(Vc - V0);
    //Serial.println(Rpot);
  }
  digitalWrite(ledDown, LOW);
  
  while(1){
    V0_b = analogRead(v0);
    V0_b_del = V0_b - V0_b_old;
    V0_b_old = V0_b;
    V0 = Vref*(V0_b/res);
    V0_sum = V0_sum + V0;
    V0_ave = V0_sum/j;
    //Serial.println(V0_ave,3);
    Rpot = R1*V0/(Vc - V0);
    Rp_sum = Rpot + Rp_sum;
    Rw_meas = Rp_sum/j;
    Serial.println(Rw_meas);
    j++;
    delay(2*td);
  }
  
}

void resMax(){
  digitalWrite(ledUp, HIGH);
  digitalWrite(cs, LOW);
  digitalWrite(ud, HIGH);
  delay(1);
  for(int i=1; i<150; i++){
    digitalWrite(inc, LOW);
    delay(td);
    digitalWrite(inc, HIGH);
    delay(td);
    V0_b = analogRead(v0);
    V0_b_del = V0_b - V0_b_old;
    V0_b_old = V0_b;
    V0 = Vref*(V0_b/res);
    //Serial.println(V0);
    Rpot = R1*V0/(Vc - V0);
    Serial.println(Rpot);
  }
  digitalWrite(ledUp, LOW);
  while(1){
    V0_b = analogRead(v0);
    V0_b_del = V0_b - V0_b_old;
    V0_b_old = V0_b;
    V0 = Vref*(V0_b/res);
    V0_sum = V0_sum + V0;
    V0_ave = V0_sum/j;
    //Serial.println(V0_ave,3);
    Rpot = R1*V0/(Vc - V0);
    Rp_sum = Rpot + Rp_sum;
    Rp_max = Rp_sum/j;
    Serial.println(Rp_max);
    j++;
    delay(2*td);
  }
}

void resSweep(){
  digitalWrite(ledUp, HIGH);
  digitalWrite(cs, LOW);
  digitalWrite(ud, HIGH);
  delay(1);
  for(int i=1; i<100; i++){
    digitalWrite(inc, LOW);
    delay(td);
    digitalWrite(inc, HIGH);
    delay(td);
    V0_b = analogRead(v0);
    V0_b_del = V0_b - V0_b_old;
    V0_b_old = V0_b;
    V0 = Vref*(V0_b/res);
    Serial.println(V0);
  }
  digitalWrite(ledUp, LOW);
  
  for(int i=0; i<20; i++){
    V0_b = analogRead(v0);
    V0_b_del = V0_b - V0_b_old;
    V0_b_old = V0_b;
    V0 = Vref*(V0_b/res);
    Serial.println(V0);
    delay(2*td);
  }
  
  digitalWrite(ledDown, HIGH);
  digitalWrite(ud, LOW);
  delay(1);
  for(int i=1; i<100; i++){
    digitalWrite(inc, LOW);
    delay(td);
    digitalWrite(inc, HIGH);
    delay(td);
    V0_b = analogRead(v0);
    V0_b_del = V0_b - V0_b_old;
    V0_b_old = V0_b;
    V0 = Vref*(V0_b/res);
    Serial.println(V0);
  }
  digitalWrite(ledDown, LOW);
  digitalWrite(cs, HIGH);

  for(int i=0; i<20; i++){
    V0_b = analogRead(v0);
    V0_b_del = V0_b - V0_b_old;
    V0_b_old = V0_b;
    V0 = Vref*(V0_b/res);
    Serial.println(V0);
    delay(2*td);
  }
}

void btnStep(){
  if(bounceUp){
    ++k;
    digitalWrite(ledUp, HIGH);
    digitalWrite(cs, LOW);
    delay(1);
    digitalWrite(ud, HIGH);
    delay(1);
    digitalWrite(inc, LOW);
    delay(1);
    digitalWrite(inc, HIGH);
    delay(1);
    digitalWrite(cs, HIGH);

    V0_b = analogRead(v0);
    V0_b_del = V0_b - V0_b_old;
    V0 = Vref*(V0_b/res);
    Serial.println("-------------------------");
    Serial.println("Up");
    //Serial.print("Vout_b = ");Serial.println(V0_b);
    //Serial.print("dVout_b = ");Serial.println(V0_b_del);V0_b_old = V0_b;
    //Serial.print("Vout = ");Serial.println(V0);
    Serial.print("k = ");Serial.println(k);
    bounceUp = false;
    delay(250);
    digitalWrite(ledUp, LOW);
    delay(100);
  }
  else if(bounceDown){
    --k;
    digitalWrite(ledDown, HIGH);
    digitalWrite(cs, LOW);
    delay(1);
    digitalWrite(ud, LOW);
    delay(1);
    digitalWrite(inc, LOW);
    delay(1);
    digitalWrite(inc, HIGH);
    delay(1);
    digitalWrite(cs, HIGH);
    
    V0_b = analogRead(v0);
    V0_b_del = V0_b - V0_b_old;
    V0 = Vref*(V0_b/res);
    Serial.println("-------------------------");
    Serial.println("Down");
    //Serial.print("Vout_b = ");Serial.println(V0_b);
    //Serial.print("dVout_b = ");Serial.println(V0_b_del);V0_b_old = V0_b;
    //Serial.print("Vout = ");Serial.println(V0);
    Serial.print("k = ");Serial.println(k);
    bounceDown = false;
    delay(250);
    digitalWrite(ledDown, LOW);
    delay(100);
  }
}

void up_btn(){
  if(!bounceUp){
    bounceUp = true;
  }
}

void down_btn(){
  if(!bounceDown){
    bounceDown = true;
  }
}
