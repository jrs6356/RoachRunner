#ifndef _ROACHRUNNERADC_H_INCLUDED
#define _ROACHRUNNERADC_H_INCLUDED

#include <Arduino.h>
#include <SPI.h>
#include "RoachRunner_Math.h"
#include "RoachRunner_MUX.h"

//Command Options - 8 bit
//ComA
#define ARA_INDIV   0x00
#define ARA_ARRAY   0x40
#define CH1         0x00
#define CH2         0x10
#define CH3         0x20
#define CH4         0x30
#define RW_W        0x00
#define RW_R        0x08
#define RSB_OFFSET  0x01
#define RSB_GAIN    0x02
#define RSB_CONF    0x03
#define RSB_CSR     0x05
//ComB
#define MC_SINGLE   0x80
#define MC_CONT     0xC0
#define SETUP1      0x80	//For Conversion
#define SETUP2      0x88	//   ||
#define SETUP3      0x90	//   ||
#define SETUP4      0x98	//  _||_
#define SETUP5      0xA0	//  \  /
#define SETUP6      0xA8	//   \/
#define SETUP7      0xB0
#define SETUP8      0xB8
#define CC_SELF_OC  0x81
#define CC_SELF_GC  0x82
#define CC_SYS_OC   0x85
#define CC_SYS_GC   0x86
#define CC_NORM_CV	0x80
#define SYNC1       0xFF
#define SYNC0       0xFE

//Configuration Register Options - 32 bit
#define PSS_STB       0x00000000
#define PSS_SLP       0x80000000
#define PDW_NORM      0x00000000
#define PDW_SAVE      0x40000000
#define RS_NORM       0x00000000
#define RS_RESET      0x20000000
#define RV_NORM       0x00000000
#define RV_VALID      0x10000000
#define IS_NORM       0x00000000
#define IS_SHORT      0x08000000
#define GB_A0_LATCH   0x00000000
#define GB_A0_CMOV    0x04000000
#define VRS_HIGH      0x00000000
#define VRS_LOW       0x02000000
#define OL_0          0x00000000
#define OL_1          0x00800000
#define OL_2          0x01000000
#define OL_3          0x01800000
#define OLS_CSR       0x00000000
#define OLS_CONF      0x00400000
#define OGS_CS        0x00000000
#define OGS_OG        0x00100000
#define FRS_NORM      0x00000000
#define FRS_SCALE     0x00080000

//Channel Setup Register Options - 16 bit
#define C1            0x0000
#define C2            0x4000
#define C3            0x8000
#define C4            0x1200
#define G1            0x0000
#define G2            0x0800
#define G4            0x1000
#define G8            0x1800
#define G16           0x2000
#define G32           0x2800
#define G64           0x3000
#define WR120         0x0000
#define WR60          0x0080
#define WR30          0x0100
#define WR15          0x0180
#define WR7_5         0x0200
#define WR3840        0x0400
#define WR1920        0x0480
#define WR960         0x0500
#define WR480         0x0580
#define WR240         0x0600
#define UB_BI         0x0000
#define UB_UNI        0x0040
#define OLB_0         0x0000
#define OLB_1         0x0010
#define OLB_2         0x0020
#define OLB_3         0x0030
#define DT_IM         0x0000
#define DT_WAIT       0x0008
#define OCD_NORM      0x0000
#define OCD_ACT       0x0004
#define OG_1          0x0000
#define OG_2          0x0001
#define OG_3          0x0002
#define OG_4          0x0003

//Register IDs
enum reg{
	CommandA,
	CommandB,
	Config,
	SetupN,
	Setup1,
	Setup2,
	Setup3,
	Setup4,
	Setup5,
	Setup6,
	Setup7,
	Setup8,
	GenReg,
	Gain1,
	Gain2,
	Offset1,
	Offset2,
	Conv
};

//ADC IDs
enum adc{
	Outcast,
	Leader,
	Follower1,
	Follower2,
	Follower3,
	Follower4,
	Follower5,
	Follower6,
	Follower7,
	Follower8,
	Follower9,
	Follower10,
	Sweep,
	WhoInvitedThisGuy
};

