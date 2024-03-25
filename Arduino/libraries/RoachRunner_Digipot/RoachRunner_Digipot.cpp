#include "RoachRunner_Digipot.h"
#include "RoachRunner_MUX.h"

Digipot::Digipot(byte* _cs, byte* _inc, byte* _ud, MUX* _mux){
	cs = _cs;
	inc = _inc;
	ud = _ud;
	mux = _mux;
	verbose = mux->verbose;
}

void Digipot::init(){
  if(*verbose && ((*cs==0) || (*inc==0) || (*ud==0))){
    Serial.println("Digipot Pins Not Assigned!");
  }
  else{
    pinMode(*inc, OUTPUT);
    pinMode(*ud, OUTPUT);

    digitalWrite(*inc, HIGH);
    digitalWrite(*ud, LOW);
    initState = true;
	if(*verbose){
		Serial.println("Digipot Initialized");
	}
  }
}

void Digipot::reset(int k){
  MI_init = k;
  resMin();
  set(MI_init);
  //for(int i=0;i<MI_init;++i){
    //incr();
  //}
}

void Digipot::set(int k){
	MI_des = k;
	while(MI<MI_des){
		incr();
	}
	while(MI>MI_des){
		decr();
	}
}

void Digipot::incr(){
  if(MI<99){
    ++MI;
  }
  mux->on(*cs);
  delayMicroseconds(1);
  digitalWrite(*ud, HIGH);
  delayMicroseconds(10);
  digitalWrite(*inc, LOW);
  delayMicroseconds(5);
  digitalWrite(*inc, HIGH);
  delayMicroseconds(5);
  mux->off(*cs);
  if(*verbose){
    Serial.println("-------------------------");
    Serial.println("Up");
    Serial.print("MI = ");Serial.println(MI);
  }
}

void Digipot::decr(){
  if(MI>0){
      --MI;
  }
  mux->on(*cs);
  delayMicroseconds(1);
  digitalWrite(*ud, LOW);
  delayMicroseconds(10);
  digitalWrite(*inc, LOW);
  delayMicroseconds(5);
  digitalWrite(*inc, HIGH);
  delayMicroseconds(5);
  mux->off(*cs);
  if(*verbose){
    Serial.println("-------------------------");
    Serial.println("Down");
    Serial.print("MI = ");Serial.println(MI);
  }
}

void Digipot::resMin(){
  for(int i=0;i<110;++i){
    decr();
  }
}

void Digipot::resMax(){
  for(int i=0;i<110;++i){
    incr();
  }
}