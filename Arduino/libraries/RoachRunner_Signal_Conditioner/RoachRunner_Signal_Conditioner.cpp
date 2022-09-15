#include <Arduino.h>
#include <SPI.h>
#include "RoachRunner_Digipot.h"
#include "RoachRunner_Math."
#include "RoachRunner_CS5532.h"
#include "RoachRunner_Signal_Conditioner.h"

SigCond::SigCond(){
	adc1.name = "Q1X";
	adc2.name = "Q1Y";
	adc3.name = "Q1Z";
	adc4.name = "Q2X";
	adc5.name = "Q2Y";
	adc6.name = "Q2Z";
	adc7.name = "Q3X";
	adc8.name = "Q3Y";
	adc9.name = "Q3Z";
	adc10.name = "Q4X";
	adc11.name = "Q4Y";
	adc12.name = "Q4Z";
	
	adc1.id = Q1X;
	adc2.id = Q1Y;
	adc3.id = Q1Z;
	adc4.id = Q2X;
	adc5.id = Q2Y;
	adc6.id = Q2Z;
	adc7.id = Q3X;
	adc8.id = Q3Y;
	adc9.id = Q3Z;
	adc10.id = Q4X;
	adc11.id = Q4Y;
	adc12.id = Q4Z;
	
}

SigCond::SigCond(int[12] _cs, int _sdo, int _clk){
	
}

void SigCond::init(){
	
}

void SigCond::startSPI(){
	
}

void SigCond::stopSPI(){
	
}

void SigCond::flightCheck(){
	
}