class ComA {
public:
	//Commands
	ComA();
	void merge(void);
	void parse(void);
	void reset(void);
	void rolecall(bool);
	void print8(uint8_t);

	// Variables
	reg id = CommandA;
	String name = "Command A Register";

	// Register Values
	uint8_t out = 0;
	uint8_t ARA = 0;
	uint8_t CS = 0;
	uint8_t RW = 0;
	uint8_t RSB = 0;
};

class ComB {
public:
	//Commands
	ComB();
	void merge(void);
	void parse(void);
	void reset(void);
	void rolecall(bool);
	void print8(uint8_t);

	// Variables
	reg id = CommandB;
	String name = "Command B Register";
	
	//Register Values
	uint8_t out = 0;
	uint8_t MC = 0;
	uint8_t CSRP = 0;
	uint8_t CC = 0;
};

class ConfReg {
public:
	//Commands
	ConfReg();
	void merge(void);
	void parse(void);
	void reset(void);
	void rolecall(bool);
	void print32(uint32_t);
	
	//Variables
	reg id = Config;
	String name = "Configuration Register";
	
	bool* rst;
	bool* vrs;
	bool* ogs;
	bool* frs;
	
	double* A;
	
	//Register Values
	uint32_t out = 0;
	uint32_t PSS = 0;
	uint32_t PDW = 0;
	uint32_t RS = 0;
	uint32_t RV = 0;   // Read only
	uint32_t IS = 0;
	uint32_t GB = 0;
	uint32_t VRS = 0;
	uint32_t A1 = 0;
	uint32_t A0 = 0;
	uint32_t OLS = 0;
	uint32_t OGS = 0;
	uint32_t FRS = 0;
	
	int cfgVals[5] = {
		0,	//VRS
		0,	//SRS
		0,	//OGS
		0,	//PSS
		0	//PDW
	};
	
};

class Setup {
public:
	//Commands
	Setup();
	void merge(void);
	void parse(void);
	void reset(void);
	void rolecall(bool);
	void print16(uint16_t);
	
	//Variables
	reg id = SetupN;
	char* name = "UNKNOWN";
	String OcalType = "Uncalibrated";
	String GcalType = "Uncalibrated";
	
	bool Ocal = false;
	bool Gcal = false;
	
	bool* FRS;
	bool* OGS;
	bool bipolar = false;
	byte ch = 0;
	byte og = 0;
	byte srcCal = 0;
	int pgiaG = 0;
	
	double wr = 0.0;
	double kBi = 0.0;
	double Gadc = 0.0;
	
	//Register Values
	uint16_t out = 0;
	uint16_t CS = 0;
	uint16_t G = 0;
	uint16_t WR = 0;
	uint16_t UB = 0;
	uint16_t OL = 0;
	uint16_t DT = 0;
	uint16_t OCD = 0;
	uint16_t OG = 0;
	
	int stpVals[5] = {
		0,	//CS
		0,	//CAL
		0,	//GAIN
		0,	//WR
		0	//UB
	};
};

class GainReg {
public:
	//Commands
	GainReg();
	void merge(void);
	void parse(void);
	void reset(void);
	void rolecall(bool);
	void convert(void);
	void print32(uint32_t);
	
	//Variables
	reg id = GenReg;
	String calTypes[13] = {
		"Uncalibrated",
		"Self Gain",
		"System Gain - Full Scale - Reference Voltage",
		"System Gain - Full Scale - Voltage Divider",
		"System Gain - Full Scale - Known Mass",
		"System Gain - InAmp Gain - Reference Voltage",
		"System Gain - InAmp Gain - Voltage Divider",
		"Imported - Self Gain",
		"Imported - Full Scale - Reference Voltage",
		"Imported - Full Scale - Voltage Divider",
		"Imported - Full Scale - Known Mass",
		"Imported - InAmp Gain - Reference Voltage",
		"Imported - InAmp Gain - Voltage Divider"
	};
	String name = "UNKNOWN";
	String calType = calTypes[0];
	
	bool calibrated = false;
	
	double gain = 0.0;
	
