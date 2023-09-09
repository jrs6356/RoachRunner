#include <Arduino.h>
#include "RoachRunner_MUX.h"
#include "RoachRunner_BLDC.h"

BLDC::BLDC(byte pins[4], double gains[3], MUX* _mux){
	enP = pins[0];
	dirP = pins[1];
	pwmP = pins[2];
	comP = pins[3];
	
	kp = gains[0];
	ki = gains[1];
	kd = gains[2];
	
	mux = _mux;
}

void BLDC::init(void){
	//pinMode(comP,INPUT_PULLUP);
	pinMode(enP,OUTPUT);
	pinMode(dirP,OUTPUT);
	pinMode(pwmP,OUTPUT);
	
	analogWriteResolution(resI);
	digitalWrite(enP,enV);
	digitalWrite(dirP,dirV);
	analogWrite(pwmP,0);
}

void BLDC::ON(void){
	enV = true;
	digitalWrite(enP,enV);
}

void BLDC::OFF(void){
	enV = false;
	digitalWrite(enP,enV);
}

void BLDC::DIR(bool _d){
	dirV = _d;
}

void BLDC::SET_SPEED(uint32_t _s){
	analogWrite(pwmP,_s); 	//Where _s is a duty expressed as an int in range 0-4096
	outputSum = (((double)_s/409600.0)*m) + b;
}

void BLDC::SET_SPEED(float _s, int _u){
	uint32_t v;
	if(_u){
		v = (uint32_t)(_s*res/100.0);	//Where _s is a percentage XX.xx%, _u=1
		outputSum = ((_s/100.0)*m) + b;
	}
	else{
		v = (uint32_t)((_s-b)*(4096.0/m));	//Where _s is motor speed in RPM, _u=0
		outputSum = _s;
	}
	analogWrite(pwmP,v);
}

double BLDC::MEAS_SPEED(void){
	sMeasI = sMeasF;
	if(bip){
		sMeasF = 30000000.0*p/dt;	//Commutation frequency -> speed of the motor shaft output
	}
	dSMeas = sMeasF - sMeasI;
	return sMeasF;
}

void BLDC::PID(double _s){
	if(bip){
		bip = false;
		MEAS_SPEED();
		error = _s - sMeasF;
		outputSum+= (ki * error);
		if(!p2e){
			outputSum-= kp*dSMeas;
		}
		if(outputSum>sMax){
			outputSum = sMax;
		}
		else if(outputSum<0){
			outputSum = 0;
		}
		if(p2e){
			output = kp*error;
		}
		else{
			output = 0;
		}
		output += outputSum - kd*dSMeas;
		
		if(output>sMax){
			output = sMax;
		}
		else if(output<0){
			output = 0;
		}
		uint32_t vOut = (uint32_t)(output-b)*(409600.0/m);
		analogWrite(pwmP,vOut);
		bip = false;
		ti = micros();
	}
}