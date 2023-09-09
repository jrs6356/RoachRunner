#include <Arduino.h>
#include <SPI.h>
#include "RoachRunner_ADC.h"
#include "RoachRunner_SGSC.h"
#include "RoachRunner_MUX.h"

SGSC::SGSC(String _name, byte* _cs, byte* _e, byte* _esdo, volatile bool* _sdof, byte* _ints[5], bool* _flags[6], uint32_t* _settings[9], MUX* _mux){
	name 		= _name;
	cs 			= _cs;
	e 			= _e;
	esdo 		= _esdo;
	sdof 		= _sdof;
	
	adc.name 	= _name;
	adc.cs 		= _cs;
	adc.esdo 	= _esdo;
	adc.sdof 	= _sdof;
	
	adcN 		= _ints[0];
	adcMic		= _ints[4];
	csAll 		= _ints[2];
	
	adc.adcN 	= _ints[0];
	adc.adcSetN = _ints[1];
	adc.csAll 	= _ints[2];
	adc.osc 	= _ints[3];
	adc.adcMic	= _ints[4];
	
	verbose = _flags[0];
	pinsSet = _flags[1];
	
	adc.verbose = _flags[0];
	adc.pinsSet = _flags[1];
	adc.clockStarted = _flags[2];
	adc.SPIStarted = _flags[3];
	adc.ADCSerialInit = _flags[4];
	adc.allClear = _flags[5];
	
	adc.CONF = _settings[0];
	adc.CSR1 = _settings[1];
	adc.CSR2 = _settings[2];
	adc.CSR3 = _settings[3];
	adc.CSR4 = _settings[4];
	adc.CSR5 = _settings[5];
	adc.CSR6 = _settings[6];
	adc.CSR7 = _settings[7];
	adc.CSR8 = _settings[8];
	
	mux = _mux;
	adc.mux = _mux;
}

void SGSC::init(bool _v){
	*verbose = _v;
	if(*verbose){
		Serial.println("\n----------\nInitializing Signal Conditioner "+name+"\n----------");
	}
	
	mux->init();
	
	if(mux->ready){
		if(*verbose){
			Serial.println("Setting relays");
		}
		//setBridge(R2SG);
		//setBridge(R3SG);
		adc.init();
		if(*verbose){
			Serial.println("\n----------\nSignal Conditioner "+name+" Initialized\n----------");
		}
		ready = adc.measReady;
	}
	else{
		if(*verbose){
			Serial.println("\n----------\nSignal Conditioner Init Failed\n----------");
		}
	}
}

void SGSC::setBridge(byte _r2){
	mux->off(*e);
	mux->on(_r2);
	mux->on(*e);
	delay(5);
	mux->off(*e);
}

void SGSC::jk(){
	mux->off(*e);
	mux->on(JK);
	mux->on(*e);
	delay(1);
	mux->off(*e);
	pgiaG = pgiaG + 1;
	if(pgiaG>3){
		pgiaG = 0;
	}
}

void SGSC::setPGIA(int _n){
	int _g = pgiaG;
	//Serial.println("---PGIA: "+name+"---");
	//Serial.println("Old:"+String(_g)+" , New:"+String(_n));
	if(_n>_g){
		//Serial.print("Up Tick: ");
		for(int _i=0;_i<_n-_g;++_i){
			//Serial.print(String(_i)+", ");
			jk();
		}
	}
	else if(_n<_g){
		//Serial.print("Down Tick: ");
		for(int _i=0;_i<4-(_g-_n);++_i){
			//Serial.print(String(_i)+", ");
			jk();
		}
	}
	//Serial.print("\n");
}

byte SGSC::checkPGIA(){
	if(*verbose){
		Serial.print("\t--- Checking PGIA Gain for ");Serial.print(name);
		Serial.print(": ");
	}
	byte adm = adc.id - 1;
	*adcMic = adm;
	if(*verbose){
		Serial.print(*adcMic);
		Serial.println(" ---");
	}
	setBridge(R2S2_OFF);
	setBridge(R2S3_OFF);
	setBridge(R3S1_ON);
	setBridge(R3S2_OFF);
	setBridge(R3S3_OFF);
	setBridge(R2SH);
	setBridge(R3SH);
	delay(100);
	*sdof = false;
	
	//*verbose = true;
	adc.setup4.G = G2;
	adc.setup4.merge();
	//Serial.println("\t--- Setting Gain for Setup4 ---");
	adc.writeReg(Setup4,0);
	//Serial.println("\n\n\t--- Performing Offset Cal via Setup4 ---");
	adc.sysOffCal(Setup4,0);
	//Serial.println("\n\n\t--- Uploading Gain to Gain1 ---");
	adc.uploadGain(Gain1,5000000,0);
	//Serial.println("\n\t--- Uploading Offset to Offset1 ---");
	//adc.uploadOffset(Offset1,adc.offsetReg1.out+134217728,0);
	//Serial.println("\t--- Performing Flightcheck ---");
	setBridge(R2SG);
	adc.startSPI();
	adc.flightCheck();
	delay(100);
	
	//Serial.println("\t--- Starting Measurement via Setup 4 ---");
	adc.startSPI();
	adc.contMeas(Setup4,0);
	uint32_t m = 0;
	int mi = 0;
	int miA = 0;
	int miAv[4] = {0,0,0,0};
	*sdof = false;
	delay(100);
	if(*verbose){
		Serial.print("----- ");Serial.print(name);Serial.println(" -----");
	}
	for(int i=0;i<4;++i){
		for(int j=0;j<10;++j){
			mux->on(*cs);
			m = adc.scanMeas();
			mi = abs(adc.conv2Dec());
			miA = miA + mi;
			//Serial.print("\t");Serial.println(mi);
			*sdof = false;
		}
		miAv[i] = miA/10;
		if(*verbose){
			Serial.print(miAv[i]);
			if(miAv[i]==8388607){
				Serial.println("\t\t!!");
			}
			else{
				Serial.println(" ");
			}
		}
		miA = 0;
		jk();
		delay(100);
	}
	mux->on(*cs);
	*sdof = false;
	adc.stopMeas(0);
	*verbose = false;
	byte ind = 0;
	for(int i=1;i<4;++i){
		if(*verbose){
			Serial.print(miAv[ind]);
		}
		if(miAv[ind]<miAv[i]){
			ind = i;
			if(*verbose){
				Serial.print(" < ");
			}
		}
		else{
			if(*verbose){
				Serial.print(" > ");
			}
		}
		if(*verbose){
			Serial.println(miAv[i]);
		}
	}
	if(*verbose){
		Serial.print("Row of Max: ");
		Serial.println(ind);
	}
	ind = 3 - ind;
	byte gA0 = ind&1;
	byte gA1 = 1&(ind>>1);
	if(*verbose){
		Serial.print("Current Rank: ");
		Serial.println(ind);
		Serial.print("A0 = ");Serial.println(gA0);
		Serial.print("A1 = ");Serial.println(gA1);
		Serial.println("-----");
	}
	return ind;
}