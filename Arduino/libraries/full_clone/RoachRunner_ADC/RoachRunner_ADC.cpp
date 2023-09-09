#include <Arduino.h>
#include <SPI.h>
#include "RoachRunner_ADC.h"
#include "RoachRunner_MUX.h"
#include "RoachRunner_Math.h"	//Tip: ALT+0 to fold all, ALT+SHIFT+0 to unfold all

ComA::ComA(){
	out = 0;
}

void ComA::merge(){
	out = ARA | CS | RW | RSB;
}

void ComA::parse(){
	ARA = out & 0x40;
	CS  = out & 0x30;
	RW  = out & 0x08;
	RSB = out & 0x07;
}

void ComA::reset(){
	out = 0;
	parse();
}

void ComA::rolecall(bool verbose){
	parse();
	if(verbose){
		Serial.println("\n------ Command A ------");
		Serial.print("OUT:\t");
		print8(out);
		parse();
		Serial.print("\nARA:\t");
		switch(ARA){
			case ARA_INDIV:
				Serial.println("0 - Access Registers Individually");
				break;
			case ARA_ARRAY:
				Serial.println("1 - Access All Registers as Arrays");
				break;
		}
		Serial.print("CS:\t");
		switch(CS){
			case CH1:
				Serial.println("00 - Physical Channel 1");
				break;
			case CH2:
				Serial.println("01 - Physical Channel 2");
				break;
			case CH3:
				Serial.println("10 - Physical Channel 3");
				break;
			case CH4:
				Serial.println("11 - Physical Channel 4");
				break;
		}
		Serial.print("RW:\t");
		switch(RW){
			case RW_W:
				Serial.println("0 - Write");
				break;
			case RW_R:
				Serial.println("1 - Read");
				break;
		}
		Serial.print("RSB:\t");
		switch(RSB){
			case RSB_OFFSET:
				Serial.println("001 - Offset Register");
				break;
			case RSB_GAIN:
				Serial.println("010 - Gain Register");
				break;
			case RSB_CONF:
				Serial.println("011 - Configuration Register");
				break;
			case RSB_CSR:
				Serial.println("101 - Channel-Setup Register");
				break;
			default:
				Serial.println("Invalid Word");
				break;
		}
	}
}

void ComA::print8(uint8_t _b){
	for(int i=0;i<8;++i){
		if((i>0)&&((i%4)==0)){
			Serial.print(" | ");
		}
		Serial.print(1&(_b>>(7-i)));
	}
}

ComB::ComB(){
	out = 0;
}

void ComB::merge(){
	out = MC | CSRP | CC;
}

void ComB::parse(){
	MC   = out & 0xC0;
	CSRP = out & 0xB8;
	CC   = out & 0x87;
}

void ComB::reset(){
	out = 0;
	parse();
}

void ComB::rolecall(bool verbose){
	parse();
	if(verbose){
		Serial.println("\n------ Command B ------");
		Serial.print("OUT:\t");
		print8(out);
		parse();
		Serial.print("\nMC:\t");
		switch(MC){
			case MC_SINGLE:
				Serial.println("0 - Perform a Single Conversion");
				break;
			case MC_CONT:
				Serial.println("1 - Perform Continuous Conversions");
				break;
		}
		Serial.print("CSRP:\t");
		switch(CSRP){
			case SETUP1:
				Serial.println("000 - Setup 1");
				break;
			case SETUP2:
				Serial.println("001 - Setup 2");
				break;
			case SETUP3:
				Serial.println("010 - Setup 3");
				break;
			case SETUP4:
				Serial.println("011 - Setup 4");
				break;
			case SETUP5:
				Serial.println("100 - Setup 5");
				break;
			case SETUP6:
				Serial.println("101 - Setup 6");
				break;
			case SETUP7:
				Serial.println("110 - Setup 7");
				break;
			case SETUP8:
				Serial.println("111 - Setup 8");
				break;
			default:
				Serial.println("Invalid Word");
		}
		Serial.print("CC:\t");
		switch(CC){
			case CC_SELF_OC:
				Serial.println("001 - Self-Offset Calibration");
				break;
			case CC_SELF_GC:
				Serial.println("010 - Self-Gain Calibration");
				break;
			case CC_SYS_OC:
				Serial.println("101 - System-Offset Calibration");
				break;
			case CC_SYS_GC:
				Serial.println("110 - System-Gain Calibration");
				break;
			case CC_NORM_CV:
				Serial.println("000 - Normal Conversion");
				break;
			default:
				Serial.println("Invalid Word");
				break;
		}
	}
}

void ComB::print8(uint8_t _b){
	for(int i=0;i<8;++i){
		if((i>0)&&((i%4)==0)){
			Serial.print(" | ");
		}
		Serial.print(1&(_b>>(7-i)));
	}
}

ConfReg::ConfReg(){
	out = 0;
}

void ConfReg::merge(){
	out = PSS | PDW | RS | RV | IS | GB | VRS | A1 | A0 | OLS | OGS | FRS;
}

void ConfReg::parse(){
	PSS = out & 0x80000000;
	PDW = out & 0x40000000;
	RS  = out & 0x20000000;
	RV  = out & 0x10000000;
	IS  = out & 0x08000000;
	GB  = out & 0x04000000;
	VRS = out & 0x02000000;
	A1  = out & 0x01000000;
	A0  = out & 0x00800000;
	OLS = out & 0x00400000;
	OGS = out & 0x00100000;
	FRS = out & 0x00080000;
}

void ConfReg::reset(){
	out = 0;
	parse();
}

void ConfReg::rolecall(bool verbose){
	parse();
	
	if(PSS){
		cfgVals[3] = 1;
	}
	else{
		cfgVals[3] = 0;
	}
	if(PDW){
		cfgVals[4] = 1;
	}
	else{
		cfgVals[4] = 0;
	}
	
	if(verbose){
		Serial.println("\n------ Configuration ------");
		Serial.print("OUT:\t");
		print32(out);
		parse();
		Serial.print("\nPSS:\t");
		if(PSS){
			Serial.print("1 - Sleep Mode");
		}
		else{
			Serial.print("0 - Standby Mode");
		}
		Serial.print("\nPDW:\t");
		if(PDW){
			Serial.print("1 - Activate Power-Down Mode");
		}
		else{
			Serial.print("0 - Normal Operation");
		}
		Serial.print("\nRS:\t");
		if(RS){
			Serial.print("1 - Active Reset Cycle");
		}
		else{
			Serial.print("0 - Normal Operation");
		}
		Serial.print("\nRV:\t");
	}
	if(RV){
		*rst = true;
		cfgVals[1] = 1;
	}
	if(verbose){
		if(RV){
			Serial.print("1 - System Reset Successful");
		}
		else{
			Serial.print("0 - Normal Operation");
		}
		Serial.print("\nIS:\t");
		if(IS){
			Serial.print("1 - Short Signal Input Pins Together");
		}
		else{
			Serial.print("0 - Normal Input");
		}
		Serial.print("\nGB:\t");
		if(GB){
			Serial.print("1 - Output CMOV Through A0");
		}
		else{
			Serial.print("0 - Normal Operation of A0 as Output Latch");
		}
		Serial.print("\nVRS:\t");
	}
	if(VRS){
		if(verbose){
			Serial.print("1 - (LOW: A = 1) 1V <= Vref <= 2.5V");
		}
		*A = 1.0;
		*vrs = true;
		cfgVals[0] = 1;
	}
	else{
		if(verbose){
			Serial.print("0 - (HIGH: A = 2) 2.5V < Vref <= [(VA+)-(VA-)]");
		}
		*A = 2.0;
		*vrs = false;
		cfgVals[0] = 0;
	}
	if(verbose){
		Serial.print("\nA1:\t");
		if(A1){
			Serial.print("1 - Logical HIGH");
		}
		else{
			Serial.print("0 - Logical LOW");
		}
		Serial.print("\nA0:\t");
		if(A0){
			Serial.print("1 - Logical HIGH");
		}
		else{
			Serial.print("0 - Logical LOW");
		}
		Serial.print("\nOLS:\t");
		if(OLS){
			Serial.print("1 - Configuration Register is Source for A0 and A1");
		}
		else{
			Serial.print("0 - Channel-Setup Register is Source for A0 and A1");
		}
		Serial.print("\nOGS:\t");
	}
	if(OGS){
		if(verbose){
			Serial.print("1 - Calibration Registers Based on OG1-OG0 from Referenced Setup");
		}
		*ogs = true;
		cfgVals[2] = 1;
	}
	else{
		if(verbose){
			Serial.print("0 - Calibration Registers Based on CS1-CS0 from Referenced Setup");
		}
		*ogs = false;
		cfgVals[2] = 0;
	}
	if(FRS){
		if(verbose){
			Serial.print("\nFRS:\t");
			Serial.println("1 - Scale Output Word Rates and Filter Characteristics by 5/6");
		}
		*frs = true;
	}
	else{
		if(verbose){
			Serial.print("\nFRS:\t");
			Serial.println("0 - Use Default Output Word Rates");
		}
		*frs = false;
	}
	//cfgSettings[0] = setVRS[cfgVals[0]];
	//cfgSettings[1] = setSRS[cfgVals[1]];
	//cfgSettings[2] = setOGS[cfgVals[2]];
	//cfgSettings[3] = setPSS[cfgVals[3]];
	//cfgSettings[4] = setPDW[cfgVals[4]];
}

