#ifndef _ROACHRUNNERCS5532_H_INCLUDED
	#define _ROACHRUNNERCS5532_H_INCLUDED

	#include <Arduino.h>
	#include <SPI.h>
	#include "RoachRunner_Digipot.h"
	#include "RoachRunner_Math.h"

	//Command Options - 8 bit
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
	#define MC_SINGLE   0x80
	#define MC_CONT     0xC0
	#define SETUP1      0x80
	#define SETUP2      0x88
	#define SETUP3      0x90
	#define SETUP4      0x98
	#define SETUP5      0xA0
	#define SETUP6      0xA8
	#define SETUP7      0xB0
	#define SETUP8      0xB8
	#define CC_SELF_OC  0x81
	#define CC_SELF_GC  0x82
	#define CC_SYS_OC   0x85
	#define CC_SYS_GC   0x86
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

	//Gain Register Options - 32 bit
	#define G_5_P_L       0x00000000
	#define G_4_P_L       0x00000000
	#define G_3_P_L       0x00000000
	#define G_2_P_L       0x00000000
	#define G_1_P_L       0x00000000
	#define G_0_L         0x00000000
	#define G_1_L         0x00000000
	#define G_2_L         0x00000000
	#define G_3_L         0x00000000
	#define G_4_L         0x00000000
	#define G_5_L         0x00000000
	#define G_6_L         0x00000000
	#define G_7_L         0x00000000
	#define G_8_L         0x00000000
	#define G_9_L         0x00000000
	#define G_10_L        0x00000000
	#define G_11_L        0x00000000
	#define G_12_L        0x00000000
	#define G_13_L        0x00000000
	#define G_14_L        0x00000000
	#define G_15_L        0x00000000
	#define G_16_L        0x00000000
	#define G_17_L        0x00000000
	#define G_18_L        0x00000000
	#define G_19_L        0x00000000
	#define G_20_L        0x00000000
	#define G_21_L        0x00000000
	#define G_22_L        0x00000000
	#define G_23_L        0x00000000
	#define G_24_L        0x00000000
	#define G_5_P_H       0x20000000
	#define G_4_P_H       0x10000000
	#define G_3_P_H       0x08000000
	#define G_2_P_H       0x04000000
	#define G_1_P_H       0x02000000
	#define G_0_H         0x01000000
	#define G_1_H         0x00800000
	#define G_2_H         0x00400000
	#define G_3_H         0x00200000
	#define G_4_H         0x00100000
	#define G_5_H         0x00080000
	#define G_6_H         0x00040000
	#define G_7_H         0x00020000
	#define G_8_H         0x00010000
	#define G_9_H         0x00008000
	#define G_10_H        0x00004000
	#define G_11_H        0x00002000
	#define G_12_H        0x00001000
	#define G_13_H        0x00000800
	#define G_14_H        0x00000400
	#define G_15_H        0x00000200
	#define G_16_H        0x00000100
	#define G_17_H        0x00000080
	#define G_18_H        0x00000040
	#define G_19_H        0x00000020
	#define G_20_H        0x00000010
	#define G_21_H        0x00000008
	#define G_22_H        0x00000004
	#define G_23_H        0x00000002
	#define G_24_H        0x00000001
	
	//Offset Register Options - 32 bit
	#define SIGN_P        0x00000000
	#define SIGN_N        0x80000000
	#define O_2_L         0x00000000
	#define O_3_L         0x00000000
	#define O_4_L         0x00000000
	#define O_5_L         0x00000000
	#define O_6_L         0x00000000
	#define O_7_L         0x00000000
	#define O_8_L         0x00000000
	#define O_9_L         0x00000000
	#define O_10_L        0x00000000
	#define O_11_L        0x00000000
	#define O_12_L        0x00000000
	#define O_13_L        0x00000000
	#define O_14_L        0x00000000
	#define O_15_L        0x00000000
	#define O_16_L        0x00000000
	#define O_17_L        0x00000000
	#define O_18_L        0x00000000
	#define O_19_L        0x00000000
	#define O_20_L        0x00000000
	#define O_21_L        0x00000000
	#define O_22_L        0x00000000
	#define O_23_L        0x00000000
	#define O_24_L        0x00000000
	#define O_2_H         0x40000000
	#define O_3_H         0x20000000
	#define O_4_H         0x10000000
	#define O_5_H         0x08000000
	#define O_6_H         0x04000000
	#define O_7_H         0x02000000
	#define O_8_H         0x01000000
	#define O_9_H         0x00800000
	#define O_10_H        0x00400000
	#define O_11_H        0x00200000
	#define O_12_H        0x00100000
	#define O_13_H        0x00080000
	#define O_14_H        0x00040000
	#define O_15_H        0x00020000
	#define O_16_H        0x00010000
	#define O_17_H        0x00008000
	#define O_18_H        0x00004000
	#define O_19_H        0x00002000
	#define O_20_H        0x00001000
	#define O_21_H        0x00000800
	#define O_22_H        0x00000400
	#define O_23_H        0x00000200
	#define O_24_H        0x00000100
	
	//Conversion Register Values - 32 bit
	#define C_23_L		0x00000000
	#define C_22_L		0x00000000
	#define C_21_L		0x00000000
	#define C_20_L		0x00000000
	#define C_19_L		0x00000000
	#define C_18_L		0x00000000
	#define C_17_L		0x00000000
	#define C_16_L		0x00000000
	#define C_15_L		0x00000000
	#define C_14_L		0x00000000
	#define C_13_L		0x00000000
	#define C_12_L		0x00000000
	#define C_11_L		0x00000000
	#define C_10_L		0x00000000
	#define C_9_L		0x00000000
	#define C_8_L		0x00000000
	#define C_7_L		0x00000000
	#define C_6_L		0x00000000
	#define C_5_L		0x00000000
	#define C_4_L		0x00000000
	#define C_3_L		0x00000000
	#define C_2_L		0x00000000
	#define C_1_L		0x00000000
	#define C_0_L		0x00000000
	#define C_23_H		0x80000000
	#define C_22_H		0x40000000
	#define C_21_H		0x20000000
	#define C_20_H		0x10000000
	#define C_19_H		0x08000000
	#define C_18_H		0x04000000
	#define C_17_H		0x02000000
	#define C_16_H		0x01000000
	#define C_15_H		0x00800000
	#define C_14_H		0x00400000
	#define C_13_H		0x00200000
	#define C_12_H		0x00100000
	#define C_11_H		0x00080000
	#define C_10_H		0x00040000
	#define C_9_H		0x00020000
	#define C_8_H		0x00010000
	#define C_7_H		0x00008000
	#define C_6_H		0x00004000
	#define C_5_H		0x00002000
	#define C_4_H		0x00001000
	#define C_3_H		0x00000800
	#define C_2_H		0x00000400
	#define C_1_H		0x00000200
	#define C_0_H		0x00000100
	#define OF_CLR		0x00000000
	#define OF_FLAG		0x00000004
	#define C_CH1		0x00000000
	#define C_CH2		0x00000001
	#define C_CH3		0x00000002
	#define C_CH4		0x00000003

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
		Qij,
		Q1X,
		Q1Y,
		Q1Z,
		Q2X,
		Q2Y,
		Q2Z,
		Q3X,
		Q3Y,
		Q3Z,
		Q4X,
		Q4Y,
		Q4Z
	};

	class ComA {
	public:
		// Commands
		ComA(){}
		ComA(String);
		void merge(void);
		void parse(void);
		void reset(void);
		void rolecall(void);
		void read8(uint8_t);
		void print8(uint8_t);

		// Register Values
		uint8_t out = 0;
		uint8_t ARA = 0;
		uint8_t CS = 0;
		uint8_t RW = 0;
		uint8_t RSB = 0;

		// Variables
		reg id = CommandA;
		String name = "CommandA Register";
		int d8[8];
	};

	class ComB {
	public:
		// Commands
		ComB(){}
		ComB(String);
		void merge(void);
		void parse(void);
		void reset(void);
		void rolecall(void);
		void read8(uint8_t);
		void print8(uint8_t);

		// Register Values
		uint8_t out = 0;
		uint8_t MC = 0;
		uint8_t CSRP = 0;
		uint8_t CC = 0;

		// Variables
		bool active = false;
		reg id = CommandB;
		String name = "CommandB Register";
		int d8[8];
	};

	class ConfReg {
	public:
		// Commands
		ConfReg(){}
		ConfReg(String);
		void merge(void);
		void parse(void);
		void reset(void);
		void rolecall(void);
		void read32(uint32_t);
		void print32(uint32_t);

		// Register Values
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

		// Variables
		float A = 0.0;
		bool rst = false;
		float Kf = 0.0;
		reg id = Config;
		String name = "Configuration Register";
		int d32[32];
	};

	class Setup {
	public:
		// Commands
		Setup(){}
		Setup(String);
		void merge(void);
		void parse(void);
		void reset(void);
		void rolecall(void);
		void read16(uint16_t);
		void print16(uint16_t);

		// Register Values
		uint16_t out = 0;
		uint16_t CS = 0;
		uint16_t G = 0;
		uint16_t WR = 0;
		uint16_t UB = 0;
		uint16_t OL = 0;
		uint16_t DT = 0;
		uint16_t OCD = 0;
		uint16_t OG = 0;

		// Variables
		String name = "UNKNOWN";
		bool Ocal = false;
		bool Gcal = false;
		String OcalType = "Uncalibrated";
		String GcalType = "Uncalibrated";
		reg id = SetupN;
		int FRS = 0;
		int OGS = 0;
		float wr = 120.0;
		float g = 0.0;
		int ch = 0;
		int og = 0;
		int src = 0;
		int d16[16];
	};

	class GainReg {
	public:
		// Commands
		GainReg(){}
		GainReg(String);
		void merge(void);
		void parse(void);
		void reset(void);
		void rolecall(void);
		void read32(uint32_t);
		void print32(uint32_t);
		void convert(void);

		// Variables
		String name = "Unknown";
		reg id = GenReg;
		int d32[32];
		bool calibrated = false;
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
		int minute = 0;
		int hour = 0;
		int day = 0;
		int month = 0;
		int year = 0;
		String calType = calTypes[0];
		float gain = 1.0;

		// Register Values
		uint32_t out = 0;
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
		// Commands
		OffsetReg(){}
		OffsetReg(String);
		void merge(void);
		void parse(void);
		void reset(void);
		void rolecall(void);
		void read32(uint32_t);
		void print32(uint32_t);
		void convert(void);

		// Variables
		String name = "UNKNOWN";
		reg id = GenReg;
		int d32[32];
		bool calibrated = false;
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
		int minute = 0;
		int hour = 0;
		int day = 0;
		int month = 0;
		int year = 0;
		String calType = calTypes[0];
		float offset = 0.0;

		// Register Values
		uint32_t out = 0;
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
		ConvReg(){}
		void rolecall(void);
		
		//Variables
		String name = "Conversion Register";
		reg id = Conv;
		bool configured = false;
		bool ready = false;
		bool active = false;
		bool bipolar = false;
		bool vRefLow = false;
		bool cont = false;
		bool of = false;
		reg setup = SetupN;
		String setupName = "N/A";
		int chMeas = 0;
		int chCal = 0;
		float G = 0.0;
		float A = 0.0;
		float kBi = 0.0;
		float Vref = 0.0;
		float Va = 0.0;
		float VinLB = 0.0;
		float VinUB = 0.0;
		float Vfs = 0.0;
		float Vspan = 0.0;
		float LSB = 0.0;
		float gainCal = 0.0;
		float offCal = 0.0;
		String GcalType = "Uncalibrated";
		String OcalType = "Uncalibrated";
		double measDec = 0;
		float measRat = 0.0;
		float measV = 0.0;
		float wr = 0;
		
		//Register Values
		uint32_t in32 = 0;
		uint32_t in24 = 0;
		uint32_t OF = 0;
		uint32_t CI = 0;
	};

	class CS5532 {
	public:
		// Commands
		CS5532(String, int, int, int);
		CS5532(String, int, int);
		CS5532();
		void init(void);
		void flightCheck(void);
		void startClock(void);
		void startSPI(void);
		void endSPI(void);
		void SPIS(void);
		void SRS(void);
		void VRSinit(void);
		void CSRinit(void);
		void calPrep(reg);
		void selfOffCal(reg);
		void sysOffCal(reg);
		void selfGainCal(reg);
		void sysGainCal(reg);
		void uploadOffset(reg, uint32_t);
		void uploadOffset(reg, float, bool);
		void uploadGain(reg, uint32_t);
		void singMeas(reg);
		void contMeas(reg);
		void stopMeas(void);
		void convert(void);
		void parse(void);
		void reset(void);
		void rolecall(void);
		void rolecall(reg);
		void updateConv(void);
		void read8(uint8_t);
		void read16(uint16_t);
		void read32(uint32_t);
		void command8(uint8_t);
		void hold8(uint8_t);
		void command16(uint16_t);
		void command32(void);
		void command32(uint32_t);
		void scan(void);
		void scanMeas(void);
		float scanMeas(int);
		void wordSplit(uint32_t);
		void readReg(reg);
		void writeReg(reg);

		// I/O
		int cs = -1;
		int sdoFlag = -1;
		int osc = -1;

		// Names
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
		adc id = Qij;
		
		bool pinsSet = false;
		bool clockStarted = false;
		bool timeKeeper = false;
		bool SPIStarted = false;
		bool ADCSerialInit = false;
		bool ADCReset = false;
		bool VRSset = false;
		bool OGSset = false;
		bool CSRset = false;
		bool ORset = false;
		bool GRset = false;
		bool measReady = false;
		bool allClear = false;
		bool prnt = true;
		
		float Fclock = 4.9152;
		float MCLK = 0.0;
		int FRS = 0;
		int OGS = 0;
		int src = 0;
		float A = 0;
		float G = 0;
		float V_S = 0;
		float V_O = 0;
		int fosc = 2e6;
		int bins10 = pow(2,24) - 1;
		int bins2C = pow(2,23) - 1;
		uint32_t measBin = 0;
		long meas2C = 0;
		
		int d8[8];
		int d16[16];
		int d24[24];
		int d32[32];
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