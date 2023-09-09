#ifndef _ROACHRUNNERMUX_H_INCLUDED
#define _ROACHRUNNERMUX_H_INCLUDED

#include <Arduino.h>

class MUX {
public:
	MUX();
	MUX(byte _pins[13], bool*, bool*, bool*);
	void init();
	void on(byte);
	void off(byte);
	void report(byte);
	
	byte le;
	byte e0;	//physical
	byte e1;	//physical
	byte e2;	//virtual
	byte e3;	//physical
	byte a0_0;
	byte a1_0;
	byte a2_0;
	byte a3_0;
	byte a0_1;
	byte a1_1;
	byte a2_1;
	byte a3_1;
	
	byte LE = 0;
	byte E0 = 0;
	byte E1 = 0;
	byte E2 = 0;
	byte E3 = 0;
	byte A00 = 0;
	byte A10 = 0;
	byte A20 = 0;
	byte A30 = 0;
	byte A01 = 0;
	byte A11 = 0;
	byte A21 = 0;
	byte A31 = 0;
	
	bool* ready;
	bool* pinsSet;
	bool* verbose;
};

#endif