void ConfReg::print32(uint32_t _b){
	for(int i=0;i<32;++i){
		if((i>0)&&((i%4)==0)){
			Serial.print(" | ");
		}
		Serial.print(1&(_b>>(31-i)));
	}
}

Setup::Setup(){
	out = 0;
}

void Setup::merge(){
	out = CS | G | WR | UB | OL | DT | OCD | OG;
}

void Setup::parse(){
	CS  = out & 0xC000;
	G   = out & 0x3800;
	WR  = out & 0x0780;
	UB  = out & 0x0040;
	OL  = out & 0x0030;
	DT  = out & 0x0008;
	OCD = out & 0x0004;
	OG  = out & 0x0003;
}

void Setup::reset(){
	out = 0;
	parse();
}

void Setup::rolecall(bool verbose){
	parse();
	
	String chStr = " ";
	String gStr = " ";
	String wrStr = " ";
	String ubStr = " ";
	String olStr = " ";
	String dtStr = " ";
	String ocdStr = " ";
	String ogStr = " ";
	
	switch(CS){
		case C1:
			ch = 1;
			stpVals[0] = 0;
			chStr = "00 - Physical Channel 1";
			break;
		case C2:
			ch = 2;
			stpVals[0] = 1;
			chStr = "01 - Physical Channel 2";
			break;
		case C3:
			ch = 3;
			stpVals[0] = 2;
			chStr = "10 - Physical Channel 3";
			break;
		case C4:
			ch = 4;
			stpVals[0] = 3;
			chStr = "11 - Physical Channel 4";
			break;
	}
	
	switch(G){
		case G1:
			Gadc = 1.0;
			stpVals[2] = 0;
			gStr = "000 - Gain = 1";
			break;
		case G2:
			Gadc = 2.0;
			stpVals[2] = 1;
			gStr = "001 - Gain = 2";
			break;
		case G4:
			Gadc = 4.0;
			stpVals[2] = 2;
			gStr = "010 - Gain = 4";
			break;
		case G8:
			Gadc = 8.0;
			stpVals[2] = 3;
			gStr = "011 - Gain = 8";
			break;
		case G16:
			Gadc = 16.0;
			stpVals[2] = 4;
			gStr = "100 - Gain = 16";
			break;
		case G32:
			Gadc = 32.0;
			stpVals[2] = 5;
			gStr = "101 - Gain = 32";
			break;
		case G64:
			Gadc = 64.0;
			stpVals[2] = 6;
			gStr = "110 - Gain = 64";
			break;
		default:
			gStr = "Invalid Entry";
			break;
	}
	
	switch(WR){
		case WR120:
			if(*FRS){
				wr = 100.0;
			}
			else{
				wr = 120.0;
			}
			wrStr = "0000 - WR = " + String(wr) + " Sps";
			stpVals[3] = 4;
			break;
		case WR60:
			if(*FRS){
				wr = 50.0;
			}
			else{
				wr = 60.0;
			}
			wrStr = "0001 - WR = " + String(wr) + " Sps";
			stpVals[3] = 3;
			break;
		case WR30:
			if(*FRS){
				wr = 25.0;
			}
			else{
				wr = 30.0;
			}
			wrStr = "0010 - WR = " + String(wr) + " Sps";
			stpVals[3] = 2;
			break;
		case WR15:
			if(*FRS){
				wr = 12.5;
			}
			else{
				wr = 15.0;
			}
			wrStr = "0011 - WR = " + String(wr) + " Sps";
			stpVals[3] = 1;
			break;
		case WR7_5:
			if(*FRS){
				wr = 6.25;
			}
			else{
				wr = 7.5;
			}
			wrStr = "0100 - WR = " + String(wr) + " Sps";
			stpVals[3] = 0;
			break;
		case WR3840:
			if(*FRS){
				wr = 3200.0;
			}
			else{
				wr = 3840.0;
			}
			wrStr = "1000 - WR = " + String(wr) + " Sps";
			stpVals[3] = 9;
			break;
		case WR1920:
			if(*FRS){
				wr = 1600.0;
			}
			else{
				wr = 1920.0;
			}
			wrStr = "1001 - WR = " + String(wr) + " Sps";
			stpVals[3] = 8;
			break;
		case WR960:
			if(*FRS){
				wr = 800.0;
			}
			else{
				wr = 960.0;
			}
			wrStr = "1010 - WR = " + String(wr) + " Sps";
			stpVals[3] = 7;
			break;
		case WR480:
			if(*FRS){
				wr = 400.0;
			}
			else{
				wr = 480.0;
			}
			wrStr = "1011 - WR = " + String(wr) + " Sps";
			stpVals[3] = 6;
			break;
		case WR240:
			if(*FRS){
				wr = 200.0;
			}
			else{
				wr = 240.0;
			}
			wrStr = "1100 - WR = " + String(wr) + " Sps";
			stpVals[3] = 5;
			break;
		default:
			wrStr = "Invalid Entry";
			break;
	}
	
	if(UB){
		ubStr = "1 - Unipolar Mode";
		bipolar = false;
		stpVals[4] = 1;
		kBi = 1.0;
	}
	else{
		ubStr = "0 - Bipolar Mode";
		bipolar = true;
		stpVals[4] = 0;
		kBi = 2.0;
	}
	
	switch(OL){
		case OLB_0:
			olStr = "00 - A0 = 0, A1 = 0";
			break;
		case OLB_1:
			olStr = "01 - A0 = 0, A1 = 1";
			break;
		case OLB_2:
			olStr = "10 - A0 = 1, A1 = 0";
			break;
		case OLB_3:
			olStr = "11 - A0 = 1, A1 = 1";
			break;
	}
	
	if(DT){
		dtStr = "1 - Wait ";
		if(FRS){
			dtStr = dtStr + "1536";
		}
		else{
			dtStr = dtStr + "1280";
		}
		dtStr = dtStr + " MCLK Tiks Before Starting Conversion";
	}
	else{
		dtStr = "0 - Begin Conversions Immediately";
	}
	
	if(OCD){
		ocdStr = "1 - Activate Current Source (Open Circuit Detect)";
	}
	else{
		ocdStr = "0 - Normal Mode";
	}
	
	switch(OG){
		case OG_1:
			og = 1;
			ogStr = "00 - Use Offset/Gain From Channel 1";
			break;
		case OG_2:
			og = 2;
			ogStr = "01 - Use Offset/Gain From Channel 2";
			break;
		case OG_3:
			og = 3;
			ogStr = "10 - Use Offset/Gain From Channel 3";
			break;
		case OG_4:
			og = 4;
			ogStr = "11 - Use Offset/Gain From Channel 4";
			break;
	}
	if(*OGS){
		ogStr = ogStr + " (In Use: OGS = 1)\n";
		srcCal = og;
		stpVals[1] = og-1;
	}
	else{
		ogStr = ogStr + " (Not In Use: OGS = 0)\n";
		srcCal = ch;
		stpVals[1] = ch-1;
	}
	
	//stpSettings[0] = setCS[stpVals[0]];
	//stpSettings[1] = setCAL[stpVals[1]];
	//stpSettings[2] = setGain[stpVals[2]];
	//stpSettings[3] = setWR[stpVals[3]];
	//stpSettings[4] = setUB[stpVals[4]];
	
	if(verbose){
		Serial.print("\n------ ");Serial.print(name);Serial.println(" ------");
		Serial.print("OUT:\t");print16(out);
		Serial.print("\nCS:\t");Serial.print(chStr);
		Serial.print("\nG:\t");Serial.print(gStr);
		Serial.print("\nWR:\t");Serial.print(wrStr);
		Serial.print("\nUB:\t");Serial.print(ubStr);
		Serial.print("\nOL:\t");Serial.print(olStr);
		Serial.print("\nDT:\t");Serial.print(dtStr);
		Serial.print("\nOCD:\t");Serial.print(ocdStr);
		Serial.print("\nOG:\t");Serial.print(ogStr);
	}
}

void Setup::print16(uint16_t _b){
	for(int i=0;i<16;++i){
		if((i>0)&&((i%4)==0)){
			Serial.print(" | ");
		}
		Serial.print(1&(_b>>(15-i)));
	}
}

GainReg::GainReg(){
	out = 0;
}

void GainReg::merge(){
	out = g5p | g4p | g3p | g2p | g1p | g0 | g1 | g2 | g3 | g4 | g5 | g6 | g7 | g8 | g9 | g10 | g11 | g12 | g13 | g14 | g15 | g16 | g17 | g18 | g19 | g20 | g21 | g22 | g23 | g24;
}

void GainReg::parse(){
	g5p = out & 0x20000000;
	g4p = out & 0x10000000;
	g3p = out & 0x08000000;
	g2p = out & 0x04000000;
	g1p = out & 0x02000000;
	g0  = out & 0x01000000;
	g1  = out & 0x00800000;
	g2  = out & 0x00400000;
	g3  = out & 0x00200000;
	g4  = out & 0x00100000;
	g5  = out & 0x00080000;
	g6  = out & 0x00040000;
	g7  = out & 0x00020000;
	g8  = out & 0x00010000;
	g9  = out & 0x00008000;
	g10 = out & 0x00004000;
	g11 = out & 0x00002000;
	g12 = out & 0x00001000;
	g13 = out & 0x00000800;
	g14 = out & 0x00000400;
	g15 = out & 0x00000200;
	g16 = out & 0x00000100;
	g17 = out & 0x00000080;
	g18 = out & 0x00000040;
	g19 = out & 0x00000020;
	g20 = out & 0x00000010;
	g21 = out & 0x00000008;
	g22 = out & 0x00000004;
	g23 = out & 0x00000002;
	g24 = out & 0x00000001;
}

