#ifndef _ROACHRUNNER_SIGNAL_CONDITIONER_H_INCLUDED
	#define _ROACHRUNNER_SIGNAL_CONDITIONER_H_INCLUDED
	
	#include <Arduino.h>
	#include <SPI.h>
	#include "RoachRunner_Digipot.h"
	#include "RoachRunner_Math.h"
	#include "RoachRunner_CS5532.h"
	
	class SigCond {
	public:
		//Commands
		SigCond();
		SigCond(int[12],int,int);
		void init(void);
		void startSPI();
		void stopSPI();
		void flightCheck();
		
		//Variables
		bool SPIstarted = false;
		bool adc1Go = false;
		bool adc2Go = false;
		bool adc3Go = false;
		bool adc4Go = false;
		bool adc5Go = false;
		bool adc6Go = false;
		bool adc7Go = false;
		bool adc8Go = false;
		bool adc9Go = false;
		bool adc10Go = false;
		bool adc11Go = false;
		bool adc12Go = false;
		bool arrayGo = false;
		
		//ADCs
		CS5532 adc1;
		CS5532 adc2;
		CS5532 adc3;
		CS5532 adc4;
		CS5532 adc5;
		CS5532 adc6;
		CS5532 adc7;
		CS5532 adc8;
		CS5532 adc9;
		CS5532 adc10;
		CS5532 adc11;
		CS5532 adc12;
	};
	
#endif