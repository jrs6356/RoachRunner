#include "RoachRunner_Digipot.h"

Digipot::Digipot(){
  cs = 0;
  inc = 0;
  ud = 0;
  upLED = 0;
  downLED = 0;

  dt = 10;

}

Digipot::Digipot(int _cs, int _inc, int _ud){

  cs = _cs;
  inc = _inc;
  ud = _ud;
  upLED = 0;
  downLED = 0;

  ai = false;

  dt = 10;

  init();
}

Digipot::Digipot(int _cs, int _inc, int _ud, int _v0){

  cs = _cs;
  inc = _inc;
  ud = _ud;
  upLED = 0;
  downLED = 0;
  v0 = _v0;

  ai = true;

  dt = 10;

  init();
}

Digipot::Digipot(int _cs, int _inc, int _ud, int _upLED, int _downLED){

  cs = _cs;
  inc = _inc;
  ud = _ud;
  upLED = _upLED;
  downLED = _downLED;

  ai = false;

  dt = 10;

  init();
}

Digipot::Digipot(int _cs, int _inc, int _ud, int _upLED, int _downLED, int _v0){

  cs = _cs;
  inc = _inc;
  ud = _ud;
  upLED = _upLED;
  downLED = _downLED;
  v0 = _v0;

  ai = true;

  dt = 10;

  init();
}

void Digipot::init(){
  if((cs==0) || (inc==0) || (ud==0)){
    Serial.println("Digipot Pins Not Assigned!");
  }else{
    pinMode(cs, OUTPUT);
    pinMode(inc, OUTPUT);
    pinMode(ud, OUTPUT);

    digitalWrite(cs, HIGH);
    digitalWrite(inc, HIGH);
    digitalWrite(ud, LOW);
    initState = true;
    Serial.println("Digipot Initialized");
  }
  if((downLED==0) || (upLED==0)){
    led = false;
  }else{
    led = true;
    pinMode(upLED, OUTPUT);
    pinMode(downLED, OUTPUT);

    digitalWrite(upLED, LOW);
    digitalWrite(downLED, LOW);
  }
}

void Digipot::reset(int k, bool a, bool b, int t){
  MI_init = k;
  for(int i=0;i<110;++i){
    decr(t,a);
  }
  for(int i=0;i<MI_init;++i){
    incr(t,b);
  }
}

void Digipot::incr(int dur, bool r){
  if(MI<99){
    ++MI;
  }
  if(led){
    digitalWrite(upLED, HIGH);
  }
  digitalWrite(cs, LOW);
  delayMicroseconds(1);
  digitalWrite(ud, HIGH);
  delayMicroseconds(10);
  digitalWrite(inc, LOW);
  delayMicroseconds(5);
  digitalWrite(inc, HIGH);
  delayMicroseconds(5);
  digitalWrite(cs, HIGH);
  if(led){
    delay(dur*dt);
    digitalWrite(upLED, LOW);
    delay(dur*dt);
  }
  if(r){
    Serial.println("-------------------------");
    Serial.println("Up");
    Serial.print("MI = ");Serial.println(MI);
  }
}

void Digipot::decr(int dur, bool r){
  if(MI>0){
      --MI;
  }
  if(led){
    digitalWrite(downLED, HIGH);
  }
  digitalWrite(cs, LOW);
  delayMicroseconds(1);
  digitalWrite(ud, LOW);
  delayMicroseconds(10);
  digitalWrite(inc, LOW);
  delayMicroseconds(5);
  digitalWrite(inc, HIGH);
  delayMicroseconds(5);
  digitalWrite(cs, HIGH);
  if(led){
    delay(dur*dt);
    digitalWrite(downLED, LOW);
    delay(dur*dt);
  }
  if(r){
    Serial.println("-------------------------");
    Serial.println("Down");
    Serial.print("MI = ");Serial.println(MI);
  }
}

void Digipot::voltSet(float V){
  Vdes = V;
  /*
  k_des = round(99*((R1*Vdes/(Vcc-Vdes)) - Rw_calc)/(Rp_max_calc - Rw_calc))-1;
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
  */
}

void Digipot::resMin(int t){
  for(int i=0;i<110;++i){
    decr(t,0);
  }
}

void Digipot::resMax(int t){
  for(int i=0;i<110;++i){
    incr(t,0);
  }
}

void Digipot::resSweep(int t){
  for(int i=0;i<110;++i){
    decr(t,0);
  }
  for(int i=0;i<110;++i){
    incr(t,0);
  }
}

void Digipot::btnStep(){
  if(bounceUp){
    incr(15,1);
    bounceUp = false;
    delay(300);
  }
  else if(bounceDown){
    decr(15,1);
    bounceDown = false;
    delay(300);
  }
}
