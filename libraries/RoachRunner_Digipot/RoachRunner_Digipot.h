#ifndef _ROACHRUNNER_DIGIPOT_H_INCLUDED
	#define _ROACHRUNNER_DIGIPOT_H_INCLUDED

	#include <Arduino.h>

	class Digipot {
	public:
		Digipot();
		Digipot(int _cs, int _inc, int _ud);
		Digipot(int _cs, int _inc, int _ud, int _v0);
		Digipot(int _cs, int _inc, int _ud, int _upLED, int _downLED);
		Digipot(int _cs, int _inc, int _ud, int _upLED, int _downLED, int _v0);

		void init(void);
		void reset(int k, bool a, bool b, int t);
		void incr(int dur, bool r);
		void decr(int dur, bool r);
		void voltSet(float V);
		void resMin(int t);
		void resMax(int t);
		void resSweep(int t);
		void btnStep(void);

		byte cs;
		byte inc;
		byte ud;
		byte upLED;
		byte downLED;
		int v0;

		volatile bool bounceUp;
		volatile bool bounceDown;
		bool led;
		bool ai;
		bool initState = false;

		int MI = 0;
		int MI_init;
		int MI_des;
		int j;
		int dt;

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