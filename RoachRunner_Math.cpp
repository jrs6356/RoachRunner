#include "RoachRunner_Math.h"
#include <Arduino.h>

void sciNote(float x, int d){
	float a;
	float xAbs = abs(x);
	int b = 0;
	if((isnan(x)) || x==0){
		Serial.print("0.0000 E0");
	}
	else{
		if(x<0.0){
			Serial.print("-");
		}
		if(xAbs<1.0){
			a = 0.0;
			while(a<1.0){
				++b;
				a = xAbs * float(pow(10,b));
			}
			Serial.print(a,d);
			Serial.print(" E-");
			Serial.print(b);
		}
		else if(xAbs>10.0){
			a = 100.0;
			while(a>10){
				++b;
				a = xAbs * float(pow(10,-b));
			}
			Serial.print(a,d);
			Serial.print(" E");
			Serial.print(b);
		}
		else{
			Serial.print(xAbs,d);
			Serial.print(" E0");
		}
	}
}