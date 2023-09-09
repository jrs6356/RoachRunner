#include <Arduino.h>
#include "RoachRunner_MUX.h"

MUX::MUX(){
	le = 0;
	e0 = 0;
	e1 = 0;
	e2 = 0;
	e3 = 0;
	a0_0 = 0;
	a1_0 = 0;
	a2_0 = 0;
	a3_0 = 0;
	a0_1 = 0;
	a1_1 = 0;
	a2_1 = 0;
	a3_1 = 0;
}

MUX::MUX(byte _pins[13], bool* _v, bool* _pinsSet, bool* _ready){
	le = _pins[0];
	e0 = _pins[1];
	e1 = _pins[2];
	e2 = _pins[3];
	e3 = _pins[4];
	a0_0 = _pins[5];
	a1_0 = _pins[6];
	a2_0 = _pins[7];
	a3_0 = _pins[8];
	a0_1 = _pins[9];
	a1_1 = _pins[10];
	a2_1 = _pins[11];
	a3_1 = _pins[12];
	
	verbose = _v;
	pinsSet = _pinsSet;
	ready = _ready;
	
	*pinsSet = true;
}

void MUX::init(){
	if(!*ready){
		if(*verbose){
			Serial.println("\n----------\nInitializing MUX\n----------");
		}
		if(*pinsSet){
			pinMode(le, OUTPUT);
			pinMode(e0, OUTPUT);
			pinMode(e1, OUTPUT);
			pinMode(e3, OUTPUT);
			pinMode(a0_0, OUTPUT);
			pinMode(a1_0, OUTPUT);
			pinMode(a2_0, OUTPUT);
			pinMode(a3_0, OUTPUT);
			pinMode(a0_1, OUTPUT);
			pinMode(a1_1, OUTPUT);
			pinMode(a2_1, OUTPUT);
			pinMode(a3_1, OUTPUT);
			LE = 1;
			E0 = 1;	//physical
			E1 = 1;	//physical
			E2 = 1;	//virtual
			E3 = 1;	//physical
			A00 = 0;
			A10 = 0;
			A20 = 0;
			A30 = 0;
			A01 = 0;
			A11 = 0;
			A21 = 0;
			A31 = 0;
			digitalWrite(le, HIGH);
			digitalWrite(e0, HIGH);
			digitalWrite(e1, HIGH);
			digitalWrite(e3, HIGH);
			digitalWrite(a0_0, LOW);
			digitalWrite(a1_0, LOW);
			digitalWrite(a2_0, LOW);
			digitalWrite(a3_0, LOW);
			digitalWrite(a0_1, LOW);
			digitalWrite(a1_1, LOW);
			digitalWrite(a2_1, LOW);
			digitalWrite(a3_1, LOW);
			
			*ready = true;
			if(*verbose){
				Serial.println("\n----------\nMUX Initialized\n----------");
			}
		}
		else{
			if(*verbose){
				Serial.println("\n----------\nMUX Init Failed\n----------");
			}
		}
	}
	else{
		if(*verbose){
			Serial.println("\n----------\nMUX Already Initialized\n----------");
		}
	}
}