void GainReg::reset(){
	out = 0;
	parse();
}

void GainReg::rolecall(bool verbose){
	parse();
	convert();
	
	if(verbose){
		Serial.print("\n------ ");Serial.print(name);Serial.println(" ------");
		Serial.print("OUT:\t");print32(out);
		Serial.print("\t( ");Serial.print(out);Serial.print(" )");
		Serial.print("\nGain:\t");Serial.print(gain, 6);Serial.print("\t[");sciNote(gain, 6);Serial.println("]");
		if(gain>40){
			Serial.println("\t\t!! WARNING: GAIN EXCEEDS MAX RECOMMENDED VALUE !!");
		}
		Serial.print("\n2^5:\t");
		if(g5p){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^4:\t");
		if(g4p){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^3:\t");
		if(g3p){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^2:\t");
		if(g2p){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^1:\t");
		if(g1p){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^0:\t");
		if(g0){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-1:\t");
		if(g1){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-2:\t");
		if(g2){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-3:\t");
		if(g3){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-4:\t");
		if(g4){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-5:\t");
		if(g5){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-6:\t");
		if(g6){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-7:\t");
		if(g7){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-8:\t");
		if(g8){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-9:\t");
		if(g9){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-10:\t");
		if(g10){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-11:\t");
		if(g11){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-12:\t");
		if(g12){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-13:\t");
		if(g13){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-14:\t");
		if(g14){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-15:\t");
		if(g15){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-16:\t");
		if(g16){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-17:\t");
		if(g17){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-18:\t");
		if(g18){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-19:\t");
		if(g19){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-20:\t");
		if(g20){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-21:\t");
		if(g21){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-22:\t");
		if(g22){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-23:\t");
		if(g23){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-24:\t");
		if(g24){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
	}
}

void GainReg::convert(){
	gain = 0.0;
	GV = out & 0x3FFFFFFF;
	for(int i=0;i<30;++i){
		if(1&(GV>>i)){
			gain = gain + pow(2,i-24);
		}
	}
}

void GainReg::print32(uint32_t _b){
	for(int i=0;i<32;++i){
		if((i>0)&&((i%4)==0)){
			Serial.print(" | ");
		}
		Serial.print(1&(_b>>(31-i)));
	}
}

OffsetReg::OffsetReg(){
	out = 0;
}

void OffsetReg::merge(){
	out = o1 | o2 | o3 | o4 | o5 | o6 | o7 | o8 | o9 | o10 | o11 | o12 | o13 | o14 | o15 | o16 | o17 | o18 | o19 | o20 | o21 | o22 | o23 | o24;
}

void OffsetReg::parse(){
	o1  = out & 0x80000000;
	o2  = out & 0x40000000;
	o3  = out & 0x20000000;
	o4  = out & 0x10000000;
	o5  = out & 0x08000000;
	o6  = out & 0x04000000;
	o7  = out & 0x02000000;
	o8  = out & 0x01000000;
	o9  = out & 0x00800000;
	o10 = out & 0x00400000;
	o11 = out & 0x00200000;
	o12 = out & 0x00100000;
	o13 = out & 0x00080000;
	o14 = out & 0x00040000;
	o15 = out & 0x00020000;
	o16 = out & 0x00010000;
	o17 = out & 0x00008000;
	o18 = out & 0x00004000;
	o19 = out & 0x00002000;
	o20 = out & 0x00001000;
	o21 = out & 0x00000800;
	o22 = out & 0x00000400;
	o23 = out & 0x00000200;
	o24 = out & 0x00000100;
}

void OffsetReg::reset(){
	out = 0;
	parse();
}

void OffsetReg::rolecall(bool verbose){
	parse();
	convert();
	
	if(verbose){
		Serial.print("\n------ ");Serial.print(name);Serial.println(" ------");
		Serial.print("OUT:\t");print32(out);Serial.print("\t( ");Serial.print(out);Serial.print(" )");
		Serial.print("\nOffset:\t");sciNote(offset, 6);
		Serial.print("\n-2^-1:\t");
		if(o1){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-2:\t");
		if(o2){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-3:\t");
		if(o3){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-4:\t");
		if(o4){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-5:\t");
		if(o5){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-6:\t");
		if(o6){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-7:\t");
		if(o7){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-8:\t");
		if(o8){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-9:\t");
		if(o9){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-10:\t");
		if(o10){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-11:\t");
		if(o11){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-12:\t");
		if(o12){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-13:\t");
		if(o13){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-14:\t");
		if(o14){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-15:\t");
		if(o15){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-16:\t");
		if(o16){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-17:\t");
		if(o17){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-18:\t");
		if(o18){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-19:\t");
		if(o19){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-20:\t");
		if(o20){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-21:\t");
		if(o21){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-22:\t");
		if(o22){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-23:\t");
		if(o23){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
		Serial.print("\n2^-24:\t");
		if(o24){
			Serial.print("1");
		}
		else{
			Serial.print("0");
		}
	}
}

void OffsetReg::convert(){
	offset = 0.0;
	OV = out >> 8;
	for(int i=0;i<31;++i){
		if(1&(out>>i)){
			offset = offset + pow(2.0,i-32);
		}
	}
	offset = offset - (1&(out>>31))*pow(2.0,-1);
}

void OffsetReg::print32(uint32_t _b){
	for(int i=0;i<32;++i){
		if((i>0)&&((i%4)==0)){
			Serial.print(" | ");
		}
		Serial.print(1&(_b>>(31-i)));
	}
}

ConvReg::ConvReg(){
	in = 0;
}

void ConvReg::parse(){
	CV = in & 0xFFFFFF00;
	OF = in & 0x00000004;
	CI = in & 0x00000003;
}

void ConvReg::reset(){
	configured = false;
	ready = false;
	active = false;
	*bipolar = false;
	*vRefLow = false;
	cont = false;
	of = false;
	setup = SetupN;
	setupName = "N/A";
	GcalType = "Uncalibrated";
	OcalType = "Uncalibrated";
	*ch = 0;
	*og = 0;
	*srcCal = 0;
	*A = 0.0;
	*kBi = 0.0;
	*Gadc = 0.0;
	*Gc = 0.0;
	*Oc = 0.0;
	*Vfs = 0.0;
	*Vspan = 0.0;
	*LSB = 0.0;
	*wr = 0.0;
}

void ConvReg::rolecall(bool verbose){
	parse();
	
	if(verbose){
		Serial.print("\n------ ");Serial.print(name);Serial.println(" ------");
		Serial.print("Configured:\t\t\t");
		if(configured){
			Serial.println("Yes");
		}
		else{
			Serial.println("No");
		}
		Serial.print("Ready:\t\t\t\t");
		if(ready){
			Serial.println("Yes");
		}
		else{
			Serial.println("No");
		}
		Serial.print("Active:\t\t\t\t");
		if(active){
			Serial.println("Yes");
		}
		else{
			Serial.println("No");
		}
		Serial.print("Sample Continuously:\t\t");
		if(cont){
			Serial.println("Yes");
		}
		else{
			Serial.println("No");
		}
		Serial.print("Polarity:\t\t\t");
		if(*bipolar){
			Serial.print("Bipolar:\tkBi = ");
			Serial.println(*kBi);
		}
		else{
			Serial.print("Unipolar:\tkBi = ");
			Serial.println(*kBi);
		}
		Serial.print("Vref Range:\t\t\t");
		if(*vRefLow){
			Serial.println("1V <= Vref <= 2.5V");
		}
		else{
			Serial.println("2.5V < Vref <= [(VA+)-(VA-)]");
		}
		Serial.print("Vref (Approx):\t\t\t");
		Serial.println(*Vref);
		Serial.print("VA (Approx):\t\t\t");
		Serial.println(*Va);
		Serial.print("\nSampling Gain:\t\t\tG = ");
		Serial.println(*Gadc);
		Serial.print("\t\t\t\tA = ");
		Serial.println(*A);
		Serial.print("\nFull-Scale Input Range:\t\tVfs = ");
		Serial.print(*Vfs,6);
		Serial.print(" V\n\t\t\t\tVspan = ");
		Serial.print(*Vspan,6);
		Serial.print(" V\n\t\t\t\tLSB = ");
		//Serial.println(LSB);
		sciNote(*LSB,6);
		Serial.print(" nV\n\nChannel-Setup Used:\t\t");
		Serial.println(setupName);
		Serial.print("Offset Calibration Method:\t");
		Serial.println(OcalType);
		Serial.print("Offset Calibration Value:\tO = ");
		sciNote(*Oc,6);
		Serial.print("\nGain Calibration Method:\t");
		Serial.println(GcalType);
		Serial.print("Gain Calibration Value:\t\tG = ");
		sciNote(*Gc,6);
		Serial.print("\nOutput Word Rate:\t\t");
		Serial.print(*wr);
		Serial.println(" Sps");
	}
}

void ConvReg::print32(uint32_t _b){
	for(int i=0;i<32;++i){
		if((i>0)&&((i%4)==0)){
			Serial.print(" | ");
		}
		Serial.print(1&(_b>>(31-i)));
	}
}

CS5532::CS5532(){
	out = 0;
	setup1.id = Setup1;
	setup2.id = Setup2;
	setup3.id = Setup3;
	setup4.id = Setup4;
	setup5.id = Setup5;
	setup6.id = Setup6;
	setup7.id = Setup7;
	setup8.id = Setup8;
	gainReg1.id = Gain1;
	gainReg2.id = Gain2;
	offsetReg1.id = Offset1;
	offsetReg2.id = Offset2;
	setup1.name = "Setup 1";
	setup2.name = "Setup 2";
	setup3.name = "Setup 3";
	setup4.name = "Setup 4";
	setup5.name = "Setup 5";
	setup6.name = "Setup 6";
	setup7.name = "Setup 7";
	setup8.name = "Setup 8";
	gainReg1.name = "Gain 1 Register";
	gainReg2.name = "Gain 2 Register";
	offsetReg1.name = "Offset 1 Register";
	offsetReg2.name = "Offset 2 Register";
}

void CS5532::init(){
	*adcN = *adcN + 1;
	id = (adc)*adcN;
	if(id==Leader){
		timeKeeper = true;
	}
	
	confReg.rst = &ADCReset;
	confReg.vrs = &VRS;
	confReg.ogs = &OGS;
	confReg.frs = &FRS;
	confReg.A = &A;
	
	setup1.FRS = &FRS;
	setup2.FRS = &FRS;
	setup3.FRS = &FRS;
	setup4.FRS = &FRS;
	setup5.FRS = &FRS;
	setup6.FRS = &FRS;
	setup7.FRS = &FRS;
	setup8.FRS = &FRS;
	setup1.OGS = &OGS;
	setup2.OGS = &OGS;
	setup3.OGS = &OGS;
	setup4.OGS = &OGS;
	setup5.OGS = &OGS;
	setup6.OGS = &OGS;
	setup7.OGS = &OGS;
	setup8.OGS = &OGS;
	
	convReg.bipolar = &bipolar;
	convReg.vRefLow = &VRS;
	convReg.ch = &ch;
	convReg.og = &og;
	convReg.srcCal = &srcCal;
	convReg.A = &A;
	convReg.kBi = &kBi;
	convReg.Gadc = &Gadc;
	convReg.Gc = &Gc;
	convReg.Oc = &Oc;
	convReg.Vref = &Vref;
	convReg.Va = &Va;
	convReg.Vcm = &Vcm;
	convReg.Vfs = &Vfs;
	convReg.Vspan = &Vspan;
	convReg.LSB = &LSB;
	convReg.wr = &wr;
	
	String spiStr = "\n";
	String pinStr = "\n";
	String clkStr1 = "\n";
	String clkStr2 = "\n";
	
	if(!*SPIStarted){
		startSPI();
		spiStr = "\n\t\t-------- " + String(name) + " Start SPI --------\n\nSPI Started\n";
	}
	if(!*pinsSet){
		pinStr = "\n WARNING: UNDECLARED PINS\n";
	}
	else{
		pinStr = "\nAll Pins Declared\n";
		clkStr1 = "Clock Started?\t\t\t" + String(*clockStarted) + "\nIs this ADC the Timekeeper?\t" + String(timeKeeper);
		if(timeKeeper && !*clockStarted){
			startClock();
			clkStr2 = "\tStarting Clock...\n\t" + String(MCLK,3) + " MHz MCLK Activated\nClock Active?\t" + String(*clockStarted) + "\n";
		}
	}
	
	if(*verbose){
		Serial.print("\n-----------------------------------------------------\nADC:\t");
		Serial.print(name);
		if(id==Leader){
			Serial.println("\tLeader");
		}
		else if(id==Sweep){
			Serial.println("\tSweep");
		}
		else{
			Serial.println("\tFollower");
		}
		Serial.println(setup1.name);
		Serial.println(setup2.name);
		Serial.println(setup3.name);
		Serial.println(setup4.name);
		Serial.println(setup5.name);
		Serial.println(setup6.name);
		Serial.println(setup7.name);
		Serial.println(setup8.name);
		Serial.println(offsetReg1.name);
		Serial.println(offsetReg2.name);
		Serial.println(gainReg1.name);
		Serial.println(gainReg2.name);
		
		Serial.print(spiStr);
		
		Serial.print("\n\t\t-------- ");Serial.print(name);Serial.println(" Clock Check --------");
		Serial.print(clkStr1);
		Serial.print(pinStr);
		Serial.print(clkStr2);
	}
}

void CS5532::flightCheck(){
	if(*pinsSet && *clockStarted && *SPIStarted && *ADCSerialInit && ADCReset && VRSset && OGSset && CSRset){
		measReady = true;
	}
	else{
		measReady = false;
	}
	convReg.configured = measReady;
	
	if(*verbose){
		Serial.print("\n-------- ");Serial.print(name);Serial.println(" Flight Check --------");
		Serial.print("Pins Declared?\t\t\t");Serial.println(*pinsSet);
		Serial.print("Clock Started?\t\t\t");Serial.println(*clockStarted);
		Serial.print("Arduino SPI ready?\t\t");Serial.println(*SPIStarted);
		Serial.print("ADC SPI Initialized?\t\t");Serial.println(*ADCSerialInit);
		Serial.print("ADC Reset Valid?\t\t");Serial.println(ADCReset);
		Serial.print("VRS Bit Set?\t\t\t");Serial.println(VRSset);
		Serial.print("OGS Bit Set?\t\t\t");Serial.println(OGSset);
		Serial.print("CSRs Set?\t\t\t");Serial.println(CSRset);
		Serial.print("Offset Register Updated?\t");Serial.print(ORset);
		
		if(ORset){
			Serial.print("\n");
		}
		else{
			Serial.println("\t!! WARNING: UNCALIBRATED OFFSET !!");
		}
		Serial.print("Gain Register Updated?\t\t");Serial.print(GRset);
		if(GRset){
			Serial.print("\n");
		}
		else{
			Serial.println("\t!! WARNING: UNCALIBRATED GAIN !!");
		}
		Serial.print("Ready to Measure?\t\t");Serial.println(measReady);
		Serial.print("Array Fully Prepped?\t\t");Serial.println(*allClear);
		Serial.println("------------------------");
	}
}

void CS5532::startClock(){
	int m = 84/Fclock;
	int m2 = m/2;
	int32_t mask_PWM_pin = digitalPinToBitMask(*osc);
	MCLK = float(84)/float(m);
	REG_PMC_PCER1 = 1<<4; // activate clock for PWM controller
	REG_PIOC_PDR |= mask_PWM_pin; // activate peripheral functions for pin (disables all PIO functionality)
	REG_PIOC_ABSR |= mask_PWM_pin; // choose peripheral option B
	REG_PWM_CLK = 0; // choose clock rate, 0 → full MCLK as reference 84MHz
	REG_PWM_CMR6 = 0<<9; // select clock and polarity for PWM channel (pin7) → (CPOL = 0)
	REG_PWM_CPRD6 = m; // initialize PWM period → T = value/84MHz (value: up to 16bit), value=8 → 10.5MHz
	REG_PWM_CDTY6 = m2; // initialize duty cycle, REG_PWM_CPRD6 / value = duty cycle, for 8/4 = 50%
	REG_PWM_ENA = 1<<6; // enable PWM on PWM channel (pin 7 = PWML6)
	*clockStarted = true;
}

void CS5532::startSPI(){
	if(!*SPIStarted){
		//SPI.begin();
		SPI.beginTransaction(SPISettings(fosc,MSBFIRST,SPI_MODE0));
		*SPIStarted = true;
	}
}

void CS5532::stopSPI(){
	if(*SPIStarted){
		SPI.endTransaction();
		*SPIStarted = false;
	}
}

void CS5532::SPIS(bool all){
	if(!*ADCSerialInit){
		for(int i=0;i<16;++i){
			command8(SYNC1,all);
		}
		command8(SYNC0,all);
		*ADCSerialInit = true;
		
		if(*verbose){
			Serial.print("\n\t\t-------- ");Serial.print(name);Serial.println(" SPIS --------");
			Serial.print("\nSYNC Commands Sent: ADC ");Serial.print(name);Serial.println("'s Serial Port Initialized");
		}
	}
}

void CS5532::SRS(bool all){
	if(!ADCReset){
		if(*verbose){
			Serial.print("\n\t\t-------- ");Serial.print(name);Serial.println(" SRS --------");
		}
		confReg.RS = RS_RESET;
		writeReg(Config,all);
		
		delayMicroseconds(20);
		
		confReg.RS = RS_NORM;
		writeReg(Config,all);
		
		readReg(Config);
		
		if(confReg.RV){
			ADCReset = true;
		}
	}
}

void CS5532::VRSinit(bool all){
	if(*verbose){
		Serial.print("\n\t\t-------- ");Serial.print(name);Serial.println(" Set VRS --------");
	}
	confReg.reset();
	confReg.VRS = VRS_HIGH;
	confReg.OGS = OGS_OG;
	writeReg(Config,all);
	
	readReg(Config);
	
	if(confReg.VRS==VRS_HIGH){
		VRSset = true;
	}
	else{
		VRSset = false;
	}
	
	if(confReg.OGS==OGS_OG){
		OGSset = true;
	}
	else{
		OGSset = false;
	}
}

void CS5532::CSRinit(bool all){
	if(*verbose){
		Serial.print("\n\t\t-------- ");Serial.print(name);Serial.println(" Set CSRs --------");
	}
	setup1.reset();
	setup2.reset();
	setup3.reset();
	setup4.reset();
	setup5.reset();
	setup6.reset();
	setup7.reset();
	setup8.reset();
	
	setup1.name = "Trial Default";
	setup2.name = "Zero Offset Cal";
	setup3.name = "Set Full Scale";
	setup4.name = "Reset PGIAs";
	setup5.name = "Sensitivity Cal";
	setup6.name = "Vibration Scan";
	setup7.name = "Custom1";
	setup8.name = "Custom2",
	
	setup1.CS = C1;
	setup2.CS = C1;
	setup3.CS = C1;
	setup4.CS = C1;
	setup5.CS = C1;
	setup6.CS = C1;
	setup7.CS = C1;
	setup8.CS = C1;
	
	setup1.G = G4;
	setup2.G = G4;
	setup3.G = G4;
	setup4.G = G16;
	setup5.G = G8;
	setup6.G = G16;
	setup7.G = G32;
	setup8.G = G64;
	
	setup1.WR = WR1920;
	setup2.WR = WR240;
	setup3.WR = WR240;
	setup4.WR = WR960;
	setup5.WR = WR3840;
	setup6.WR = WR3840;
	setup7.WR = WR3840;
	setup8.WR = WR3840;
	
	setup1.UB = UB_BI;
	setup2.UB = UB_BI;
	setup3.UB = UB_BI;
	setup4.UB = UB_BI;
	setup5.UB = UB_BI;
	setup6.UB = UB_BI;
	setup7.UB = UB_BI;
	setup8.UB = UB_BI;
	
	setup1.OL = OLB_0;
	setup2.OL = OLB_0;
	setup3.OL = OLB_0;
	setup4.OL = OLB_0;
	setup5.OL = OLB_0;
	setup6.OL = OLB_0;
	setup7.OL = OLB_0;
	setup8.OL = OLB_0;
	
	setup1.DT = DT_IM;
	setup2.DT = DT_IM;
	setup3.DT = DT_IM;
	setup4.DT = DT_IM;
	setup5.DT = DT_IM;
	setup6.DT = DT_IM;
	setup7.DT = DT_IM;
	setup8.DT = DT_IM;
	
	setup1.OCD = OCD_NORM;
	setup2.OCD = OCD_NORM;
	setup3.OCD = OCD_NORM;
	setup4.OCD = OCD_NORM;
	setup5.OCD = OCD_NORM;
	setup6.OCD = OCD_NORM;
	setup7.OCD = OCD_NORM;
	setup8.OCD = OCD_NORM;
	
	setup1.OG = OG_1;
	setup2.OG = OG_1;
	setup3.OG = OG_1;
	setup4.OG = OG_1;
	setup5.OG = OG_1;
	setup6.OG = OG_1;
	setup7.OG = OG_1;
	setup8.OG = OG_1;
	
	setup1.pgiaG = 2;
	setup2.pgiaG = 2;
	setup3.pgiaG = 2;
	setup4.pgiaG = 0;
	setup5.pgiaG = 0;
	setup6.pgiaG = 0;
	setup7.pgiaG = 0;
	setup8.pgiaG = 0;
	
	setup1.merge();
	setup2.merge();
	setup3.merge();
	setup4.merge();
	setup5.merge();
	setup6.merge();
	setup7.merge();
	setup8.merge();
	
	writeReg(Setup1,all);
	writeReg(Setup2,all);
	writeReg(Setup3,all);
	writeReg(Setup4,all);
	writeReg(Setup5,all);
	writeReg(Setup6,all);
	writeReg(Setup7,all);
	writeReg(Setup8,all);
	
	readReg(Setup1);
	readReg(Setup2);
	readReg(Setup3);
	readReg(Setup4);
	readReg(Setup5);
	readReg(Setup6);
	readReg(Setup7);
	readReg(Setup8);
	
	CSRset = true;
}

void CS5532::updateConv(){
	confReg.rolecall(*verbose);
	bool _go = true;
	switch(convReg.setup){
		case Setup1:
			ch = setup1.ch;
			og = setup1.og;
			srcCal = setup1.srcCal;
			Gadc = setup1.Gadc;
			wr = setup1.wr;
			convReg.setupName = setup1.name;
			bipolar = setup1.bipolar;
			kBi = setup1.kBi;
			break;
		case Setup2:
			ch = setup2.ch;
			og = setup2.og;
			srcCal = setup2.srcCal;
			Gadc = setup2.Gadc;
			wr = setup2.wr;
			convReg.setupName = setup2.name;
			bipolar = setup2.bipolar;
			kBi = setup2.kBi;
			break;
		case Setup3:
			ch = setup3.ch;
			og = setup3.og;
			srcCal = setup3.srcCal;
			Gadc = setup3.Gadc;
			wr = setup3.wr;
			convReg.setupName = setup3.name;
			bipolar = setup3.bipolar;
			kBi = setup3.kBi;
			break;
		case Setup4:
			ch = setup4.ch;
			og = setup4.og;
			srcCal = setup4.srcCal;
			Gadc = setup4.Gadc;
			wr = setup4.wr;
			convReg.setupName = setup4.name;
			bipolar = setup4.bipolar;
			kBi = setup4.kBi;
			break;
		case Setup5:
			ch = setup5.ch;
			og = setup5.og;
			srcCal = setup5.srcCal;
			Gadc = setup5.Gadc;
			wr = setup5.wr;
			convReg.setupName = setup5.name;
			bipolar = setup5.bipolar;
			kBi = setup5.kBi;
			break;
		case Setup6:
			ch = setup6.ch;
			og = setup6.og;
			srcCal = setup6.srcCal;
			Gadc = setup6.Gadc;
			wr = setup6.wr;
			convReg.setupName = setup6.name;
			bipolar = setup6.bipolar;
			kBi = setup6.kBi;
			break;
		case Setup7:
			ch = setup7.ch;
			og = setup7.og;
			srcCal = setup7.srcCal;
			Gadc = setup7.Gadc;
			wr = setup7.wr;
			convReg.setupName = setup7.name;
			bipolar = setup7.bipolar;
			kBi = setup7.kBi;
			break;
		case Setup8:
			ch = setup8.ch;
			og = setup8.og;
			srcCal = setup8.srcCal;
			Gadc = setup8.Gadc;
			wr = setup8.wr;
			convReg.setupName = setup8.name;
			bipolar = setup8.bipolar;
			kBi = setup8.kBi;
			break;
		default:
			_go = false;
			if(*verbose){
				Serial.println("Invalid Entry");
			}
			break;
	}
	switch(srcCal){
		case 1:
			readReg(Gain1);
			readReg(Offset1);
			Gc = gainReg1.gain;
			Oc = offsetReg1.offset;
			convReg.GcalType = gainReg1.calType;
			convReg.OcalType = offsetReg1.calType;
			break;
		case 2:
			readReg(Gain2);
			readReg(Offset2);
			Gc = gainReg2.gain;
			Oc = offsetReg2.offset;
			convReg.GcalType = gainReg2.calType;
			convReg.OcalType = offsetReg2.calType;
			break;
		default:
			if(*verbose){
				Serial.println("Invalid Register");
			}
			_go = false;
			break;
	}
	if(convReg.OcalType==offsetReg1.calTypes[0]){
		ORset = false;
	}
	else{
		ORset = true;
	}
	if(convReg.GcalType==gainReg1.calTypes[0]){
		GRset = false;
	}
	else{
		GRset = true;
	}
	startSPI();
	flightCheck();
	if(_go && convReg.configured){
		convReg.ready = true;
	}
	else{
		convReg.ready = false;
	}
	Vfs = Vref/(Gadc*A*Gc);
	Vspan = kBi*Vfs;
	LSB = (pow(10,9)/pow(2,24))*Vspan;
	convReg.rolecall(*verbose);
}
	
void CS5532::parse(){
	comA.parse();
	comB.parse();
	confReg.parse();
	setup1.parse();
	setup2.parse();
	setup3.parse();
	setup4.parse();
	setup5.parse();
	setup6.parse();
	setup7.parse();
	setup8.parse();
	gainReg1.parse();
	gainReg2.parse();
	offsetReg1.parse();
	offsetReg2.parse();
}

void CS5532::reset(){
	comA.reset();
	comB.reset();
	confReg.reset();
	setup1.reset();
	setup2.reset();
	setup3.reset();
	setup4.reset();
	setup5.reset();
	setup6.reset();
	setup7.reset();
	setup8.reset();
	gainReg1.reset();
	gainReg2.reset();
	offsetReg1.reset();
	offsetReg2.reset();
	convReg.reset();
}

void CS5532::rolecall(){
	comA.rolecall(*verbose);
	comB.rolecall(*verbose);
	confReg.rolecall(*verbose);
	setup1.rolecall(*verbose);
	setup2.rolecall(*verbose);
	setup3.rolecall(*verbose);
	setup4.rolecall(*verbose);
	setup5.rolecall(*verbose);
	setup6.rolecall(*verbose);
	setup7.rolecall(*verbose);
	setup8.rolecall(*verbose);
	offsetReg1.rolecall(*verbose);
	offsetReg2.rolecall(*verbose);
	gainReg1.rolecall(*verbose);
	gainReg2.rolecall(*verbose);
	convReg.rolecall(*verbose);
}

void CS5532::rolecall(reg r){
	switch(r){
		case CommandA:
			comA.rolecall(*verbose);
			break;
		case CommandB:
			comB.rolecall(*verbose);
			break;
		case Config:
			confReg.rolecall(*verbose);
			break;
		case Setup1:
			setup1.rolecall(*verbose);
			break;
		case Setup2:
			setup2.rolecall(*verbose);
			break;
		case Setup3:
			setup3.rolecall(*verbose);
			break;
		case Setup4:
			setup4.rolecall(*verbose);
			break;
		case Setup5:
			setup5.rolecall(*verbose);
			break;
		case Setup6:
			setup6.rolecall(*verbose);
			break;
		case Setup7:
			setup7.rolecall(*verbose);
			break;
		case Setup8:
			setup8.rolecall(*verbose);
			break;
		case Offset1:
			offsetReg1.rolecall(*verbose);
			break;
		case Offset2:
			offsetReg2.rolecall(*verbose);
			break;
		case Gain1:
			gainReg1.rolecall(*verbose);
			break;
		case Gain2:
			gainReg2.rolecall(*verbose);
			break;
		case Conv:
			convReg.rolecall(*verbose);
			break;
		default:
			if(*verbose){
				Serial.println("Invalid Entry");
			}
			break;
	}
}

void CS5532::follow(void){
	if(*verbose){
		Serial.print("\n\t\t-------- ");Serial.print(name);Serial.println(": Follow The Leader --------");
	}
	readReg(Config);
	readReg(Setup1);
	readReg(Setup2);
	readReg(Setup3);
	readReg(Setup4);
	readReg(Setup5);
	readReg(Setup6);
	readReg(Setup7);
	readReg(Setup8);
	
	if(confReg.out==*CONF){
		VRSset = true;
		OGSset = true;
	}
	bool _go = true;
	if(setup1.out!=*CSR1){
		_go = false;
	}
	if(setup2.out!=*CSR2){
		_go = false;
	}
	if(setup3.out!=*CSR3){
		_go = false;
	}
	if(setup4.out!=*CSR4){
		_go = false;
	}
	if(setup5.out!=*CSR5){
		_go = false;
	}
	if(setup6.out!=*CSR6){
		_go = false;
	}
	if(setup7.out!=*CSR7){
		_go = false;
	}
	if(setup8.out!=*CSR8){
		_go = false;
	}
	if(_go){
		CSRset = true;
	}
}

void CS5532::readReg(reg r){
	startSPI();
	comA.reset();
	comA.RW = RW_R;
	switch(r){
		case Config:
			comA.RSB = RSB_CONF;
			break;
		case Setup1:
			comA.CS = CH1;
			comA.RSB = RSB_CSR;
			break;
		case Setup2:
			comA.CS = CH1;
			comA.RSB = RSB_CSR;
			break;
		case Setup3:
			comA.CS = CH2;
			comA.RSB = RSB_CSR;
			break;
		case Setup4:
			comA.CS = CH2;
			comA.RSB = RSB_CSR;
			break;
		case Setup5:
			comA.CS = CH3;
			comA.RSB = RSB_CSR;
			break;
		case Setup6:
			comA.CS = CH3;
			comA.RSB = RSB_CSR;
			break;
		case Setup7:
			comA.CS = CH4;
			comA.RSB = RSB_CSR;
			break;
		case Setup8:
			comA.CS = CH4;
			comA.RSB = RSB_CSR;
			break;
		case Gain1:
			comA.CS = CH1;
			comA.RSB = RSB_GAIN;
			break;
		case Gain2:
			comA.CS = CH2;
			comA.RSB = RSB_GAIN;
			break;
		case Offset1:
			comA.CS = CH1;
			comA.RSB = RSB_OFFSET;
			break;
		case Offset2:
			comA.CS = CH2;
			comA.RSB = RSB_OFFSET;
			break;
		default:
			if(*verbose){
				Serial.println("Invalid Entry");
			}
			break;
	}
	comA.merge();
	if(*verbose){
		Serial.print("\n"+name+": ");
	}
	comA.rolecall(*verbose);
	hold8(comA.out,0);
	scan();
	comA.reset();
	
	switch(r){
		case(Config):
			confReg.out = in;
			if(id==Leader){
				*CONF = confReg.out;
			}
			break;
		case(Setup1):
			setup1.out = inA;
			if(id==Leader){
				*CSR1 = setup1.out;
			}
			break;
		case(Setup2):
			setup2.out = inB;
			if(id==Leader){
				*CSR2 = setup2.out;
			}
			break;
		case(Setup3):
			setup3.out = inA;
			if(id==Leader){
				*CSR3 = setup3.out;
			}
			break;
		case(Setup4):
			setup4.out = inB;
			if(id==Leader){
				*CSR4 = setup4.out;
			}
			break;
		case(Setup5):
			setup5.out = inA;
			if(id==Leader){
				*CSR5 = setup5.out;
			}
			break;
		case(Setup6):
			setup6.out = inB;
			if(id==Leader){
				*CSR6 = setup6.out;
			}
			break;
		case(Setup7):
			setup7.out = inA;
			if(id==Leader){
				*CSR7 = setup7.out;
			}
			break;
		case(Setup8):
			setup8.out = inB;
			if(id==Leader){
				*CSR8 = setup8.out;
			}
			break;
		case(Gain1):
			gainReg1.out = in;
			break;
		case(Gain2):
			gainReg2.out = in;
			break;
		case(Offset1):
			offsetReg1.out = in;
			break;
		case(Offset2):
			offsetReg2.out = in;
			break;
		default:
			if(*verbose){
				Serial.println("Invalid Entry");
			}
			break;
	}
	if(*verbose){
		Serial.print("\n"+name+": ");
	}
	rolecall(r);
	stopSPI();
}

void CS5532::writeReg(reg r, bool all){
	startSPI();
	comA.reset();
	comA.RW = RW_W;
	out = 0;
	outA = 0;
	outB = 0;
	bool whole = false;
	switch(r){
		case Config:
			whole = true;
			comA.RSB = RSB_CONF;
			confReg.merge();
			out = confReg.out;
			break;
		case Setup1:
			comA.CS = CH1;
			comA.RSB = RSB_CSR;
			setup1.merge();
			setup2.merge();
			outA = setup1.out;
			outB = setup2.out;
			break;
		case Setup2:
			comA.CS = CH1;
			comA.RSB = RSB_CSR;
			setup1.merge();
			setup2.merge();
			outA = setup1.out;
			outB = setup2.out;
			break;
		case Setup3:
			comA.CS = CH2;
			comA.RSB = RSB_CSR;
			setup3.merge();
			setup4.merge();
			outA = setup3.out;
			outB = setup4.out;
			break;
		case Setup4:
			comA.CS = CH2;
			comA.RSB = RSB_CSR;
			setup3.merge();
			setup4.merge();
			outA = setup3.out;
			outB = setup4.out;
			break;
		case Setup5:
			comA.CS = CH3;
			comA.RSB = RSB_CSR;
			setup5.merge();
			setup6.merge();
			outA = setup5.out;
			outB = setup6.out;
			break;
		case Setup6:
			comA.CS = CH3;
			comA.RSB = RSB_CSR;
			setup5.merge();
			setup6.merge();
			outA = setup5.out;
			outB = setup6.out;
			break;
		case Setup7:
			comA.CS = CH4;
			comA.RSB = RSB_CSR;
			setup7.merge();
			setup8.merge();
			outA = setup7.out;
			outB = setup8.out;
			break;
		case Setup8:
			comA.CS = CH4;
			comA.RSB = RSB_CSR;
			setup7.merge();
			setup8.merge();
			outA = setup7.out;
			outB = setup8.out;
			break;
		case Gain1:
			whole = true;
			comA.CS = CH1;
			comA.RSB = RSB_GAIN;
			gainReg1.merge();
			out = gainReg1.out;
			break;
		case Gain2:
			whole = true;
			comA.CS = CH2;
			comA.RSB = RSB_GAIN;
			gainReg2.merge();
			out = gainReg2.out;
			break;
		case Offset1:
			whole = true;
			comA.CS = CH1;
			comA.RSB = RSB_OFFSET;
			offsetReg1.merge();
			out = offsetReg1.out;
			break;
		case Offset2:
			whole = true;
			comA.CS = CH2;
			comA.RSB = RSB_OFFSET;
			offsetReg2.merge();
			out = offsetReg2.out;
			break;
		default:
			if(*verbose){
				Serial.println("Invalid Entry");
			}
			break;
	}
	comA.merge();
	if(*verbose){
		Serial.print("\n"+name+": ");
	}
	comA.rolecall(*verbose);
	if(*verbose){
		Serial.print("\n"+name+": ");
	}
	rolecall(r);
	if(whole){
		hold8(comA.out,all);
		command32(out,all);
	}
	else{
		hold8(comA.out,all);
		command32(all);
	}
	comA.reset();
	stopSPI();
}
	
bool CS5532::calPrep(reg r){
	bool _b = false;
	switch(r){
		case Setup1:
			srcCal = setup1.srcCal;
			comB.CSRP = SETUP1;
			_b = true;
			break;
		case Setup2:
			srcCal = setup2.srcCal;
			comB.CSRP = SETUP2;
			_b = true;
			break;
		case Setup3:
			srcCal = setup3.srcCal;
			comB.CSRP = SETUP3;
			_b = true;
			break;
		case Setup4:
			srcCal = setup4.srcCal;
			comB.CSRP = SETUP4;
			_b = true;
			break;
		case Setup5:
			srcCal = setup5.srcCal;
			comB.CSRP = SETUP5;
			_b = true;
			break;
		case Setup6:
			srcCal = setup6.srcCal;
			comB.CSRP = SETUP6;
			_b = true;
			break;
		case Setup7:
			srcCal = setup7.srcCal;
			comB.CSRP = SETUP7;
			_b = true;
			break;
		case Setup8:
			srcCal = setup8.srcCal;
			comB.CSRP = SETUP8;
			_b = true;
			break;
		default:
			if(*verbose){
				Serial.println("Invalid Entry");
			}
			break;
	}
	return _b;
}

void CS5532::selfOffCal(reg r, bool all){
	bool _go = true;
	reg r2 = SetupN;
	if(*verbose){
		Serial.print("\n\t\t-------- ");Serial.print(name);Serial.println(" Self Offset Cal --------");
	}
	comB.reset();
	comB.CC = CC_SELF_OC;
	if(!calPrep(r)){
		if(*verbose){
			Serial.println("Self-Offset Calibration Failed!");
		}
		_go = false;
	}
	if(_go){
		switch(srcCal){
			case 1:
				r2 = Offset1;
				break;
			case 2:
				r2 = Offset2;
				break;
			default:
				if(*verbose){
					Serial.println("Register Not Available");
				}
				_go = false;
				break;
		}
	}
	if(_go){
		comB.merge();
		comB.rolecall(*verbose);
		*sdof = false;
		hold8(comB.out,all);
		while(!*sdof){}
		readReg(r2);
		rolecall(r2);
		comB.reset();
		String calType = offsetReg1.calTypes[1];
		switch(r2){
			case Offset1:
				offsetReg1.calibrated = true;
				offsetReg1.calType = calType;
				break;
			case Offset2:
				offsetReg2.calibrated = true;
				offsetReg2.calType = calType;
				break;
			default:
				if(*verbose){
					Serial.println("Register Not Available");
				}
				break;
		}
		switch(r){
			case Setup1:
				setup1.Ocal = true;
				setup1.OcalType = calType;
				break;
			case Setup2:
				setup2.Ocal = true;
				setup2.OcalType = calType;
				break;
			case Setup3:
				setup3.Ocal = true;
				setup3.OcalType = calType;
				break;
			case Setup4:
				setup4.Ocal = true;
				setup4.OcalType = calType;
				break;
			case Setup5:
				setup5.Ocal = true;
				setup5.OcalType = calType;
				break;
			case Setup6:
				setup6.Ocal = true;
				setup6.OcalType = calType;
				break;
			case Setup7:
				setup7.Ocal = true;
				setup7.OcalType = calType;
				break;
			case Setup8:
				setup8.Ocal = true;
				setup8.OcalType = calType;
				break;
			default:
				if(*verbose){
					Serial.println("Invalid Entry - Must Input Setup");
				}
				break;
		}
	}
}

void CS5532::selfGainCal(reg r, bool all){
	bool _go = true;
	reg r2 = SetupN;
	if(*verbose){
		Serial.print("\n\t\t-------- ");Serial.print(name);Serial.println(" Self Gain Cal --------");
	}
	comB.reset();
	comB.CC = CC_SELF_GC;
	if(!calPrep(r)){
		if(*verbose){
			Serial.println("Self-Gain Calibration Failed!");
		}
		_go = false;
	}
	if(_go){
		switch(srcCal){
			case 1:
				r2 = Gain1;
				break;
			case 2:
				r2 = Gain2;
				break;
			default:
				if(*verbose){
					Serial.println("Register Not Available");
				}
				_go = false;
				break;
		}
	}
	if(_go){
		comB.merge();
		comB.rolecall(*verbose);
		*sdof = false;
		hold8(comB.out,all);
		while(!*sdof){}
		readReg(r2);
		rolecall(r2);
		comB.reset();
		String calType = gainReg1.calTypes[1];
		switch(r2){
			case Gain1:
				gainReg1.calibrated = true;
				gainReg1.calType = calType;
				break;
			case Gain2:
				gainReg2.calibrated = true;
				gainReg2.calType = calType;
				break;
			default:
				if(*verbose){
					Serial.println("Register Not Available");
				}
				break;
		}
		switch(r){
			case Setup1:
				setup1.Gcal = true;
				setup1.GcalType = calType;
				break;
			case Setup2:
				setup2.Gcal = true;
				setup2.GcalType = calType;
				break;
			case Setup3:
				setup3.Gcal = true;
				setup3.GcalType = calType;
				break;
			case Setup4:
				setup4.Gcal = true;
				setup4.GcalType = calType;
				break;
			case Setup5:
				setup5.Gcal = true;
				setup5.GcalType = calType;
				break;
			case Setup6:
				setup6.Gcal = true;
				setup6.GcalType = calType;
				break;
			case Setup7:
				setup7.Gcal = true;
				setup7.GcalType = calType;
				break;
			case Setup8:
				setup8.Gcal = true;
				setup8.GcalType = calType;
				break;
			default:
				if(*verbose){
					Serial.println("Invalid Entry - Must Input Setup");
				}
				break;
		}
	}
}

void CS5532::sysOffCal(reg r, bool all){
	bool _go = true;
	reg r2 = SetupN;
	startSPI();
	if(*verbose){
		Serial.print("\n\t\t-------- ");Serial.print(name);Serial.println(" System Offset Cal --------");
	}
	comB.reset();
	comB.CC = CC_SYS_OC;
	if(!calPrep(r)){
		if(*verbose){
			Serial.println("System-Offset Calibration Failed!");
		}
		_go = false;
	}
	if(_go){
		switch(srcCal){
			case 1:
				r2 = Offset1;
				break;
			case 2:
				r2 = Offset2;
				break;
			default:
				if(*verbose){
					Serial.println("Register Not Available");
				}
				_go = false;
				break;
		}
	}
	if(_go){
		comB.merge();
		comB.rolecall(*verbose);
		*sdof = false;
		hold8(comB.out,all);
		while(!*sdof){}
		readReg(r2);
		rolecall(r2);
		comB.reset();
		String calType = offsetReg1.calTypes[4];
		switch(r2){
			case Offset1:
				offsetReg1.calibrated = true;
				offsetReg1.calType = calType;
				break;
			case Offset2:
				offsetReg2.calibrated = true;
				offsetReg2.calType = calType;
				break;
			default:
				if(*verbose){
					Serial.println("Register Not Available");
				}
				break;
		}
		switch(r){
			case Setup1:
				setup1.Ocal = true;
				setup1.OcalType = calType;
				break;
			case Setup2:
				setup2.Ocal = true;
				setup2.OcalType = calType;
				break;
			case Setup3:
				setup3.Ocal = true;
				setup3.OcalType = calType;
				break;
			case Setup4:
				setup4.Ocal = true;
				setup4.OcalType = calType;
				break;
			case Setup5:
				setup5.Ocal = true;
				setup5.OcalType = calType;
				break;
			case Setup6:
				setup6.Ocal = true;
				setup6.OcalType = calType;
				break;
			case Setup7:
				setup7.Ocal = true;
				setup7.OcalType = calType;
				break;
			case Setup8:
				setup8.Ocal = true;
				setup8.OcalType = calType;
				break;
			default:
				if(*verbose){
					Serial.println("Invalid Entry - Must Input Setup");
				}
				break;
		}
	}
}

void CS5532::sysGainCal(reg r, bool all){
	bool _go = true;
	reg r2 = SetupN;
	startSPI();
	if(*verbose){
		Serial.print("\n\t\t-------- ");Serial.print(name);Serial.println(" System Gain Cal --------");
	}
	comB.reset();
	comB.CC = CC_SYS_GC;
	if(!calPrep(r)){
		if(*verbose){
			Serial.println("System-Gain Calibration Failed!");
		}
		_go = false;
	}
	if(_go){
		switch(srcCal){
			case 1:
				r2 = Gain1;
				break;
			case 2:
				r2 = Gain2;
				break;
			default:
				if(*verbose){
					Serial.println("Register Not Available");
				}
				_go = false;
				break;
		}
	}
	if(_go){
		comB.merge();
		comB.rolecall(*verbose);
		*sdof = false;
		hold8(comB.out,all);
		while(!*sdof){}
		readReg(r2);
		rolecall(r2);
		comB.reset();
		String calType = gainReg1.calTypes[3];
		switch(r2){
			case Gain1:
				gainReg1.calibrated = true;
				gainReg1.calType = calType;
				break;
			case Gain2:
				gainReg2.calibrated = true;
				gainReg2.calType = calType;
				break;
			default:
				if(*verbose){
					Serial.println("Register Not Available");
				}
				break;
		}
		switch(r){
			case Setup1:
				setup1.Gcal = true;
				setup1.GcalType = calType;
				break;
			case Setup2:
				setup2.Gcal = true;
				setup2.GcalType = calType;
				break;
			case Setup3:
				setup3.Gcal = true;
				setup3.GcalType = calType;
				break;
			case Setup4:
				setup4.Gcal = true;
				setup4.GcalType = calType;
				break;
			case Setup5:
				setup5.Gcal = true;
				setup5.GcalType = calType;
				break;
			case Setup6:
				setup6.Gcal = true;
				setup6.GcalType = calType;
				break;
			case Setup7:
				setup7.Gcal = true;
				setup7.GcalType = calType;
				break;
			case Setup8:
				setup8.Gcal = true;
				setup8.GcalType = calType;
				break;
			default:
				if(*verbose){
					Serial.println("Invalid Entry - Must Input Setup");
				}
				break;
		}
	}
}

void CS5532::uploadOffset(reg r, uint32_t o, bool all){
	bool _go = true;
	if(*verbose){
		Serial.print("\n\t\t-------- ");Serial.print(name);Serial.println(" Upload Offset Cal --------");
	}
	switch(r){
		case Offset1:
			offsetReg1.out = o;
			offsetReg1.parse();
			offsetReg1.calibrated = true;
			offsetReg1.calType = offsetReg1.calTypes[9];
			break;
		case Offset2:
			offsetReg2.out = o;
			offsetReg2.parse();
			offsetReg2.calibrated = true;
			offsetReg2.calType = offsetReg1.calTypes[9];
			break;
		default:
			if(*verbose){
				Serial.println("Invalid Entry - Must Input Offset Register");
			}
			_go = false;
			break;
	}
	if(_go){
		writeReg(r,all);
		readReg(r);
		rolecall(r);
	}
	else{
		if(*verbose){
			Serial.println("Offset Upload Failed");
		}
	}
}

void CS5532::uploadGain(reg r, uint32_t g, bool all){
	if(*verbose){
		Serial.print("\n\t\t-------- ");Serial.print(name);Serial.println(" Upload Gain Cal --------");
	}
	bool _go = true;
	switch(r){
		case Gain1:
			gainReg1.out = g;
			gainReg1.parse();
			gainReg1.calibrated = true;
			gainReg1.calType = gainReg1.calTypes[9];
			break;
		case Gain2:
			gainReg2.out = g;
			gainReg2.parse();
			gainReg2.calibrated = true;
			gainReg2.calType = gainReg1.calTypes[9];
			break;
		default:
			if(*verbose){
				Serial.println("Invalid Entry - Must Input Gain Register");
			}
			_go = false;
			break;
	}
	if(_go){
		writeReg(r,all);
		readReg(r);
		rolecall(r);
	}
	else{
		if(*verbose){
			Serial.println("Gain Upload Failed");
		}
	}
}

void CS5532::singMeas(reg r, bool all){
	if(*verbose){
		Serial.print("\n\t\t-------- ");Serial.print(name);Serial.println(" Single Measurement --------");
	}
	comB.reset();
	comB.MC = MC_SINGLE;
	comB.CC = CC_NORM_CV;
	convReg.cont = false;
	convReg.setup = r;
	bool _go = true;
	switch(r){
		case Setup1:
			comB.CSRP = SETUP1;
			break;
		case Setup2:
			comB.CSRP = SETUP2;
			break;
		case Setup3:
			comB.CSRP = SETUP3;
			break;
		case Setup4:
			comB.CSRP = SETUP4;
			break;
		case Setup5:
			comB.CSRP = SETUP5;
			break;
		case Setup6:
			comB.CSRP = SETUP6;
			break;
		case Setup7:
			comB.CSRP = SETUP7;
			break;
		case Setup8:
			comB.CSRP = SETUP8;
			break;
		default:
			_go = false;
			if(*verbose){
				Serial.println("Invalid Entry - Must Input Setup");
			}
			break;
	}
	if(_go){
		comB.merge();
		comB.rolecall(*verbose);
		updateConv();
		hold8(comB.out,all);
		scanMeas();
		conv2Volt();
		comB.reset();
	}
	convReg.reset();
}

void CS5532::contMeas(reg r, bool all){
	startSPI();
	if(*verbose){
		Serial.print("\n\t\t-------- ");Serial.print(name);Serial.println(" Continuous Measurement --------");
	}
	comB.reset();
	comB.MC = MC_CONT;
	comB.CC = CC_NORM_CV;
	convReg.cont = true;
	convReg.setup = r;
	bool _go = true;
	switch(r){
		case Setup1:
			comB.CSRP = SETUP1;
			break;
		case Setup2:
			comB.CSRP = SETUP2;
			break;
		case Setup3:
			comB.CSRP = SETUP3;
			break;
		case Setup4:
			comB.CSRP = SETUP4;
			break;
		case Setup5:
			comB.CSRP = SETUP5;
			break;
		case Setup6:
			comB.CSRP = SETUP6;
			break;
		case Setup7:
			comB.CSRP = SETUP7;
			break;
		case Setup8:
			comB.CSRP = SETUP8;
			break;
		default:
			_go = false;
			if(*verbose){
				Serial.println("Invalid Entry - Must Input Setup");
			}
			break;
	}
	if(_go){
		comB.merge();
		comB.rolecall(*verbose);
		startSPI();
		updateConv();
		if(convReg.configured && convReg.ready){
			hold8(comB.out,all);
			convReg.active = true;
		}
		comB.reset();
	}
	stopSPI();
}

void CS5532::stopMeas(bool all){
	startSPI();
	//float t1 = millis();
	//Serial.println("Stopping Measurement");
	mux->on(*esdo);
	mux->on(*cs);
	//Serial.print("Here:"+name);
	while(!*sdof){}
	//Serial.println(", And here");
	//float t2 = millis();
	//Serial.print("Measurement Stopped\t");
	//Serial.print((t2-t1)/1000.0,2);Serial.println(" s");
	hold8(SYNC1,all);
	command32(0,all);
	convReg.active = false;
	//convReg.reset();
	stopSPI();
	mux->off(*cs);
	mux->off(*esdo);
}

void CS5532::scan(){
	startSPI();
	mux->on(*esdo);
	mux->on(*cs);
	
	inA = SPI.transfer16(0x0000);
	inB = SPI.transfer16(0x0000);
	stopSPI();
	
	mux->off(*cs);
	mux->off(*esdo);
	
	in = (inA << 16) | inB;
}

uint32_t CS5532::scanMeas(){
	mux->on(*esdo);
	mux->on(*cs);
	startSPI();
	while(!*sdof){}
	timestamp = micros();
	
	//mux->on(*esdo);
	//mux->on(*cs);
	SPI.transfer(0x00);
	inA = SPI.transfer16(0x0000);
	inB = SPI.transfer16(0x0000);
	stopSPI();
	mux->off(*cs);
	mux->off(*esdo);
	in = (inA << 16) | inB;
	
	return in;
}

int CS5532::conv2Dec(){
	CV = in >> 8;
	CVD = (CV & 0x007FFFFF) - (CV & 0x00800000);
	return CVD;
}

double CS5532::conv2Volt(){
	conv2Dec();
	CVV = CVD*LSB;
	return CVV;
}

double CS5532::conv2F(){
	conv2Dec();
	CVF = CVD*m + b;
	return CVF;
}

void CS5532::command8(uint8_t val, bool all){
	byte _c = *cs;
	if(all){
		_c = *csAll;
	}
	mux->on(_c);
	//startSPI();
	SPI.transfer(val);
	//stopSPI();
	mux->off(_c);
}

void CS5532::hold8(uint8_t val, bool all){
	byte _c = *cs;
	if(all){
		_c = *csAll;
	}
	mux->on(_c);
	//startSPI();
	SPI.transfer(val);
	//stopSPI();
}

void CS5532::command16(uint16_t val, bool all){
	byte _c = *cs;
	if(all){
		_c = *csAll;
	}
	mux->on(_c);
	//startSPI();
	SPI.transfer16(val);
	//stopSPI();
	mux->off(_c);
}

void CS5532::command32(bool all){
	byte _c = *cs;
	if(all){
		_c = *csAll;
	}
	mux->on(_c);
	//startSPI();
	SPI.transfer16(outA);
	SPI.transfer16(outB);
	//stopSPI();
	mux->off(_c);
}

void CS5532::command32(uint32_t val, bool all){
	byte _c = *cs;
	if(all){
		_c = *csAll;
	}
	outA = val >> 16;
	outB = val & 0xFFFF;
	mux->on(_c);
	//startSPI();
	SPI.transfer16(outA);
	SPI.transfer16(outB);
	//stopSPI();
	mux->off(_c);
}

void CS5532::print8(uint8_t _b){
	for(int i=0;i<8;++i){
		if((i>0)&&((i%4)==0)){
			Serial.print(" | ");
		}
		Serial.print(1&(_b>>(7-i)));
	}
	
}

void CS5532::print16(uint16_t _b){
	for(int i=0;i<16;++i){
		if((i>0)&&((i%4)==0)){
			Serial.print(" | ");
		}
		Serial.print(1&(_b>>(15-i)));
	}
}

void CS5532::print32(uint32_t _b){
	for(int i=0;i<32;++i){
		if((i>0)&&((i%4)==0)){
			Serial.print(" | ");
		}
		Serial.print(1&(_b>>(31-i)));
	}
}