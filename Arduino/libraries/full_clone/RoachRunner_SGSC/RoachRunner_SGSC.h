#ifndef _ROACHRUNNER_SIGNAL_CONDITIONER_H_INCLUDED
#define _ROACHRUNNER_SIGNAL_CONDITIONER_H_INCLUDED

#include <Arduino.h>
#include <SPI.h>
#include "RoachRunner_ADC.h"
#include "RoachRunner_MUX.h"

#define R2S3_OFF 	0x40
#define R2S3_ON		0x41
#define R2S2_OFF	0x42
#define R2S2_ON		0x43
#define R2SG		0x44
#define R2SH		0x45
#define R3SG		0x46
#define R3SH		0x47
#define R3S1_OFF	0x48
#define R3S1_ON		0x49
#define R3S2_OFF	0x4A
#define R3S2_ON		0x4B
#define R3S3_OFF	0x4C
#define R3S3_ON		0x4D
#define JK			0x4E

class SGSC {
public:
	//Commands
	//SGSC(String, byte*, volatile bool*, byte*, byte*, byte*, byte*, byte*, MUX*);
	SGSC(String, byte*, byte*, byte*, volatile bool*, byte* _ints[5], bool* _flags[6], uint32_t* _settings[9], MUX*);
	void init(bool);
	void setBridge(byte _r2);
	void jk(void);
	void setPGIA(int _n);
	byte checkPGIA(void);
	
	//Variables
	String name;
	byte* adcN;
	byte* adcMic;
	byte* e;
	byte* cs;
	byte* csAll;
	byte* esdo;
	//byte* osc;
	MUX* mux;
	CS5532 adc;
	int pgiaG = 0;
	
	bool ready = false;
	volatile bool* sdof;
	bool* verbose;
	bool* pinsSet;
};
	
#endif