void MUX::on(byte _ch){
	byte sys = _ch >> 4;
	byte A0 = 1&_ch;
	byte A1 = 1&(_ch>>1);
	byte A2 = 1&(_ch>>2);
	byte A3 = 1&(_ch>>3);
	
	switch(_ch >> 4){
		case 0:		//CS: Signal Conditioners, LCD, SD, Accel+Gyro, Mag
			if(*verbose){
				Serial.println("\nMUX A");
			}
			digitalWrite(e0,HIGH);
			digitalWrite(e1,HIGH);
			delayMicroseconds(1);
			digitalWrite(a0_0,A0);
			digitalWrite(a1_0,A1);
			digitalWrite(a2_0,A2);
			digitalWrite(a3_0,A3);
			delayMicroseconds(1);
			digitalWrite(e0,LOW);
			E0 = 0;
			E1 = 1;
			A00 = A0;
			A10 = A1;
			A20 = A2;
			A30 = A3;
			if(*verbose){
				report(_ch);
			}
			break;
		case 1:		//E: Signal Conditioners, LCD Relay MUX		CS: Digipot, LCD2, All Signal Conditioners
			if(*verbose){
				Serial.println("\nMUX B");
			}
			digitalWrite(e0,HIGH);
			digitalWrite(e1,HIGH);
			delayMicroseconds(1);
			digitalWrite(a0_0,A0);
			digitalWrite(a1_0,A1);
			digitalWrite(a2_0,A2);
			digitalWrite(a3_0,A3);
			delayMicroseconds(1);
			digitalWrite(e1,LOW);
			E0 = 1;
			E1 = 0;
			A00 = A0;
			A10 = A1;
			A20 = A2;
			A30 = A3;
			if(*verbose){
				report(_ch);
			}
			break;
		case 2:		//LCD Relay Signals
			if(*verbose){
				Serial.println("\nMUX C");
			}
			digitalWrite(e0,HIGH);
			digitalWrite(e1,HIGH);
			digitalWrite(e3,HIGH);
			delay(5);
			digitalWrite(a0_1,A0);
			digitalWrite(a1_1,A1);
			digitalWrite(a2_1,A2);
			digitalWrite(a3_1,A3);
			delay(5);
			E0 = 1;
			E1 = 1;
			E3 = 1;
			A01 = A0;
			A11 = A1;
			A21 = A2;
			A31 = A3;
			//on(e2);
			digitalWrite(a0_0,HIGH);
			digitalWrite(a1_0,LOW);
			digitalWrite(a2_0,HIGH);
			digitalWrite(a3_0,HIGH);
			delay(5);
			digitalWrite(e1,LOW);
			delay(10);
			digitalWrite(e1,HIGH);
			E1 = 1;
			if(*verbose){
				report(_ch);
			}
			break;
		case 3:		//SDO Select
			if(*verbose){
				Serial.println("\nMUX D");
			}
			digitalWrite(e3,HIGH);
			delayMicroseconds(1);
			digitalWrite(a0_1,A0);
			digitalWrite(a1_1,A1);
			digitalWrite(a2_1,A2);
			digitalWrite(a3_1,A3);
			delayMicroseconds(1);
			digitalWrite(e3,LOW);
			E3 = 0;
			A01 = A0;
			A11 = A1;
			A21 = A2;
			A31 = A3;
			if(*verbose){
				report(_ch);
			}
			break;
		case 4:		//Signal Conditioner Relay Signals
			if(*verbose){
				Serial.println("\nMUX E");
			}
			digitalWrite(e1,HIGH);
			digitalWrite(e3,HIGH);
			delayMicroseconds(1);
			digitalWrite(a0_1,A0);
			digitalWrite(a1_1,A1);
			digitalWrite(a2_1,A2);
			digitalWrite(a3_1,A3);
			delayMicroseconds(1);
			E1 = 1;
			E3 = 1;
			A01 = A0;
			A11 = A1;
			A21 = A2;
			A31 = A3;
			if(*verbose){
				report(_ch);
			}
			break;
	}
}

void MUX::off(byte _ch){
	switch(_ch >> 4){
		case 0:		//CS: Signal Conditioners, LCD, SD, Accel+Gyro, Mag
			digitalWrite(e0,HIGH);
			digitalWrite(e1,HIGH);
			E0 = 1;
			E1 = 1;
			break;
		case 1:		//E: Signal Conditioners, LCD Relay MUX		CS: Digipot, LCD2, All Signal Conditioners
			digitalWrite(e0,HIGH);
			digitalWrite(e1,HIGH);
			E0 = 1;
			E1 = 1;
			break;
		case 2:		//LCD Relay Signals
			digitalWrite(e0,HIGH);
			digitalWrite(e1,HIGH);
			digitalWrite(e3,HIGH);
			E0 = 1;
			E1 = 1;
			E3 = 1;
			break;
		case 3:		//SDO Select
			digitalWrite(e3,HIGH);
			E3 = 1;
			break;
		case 4:		//Signal Conditioner Relay Signals
			delayMicroseconds(10);
			break;
	}
}

void MUX::report(byte _ch){
	Serial.print("----- MUX: ");Serial.print(_ch);Serial.println(" -----");
	Serial.print("E0:\t");Serial.println(E0);
	Serial.print("E1:\t");Serial.println(E1);
	Serial.print("E3:\t");Serial.println(E3);
	Serial.print(15&_ch);Serial.print(":\t");
	Serial.print(1&(_ch>>3));
	Serial.print(1&(_ch>>2));
	Serial.print(1&(_ch>>1));
	Serial.println(1&(_ch>>0));
	Serial.print("A00:\t");Serial.print(A00);Serial.print("\tA01:\t");Serial.println(A01);
	Serial.print("A10:\t");Serial.print(A10);Serial.print("\tA11:\t");Serial.println(A11);
	Serial.print("A20:\t");Serial.print(A20);Serial.print("\tA21:\t");Serial.println(A21);
	Serial.print("A30:\t");Serial.print(A30);Serial.print("\tA31:\t");Serial.println(A31);
}