	//Register Values
	uint32_t out = 0;
	uint32_t GV = 0;
	uint32_t g5p = 0;
	uint32_t g4p = 0;
	uint32_t g3p = 0;
	uint32_t g2p = 0;
	uint32_t g1p = 0;
	uint32_t g0 = 0;
	uint32_t g1 = 0;
	uint32_t g2 = 0;
	uint32_t g3 = 0;
	uint32_t g4 = 0;
	uint32_t g5 = 0;
	uint32_t g6 = 0;
	uint32_t g7 = 0;
	uint32_t g8 = 0;
	uint32_t g9 = 0;
	uint32_t g10 = 0;
	uint32_t g11 = 0;
	uint32_t g12 = 0;
	uint32_t g13 = 0;
	uint32_t g14 = 0;
	uint32_t g15 = 0;
	uint32_t g16 = 0;
	uint32_t g17 = 0;
	uint32_t g18 = 0;
	uint32_t g19 = 0;
	uint32_t g20 = 0;
	uint32_t g21 = 0;
	uint32_t g22 = 0;
	uint32_t g23 = 0;
	uint32_t g24 = 0;
};

class OffsetReg {
public:
	//Commands
	OffsetReg();
	void merge(void);
	void parse(void);
	void reset(void);
	void rolecall(bool);
	void convert(void);
	void print32(uint32_t);
	
	//Variables
	reg id = GenReg;
	String calTypes[11] {
		"Uncalibrated",
		"Self Offset",
		"System Offset - Input Short",
		"System Offset - Bias - Reference Voltage",
		"System Offset - Bias - Voltage Divider",
		"System Offset - Bias - Unloaded Mass",
		"Imported - Self Offset",
		"Imported - Input Short",
		"Imported - Bias - Reference Voltage",
		"Imported - Bias - Voltage Divider",
		"Imported - Bias - Unloaded Mass"
	};
	String name = "UNKNOWN";
	String calType = calTypes[0];
	
	bool calibrated = false;
	
	double offset = 0.0;
	
	//Register Values
	uint32_t out = 0;
	uint32_t OV = 0;
	uint32_t o1 = 0;
	uint32_t o2 = 0;
	uint32_t o3 = 0;
	uint32_t o4 = 0;
	uint32_t o5 = 0;
	uint32_t o6 = 0;
	uint32_t o7 = 0;
	uint32_t o8 = 0;
	uint32_t o9 = 0;
	uint32_t o10 = 0;
	uint32_t o11 = 0;
	uint32_t o12 = 0;
	uint32_t o13 = 0;
	uint32_t o14 = 0;
	uint32_t o15 = 0;
	uint32_t o16 = 0;
	uint32_t o17 = 0;
	uint32_t o18 = 0;
	uint32_t o19 = 0;
	uint32_t o20 = 0;
	uint32_t o21 = 0;
	uint32_t o22 = 0;
	uint32_t o23 = 0;
	uint32_t o24 = 0;
};

class ConvReg {
public:
	//Commands
	ConvReg();
	void parse(void);
	void reset(void);
	void rolecall(bool);
	void print32(uint32_t);
	
	//Variables
	reg id = Conv;
	reg setup = SetupN;
	
	String name = "Conversion Register";
	String setupName = "N/A";
	String GcalType = "Uncalibrated";
	String OcalType = "Uncalibrated";
	
	bool configured = false;
	bool ready = false;
	bool active = false;
	bool* bipolar;
	bool* vRefLow;
	bool cont = false;
	bool of = false;
	
	byte* ch;
	byte* og;
	byte* srcCal;
	
	double* A;
	double* kBi;
	double* Gadc;
	double* Gc;
	double* Oc;
	double* Vref;
	double* Va;
	double* Vcm;
	double* Vfs;
	double* Vspan;
	double* LSB;
	double* wr;
	
	//Register Values
	uint32_t in = 0;
	uint32_t CV = 0;
	uint32_t OF = 0;
	uint32_t CI = 0;
};

class CS5532 {
public:
	CS5532();
	void init();
	void flightCheck(void);
	void startClock(void);
	void startSPI(void);
	void stopSPI(void);
	void SPIS(bool);
	void SRS(bool);
	void VRSinit(bool);
	void CSRinit(bool);
	void updateConv(void);
	
