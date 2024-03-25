#ifndef _ROACHRUNNER_DIGIPOT_H_INCLUDED
#define _ROACHRUNNER_DIGIPOT_H_INCLUDED

#include <Arduino.h>
#include "RoachRunner_MUX.h"

class Digipot {
public:
	Digipot(byte*, byte*, byte*, MUX*);

	void init(void);
	void reset(int k);
	void set(int k);
	void incr(void);
	void decr(void);
	void resMin(void);
	void resMax(void);
	
	MUX* mux;
	byte* cs;
	byte* inc;
	byte* ud;
	
	bool* verbose;
	bool initState = false;

	int MI = 0;
	int MI_init = 0;
	int MI_des = 0;
	int j = 0;
	int dt = 10;

	float Rhl_0;
	float Rhl_99;
	float Ra;
	float Rb;
	float Rw;
	float R1;
	float R2;

	float V0;
	float Vcc;
	float Vdes;
	float Vref;
	float V0_sum;
	float V0_ave;

	float res;
};

#endif