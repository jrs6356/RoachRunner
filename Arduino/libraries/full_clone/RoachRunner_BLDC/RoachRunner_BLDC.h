#ifndef _ROACHRUNNERBLDC_H_INCLUDED
#define _ROACHRUNNERBLDC_H_INCLUDED

#include <Arduino.h>
#include "RoachRunner_MUX.h"

#define CW 		false
#define CCW 	true

class BLDC {
public:
	//Methods
	BLDC(byte a[4], double b[3], MUX*);
	void init(void);
	void ON(void);
	void OFF(void);
	void DIR(bool);
	void SET_SPEED(uint32_t);
	void SET_SPEED(float, int);
	double MEAS_SPEED(void);
	void PID(double);

	//Pins
	byte enP;
	byte dirP;
	byte pwmP;
	byte comP;
	
	//Objects
	MUX* mux;
	
	//Variables
	bool enV = false;	// true=on , false=off
	bool dirV = true;	// true=ccw , false=cw
	uint32_t pwmB = 0;
	float pwmF = 0.0;
	
	float sMax = 12000.0; //RPM
	float m = 15000.0;
	float b = -1500.0;
	float lbX = 0.1;
	float ubX = 0.9;
	int resI = 12;
	float res = pow(2,resI);
	float speedK = 680.0; // rpm/V
	float thermalKt = 5.4; // sec
	float ppN = 2;	// pole pairs
	float vRampK = 1000.0; 	// rpm/s
	float gr = 3375.0/64.0;
	
	double kp = 0.0;
	double ki = 0.0;
	double kd = 0.0;
	bool p2e = true;
	double error = 0;
	unsigned long ts = 100000; // Sampling time = 0.1s
	volatile unsigned long ti = 0;
	volatile unsigned long tf = 0;
	volatile unsigned long tc = 0;
	volatile double dt = 0;
	volatile unsigned long dtk = 0;
	volatile double p = 0;
	volatile uint32_t pk = 0;
	volatile bool bip = false;
	double sMeasI = 0;
	double sMeasF = 0;
	double dSMeas = 0;
	double outputSum;
	double output;
	
	//
};

#endif