	void parse(void);
	void reset(void);
	void rolecall(void);
	void rolecall(reg);
	void follow(void);
	void readReg(reg);
	void writeReg(reg,bool);
	
	bool calPrep(reg);
	void selfOffCal(reg, bool);
	void selfGainCal(reg, bool);
	void sysOffCal(reg, bool);
	void sysGainCal(reg, bool);
	void uploadOffset(reg, uint32_t, bool);
	void uploadGain(reg, uint32_t, bool);
	
	void singMeas(reg, bool);
	void contMeas(reg, bool);
	void stopMeas(bool);
	void scan(void);
	uint32_t scanMeas(void);
	int conv2Dec(void);
	double conv2Volt(void);
	double conv2F(void);
	
	void command8(uint8_t,bool);
	void hold8(uint8_t,bool);
	void command16(uint16_t,bool);
	void command32(bool);
	void command32(uint32_t,bool);
	void print8(uint8_t);
	void print16(uint16_t);
	void print32(uint32_t);
	
	byte* cs;
	byte* csAll;
	byte* esdo;
	byte* osc;
	
	MUX* mux;
	
	String setups[8] = {
		"Setup 1 - Use For System Offset Calibration",
		"Setup 2 - Use For System Gain Calibration",
		"Setup 3 - Use For Continuous Measurement (Calibrated)",
		"Setup 4 - Use For Continuous Measurement (Uncalibrated)",
		"Setup 5 - Use For Self Offset Calibration",
		"Setup 6 - Use For Self Gain Calibration",
		"Setup 7",
		"Setup 8"
	};
	String gainRegs[2] = {
		"Channel 1 Gain",
		"Channel 2 Gain"
	};
	String offsetRegs[2] = {
		"Channel 1 Offset",
		"Channel 2 Offset"
	};

	// Variables
	String name = "UNKNOWN";
	adc id = Outcast;
	byte* adcN;
	byte* adcSetN;
	byte* adcMic;
	
	//Flags
	bool* pinsSet;
	bool* clockStarted;
	bool timeKeeper = false;
	bool* SPIStarted;
	bool* ADCSerialInit;
	bool ADCReset = false;
	bool VRSset = false;
	bool OGSset = false;
	bool CSRset = false;
	bool ORset = false;
	bool GRset = false;
	bool measReady = false;
	bool* allClear;
	bool* verbose;
	volatile bool* sdof;
	
	//Conversion Variables
	float Fclock = 4.9152;
	float MCLK = 0.0;
	int fosc = 2e6;
	
	bool VRS = false;
	bool OGS = false;
	bool FRS = false;
	bool bipolar = false;
	byte ch = 0;
	byte og = 0;
	byte srcCal = 0;
	
	double A = 0;
	double kBi = 0;
	double Gadc = 1.0;
	double Gc = 1.0;
	double Oc = 0.0;
	double Vref = 3.0;
	double Va = 5.0;
	double Vcm = 1.5;
	double Vfs = 0.0;
	double Vspan = 0.0;
	double LSB = 0.0;
	double wr = 0.0;
	double m = 1.0;
	double b = 0.0;
	
	unsigned long timestamp = 0;
	uint32_t CV = 0;
	int CVD = 0;
	double CVV = 0.0;
	double CVF = 0.0;
	
	//Leader Settings
	uint32_t* CONF;
	uint32_t* CSR1;
	uint32_t* CSR2;
	uint32_t* CSR3;
	uint32_t* CSR4;
	uint32_t* CSR5;
	uint32_t* CSR6;
	uint32_t* CSR7;
	uint32_t* CSR8;
	
	//SPI Buffers
	uint32_t out = 0;
	uint16_t outA = 0;
	uint16_t outB = 0;
	uint32_t in = 0;
	uint16_t inA = 0;
	uint16_t inB = 0;
	
	// Registers
	ComA comA;
	ComB comB;
	ConfReg confReg;
	Setup setup1;
	Setup setup2;
	Setup setup3;
	Setup setup4;
	Setup setup5;
	Setup setup6;
	Setup setup7;
	Setup setup8;
	GainReg gainReg1;
	GainReg gainReg2;
	OffsetReg offsetReg1;
	OffsetReg offsetReg2;
	ConvReg convReg;
};

#endif