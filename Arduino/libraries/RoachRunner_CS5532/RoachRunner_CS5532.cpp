#include "RoachRunner_CS5532.h"
//#include "RoachRunner_Digipot.h"
#include "RoachRunner_Math.h"
#include <Arduino.h>
#include <SPI.h>

ComA::ComA(String _a){
	name = _a;
}

void ComA::merge(){
	out = ARA | CS | RW | RSB;
}

void ComA::parse(){
	read8(out);
	ARA = 0x7F & (d8[1]<<6);
	CS = 0x7F & ((d8[2]<<5) | (d8[3]<<4));
	RW = 0x7F & (d8[4]<<3);
	RSB = 0x7F & ((d8[5]<<2) | (d8[6]<<1) | d8[7]);
}

void ComA::reset(){
	out = 0;
	parse();
}

void ComA::read8(uint8_t _a){
	for(int i=0;i<8;++i){
		d8[i] = 0;
	}
	for(int i=0;i<8;++i){
		if(_a>=pow(2,7-i)){
			d8[i] = 1;
			_a = _a - pow(2,7-i);
		}
	}
}

void ComA::print8(uint8_t _a){
	Serial.print(_a);
	for(int i=0;i<8;++i){
		d8[i] = 0;
	}
	for(int i=0;i<8;++i){
		if(i==4){
			Serial.print(" | ");
		}
		if(_a>=pow(2,7-i)){
			d8[i] = 1;
			_a = _a - pow(2,7-i);
		}
		Serial.print(d8[i]);
	}
}

void ComA::rolecall(){
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

ComB::ComB(String _a){
	name = _a;
}

void ComB::merge(){
	out = 0x80 | MC | CSRP | CC;
}

void ComB::parse(){
	read8(out);
	MC = 0x80 | d8[1]<<6;
	CSRP = 0x80 | (d8[2]<<5) | (d8[3]<<4) | (d8[4]<<3);
	CC = 0x80 | (d8[5]<<2) | (d8[6]<<1) | d8[7];
}

void ComB::reset(){
	out = 0;
	parse();
}

void ComB::read8(uint8_t _a){
	for(int i=0;i<8;++i){
		d8[i] = 0;
	}
	for(int i=0;i<8;++i){
		if(_a>=pow(2,7-i)){
			d8[i] = 1;
			_a = _a - pow(2,7-i);
		}
	}
}

void ComB::print8(uint8_t _a){
	for(int i=0;i<8;++i){
		d8[i] = 0;
	}
	for(int i=0;i<8;++i){
		if(i==4){
			Serial.print(" | ");
		}
		if(_a>=pow(2,7-i)){
			d8[i] = 1;
			_a = _a - pow(2,7-i);
		}
	Serial.print(d8[i]);
	}
}

void ComB::rolecall(){
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

ConfReg::ConfReg(String _a){
	name = _a;
}

void ConfReg::merge(){
	out = PSS | PDW | RS | IS | GB | VRS | A1 | A0 | OLS | OGS | FRS;
}

void ConfReg::parse(){
	read32(out);
	PSS = d32[0]<<31;
	PDW = d32[1]<<30;
	RS = d32[2]<<29;
	RV = d32[3]<<28;
	IS = d32[4]<<27;
	GB = d32[5]<<26;
	VRS = d32[6]<<25;
	A1 = d32[7]<<24;
	A0 = d32[8]<<23;
	OLS = d32[9]<<22;
	OGS = d32[11]<<20;
	FRS = d32[12]<<19;
}

void ConfReg::reset(){
	out = 0;
	parse();
}

void ConfReg::read32(uint32_t _a){
	for(int i=0;i<32;++i){
		d32[i] = 0;
	}
	for(int i=0;i<32;++i){
		if(_a>=pow(2,31-i)){
			d32[i] = 1;
			_a = _a - pow(2,31-i);
		}
	}
}

void ConfReg::print32(uint32_t _a){
	int k = 0;
	for(int i=0;i<32;++i){
		d32[i] = 0;
	}
	for(int i=0;i<32;++i){
		++k;
		if(k>4){
			Serial.print(" | ");
			k=1;
		}
		if(_a>=pow(2,31-i)){
			d32[i] = 1;
			_a = _a - pow(2,31-i);
		}
		Serial.print(d32[i]);
	}
}

void ConfReg::rolecall(){
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
	if(VRS){
		Serial.print("1 - (LOW: A = 1) 1V <= Vref <= 2.5V");
		A = 1;
	}
	else{
		Serial.print("0 - (HIGH: A = 2) 2.5V < Vref <= [(VA+)-(VA-)]");
		A = 2;
	}
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
	if(OGS){
		Serial.print("1 - Calibration Registers Based on OG1-OG0 from Referenced Setup");
	}
	else{
		Serial.print("0 - Calibration Registers Based on CS1-CS0 from Referenced Setup");
	}
	Serial.print("\nFRS:\t");
	if(FRS){
		Serial.println("1 - Scale Output Word Rates and Filter Characteristics by 5/6");
	}
	else{
		Serial.println("0 - Use Default Output Word Rates");
	}
}

Setup::Setup(String _a){
	name = _a;
}

void Setup::merge(){
	out = CS | G | WR | UB | OL | DT | OCD | OG;
}

void Setup::parse(){
	read16(out);
	CS = (d16[0]<<15) | (d16[1]<<14);
	G  = (d16[2]<<13) | (d16[3]<<12) | (d16[4]<<11);
	WR = (d16[5]<<10) | (d16[6]<<9) | (d16[7]<<8) | (d16[8]<<7);
	UB = d16[9]<<6;
	OL = (d16[10]<<5) | (d16[11]<<4);
	DT = d16[12]<<3;
	OCD = d16[13]<<2;
	OG = (d16[14]<<1) | d16[15];
}

void Setup::reset(){
	out = 0;
	parse();
}

void Setup::read16(uint16_t _a){
	for(int i=0;i<16;++i){
		d16[i] = 0;
	}
	for(int i=0;i<16;++i){
		if(_a>=pow(2,15-i)){
			d16[i] = 1;
			_a = _a - pow(2,15-i);
		}
	}
}

void Setup::print16(uint16_t _a){
	int k = 0;
	for(int i=0;i<16;++i){
		d16[i] = 0;
	}
	for(int i=0;i<16;++i){
		++k;
		if(k>4){
			Serial.print(" | ");
			k=1;
		}
		if(_a>=pow(2,15-i)){
			d16[i] = 1;
			_a = _a - pow(2,15-i);
		}
		Serial.print(d16[i]);
	}
}

void Setup::rolecall(){
	Serial.print("\n------ ");Serial.print(name);Serial.println(" ------");
	Serial.print("OUT:\t");
	print16(out);
	parse();
	Serial.print("\nCS:\t");
	switch(CS){
		case C1:
			ch = 1;
			Serial.print("00 - Physical Channel 1");
			break;
		case C2:
			ch = 2;
			Serial.print("01 - Physical Channel 2");
			break;
		case C3:
			ch = 3;
			Serial.print("10 - Physical Channel 3");
			break;
		case C4:
			ch = 4;
			Serial.print("11 - Physical Channel 4");
			break;
	}
	Serial.print("\nG:\t");
	switch(G){
		case G1:
			g = 1.0;
			Serial.print("000 - Gain = 1");
			break;
		case G2:
			g = 2.0;
			Serial.print("001 - Gain = 2");
			break;
		case G4:
			g = 4.0;
			Serial.print("010 - Gain = 4");
			break;
		case G8:
			g = 8.0;
			Serial.print("011 - Gain = 8");
			break;
		case G16:
			g = 16.0;
			Serial.print("100 - Gain = 16");
			break;
		case G32:
			g = 32.0;
			Serial.print("101 - Gain = 32");
			break;
		case G64:
			g = 64.0;
			Serial.print("110 - Gain = 64");
			break;
		default:
			Serial.print("Invalid Entry");
			break;
	}
	Serial.print("\nWR:\t");
	switch(WR){
		case WR120:
			if(FRS){
				wr = 100.0;
			}
			else{
				wr = 120.0;
			}
			Serial.print("0000 - WR = ");
			Serial.print(wr);
			Serial.print(" Sps");
			break;
		case WR60:
			if(FRS){
				wr = 50.0;
			}
			else{
				wr = 60.0;
			}
			Serial.print("0001 - WR = ");
			Serial.print(wr);
			Serial.print(" Sps");
			break;
		case WR30:
			if(FRS){
				wr = 25.0;
			}
			else{
				wr = 30.0;
			}
			Serial.print("0010 - WR = ");
			Serial.print(wr);
			Serial.print(" Sps");
			break;
		case WR15:
			if(FRS){
				wr = 12.5;
			}
			else{
				wr = 15.0;
			}
			Serial.print("0011 - WR = ");
			Serial.print(wr);
			Serial.print(" Sps");
			break;
		case WR7_5:
			if(FRS){
				wr = 6.25;
			}
			else{
				wr = 7.5;
			}
			Serial.print("0100 - WR = ");
			Serial.print(wr);
			Serial.print(" Sps");
			break;
		case WR3840:
			if(FRS){
				wr = 3200.0;
			}
			else{
				wr = 3840.0;
			}
			Serial.print("1000 - WR = ");
			Serial.print(wr);
			Serial.print(" Sps");
			break;
		case WR1920:
			if(FRS){
				wr = 1600.0;
			}
			else{
				wr = 1920.0;
			}
			Serial.print("1001 - WR = ");
			Serial.print(wr);
			Serial.print(" Sps");
			break;
		case WR960:
			if(FRS){
				wr = 800.0;
			}
			else{
				wr = 960.0;
			}
			Serial.print("1010 - WR = ");
			Serial.print(wr);
			Serial.print(" Sps");
			break;
		case WR480:
			if(FRS){
				wr = 400.0;
			}
			else{
				wr = 480.0;
			}
			Serial.print("1011 - WR = ");
			Serial.print(wr);
			Serial.print(" Sps");
			break;
		case WR240:
			if(FRS){
				wr = 200.0;
			}
			else{
				wr = 240.0;
			}
			Serial.print("1100 - WR = ");
			Serial.print(wr);
			Serial.print(" Sps");
			break;
		default:
			Serial.print("Invalid Entry");
			break;
	}
	Serial.print("\nUB:\t");
	if(UB){
		Serial.print("1 - Unipolar Mode");
	}
	else{
		Serial.print("0 - Bipolar Mode");
	}
	Serial.print("\nOL:\t");
	switch(OL){
		case OLB_0:
			Serial.print("00 - A0 = 0, A1 = 0");
			break;
		case OLB_1:
			Serial.print("01 - A0 = 0, A1 = 1");
			break;
		case OLB_2:
			Serial.print("10 - A0 = 1, A1 = 0");
			break;
		case OLB_3:
			Serial.print("11 - A0 = 1, A1 = 1");
			break;
	}
	Serial.print("\nDT:\t");
	if(DT){
		Serial.print("1 - Wait ");
		if(FRS){
			Serial.print("1536");
		}
		else{
			Serial.print("1280");
		}
		Serial.print(" MCLK Tiks Before Starting Conversion");
	}
	else{
		Serial.print("0 - Begin Conversions Immediately");
	}
	Serial.print("\nOCD:\t");
	if(OCD){
		Serial.print("1 - Activate Current Source (Open Circuit Detect)");
	}
	else{
		Serial.print("0 - Normal Mode");
	}
	Serial.print("\nOG:\t");
	switch(OG){
		case OG_1:
			og = 1;
			Serial.print("00 - Use Offset/Gain From Channel 1");
			break;
		case OG_2:
			og = 2;
			Serial.print("01 - Use Offset/Gain From Channel 2");
			break;
		case OG_3:
			og = 3;
			Serial.print("10 - Use Offset/Gain From Channel 3");
			break;
		case OG_4:
			og = 4;
			Serial.print("11 - Use Offset/Gain From Channel 4");
			break;
	}
	if(OGS){
		Serial.println(" (In Use: OGS = 1)");
		src = og;
	}
	else{
		Serial.println(" (Not In Use: OGS = 0)");
		src = ch;
	}
}

GainReg::GainReg(String _a){
	name = _a;
}

void GainReg::merge(){
	out = g5p | g4p | g3p | g2p | g1p | g0 | g1 | g2 | g3 | g4 | g5 | g6 | g7 | g8 | g9 | g10 | g11 | g12 | g13 | g14 | g15 | g16 | g18 | g19 | g20 | g21 | g22 | g23 | g24;
}

void GainReg::parse(){
	read32(out);
	int i = 29;
	g5p = d32[i--];
	g4p = d32[i--];
	g3p = d32[i--];
	g2p = d32[i--];
	g1p = d32[i--];
	g0 = d32[i--];
	g1 = d32[i--];
	g2 = d32[i--];
	g3 = d32[i--];
	g4 = d32[i--];
	g5 = d32[i--];
	g6 = d32[i--];
	g7 = d32[i--];
	g8 = d32[i--];
	g9 = d32[i--];
	g10 = d32[i--];
	g11 = d32[i--];
	g12 = d32[i--];
	g13 = d32[i--];
	g14 = d32[i--];
	g15 = d32[i--];
	g16 = d32[i--];
	g17 = d32[i--];
	g18 = d32[i--];
	g19 = d32[i--];
	g20 = d32[i--];
	g21 = d32[i--];
	g22 = d32[i--];
	g23 = d32[i--];
	g24 = d32[i--];
}

void GainReg::reset(){
	out = 0;
	parse();
}

void GainReg::read32(uint32_t _a){
	for(int i=0;i<32;++i){
		d32[i] = (_a>>i)&1;
	}
}

void GainReg::print32(uint32_t _a){
	int k = 0;
	for(int i=0;i<32;++i){
		d32[i] = 0;
	}
	for(int i=0;i<32;++i){
		++k;
		if(k>4){
			Serial.print(" | ");
			k=1;
		}
		if(_a>=pow(2,31-i)){
			d32[i] = 1;
			_a = _a - pow(2,31-i);
		}
		Serial.print(d32[i]);
	}
}

void GainReg::rolecall(){
	Serial.print("\n------ ");Serial.print(name);Serial.println(" ------");
	Serial.print("OUT:\t");
	print32(out);
	Serial.print("\t( ");
	Serial.print(out);
	Serial.print(" )");
	parse();
	convert();
	Serial.print("\nGain:\t");
	Serial.print(gain, 6);
	Serial.print("\t[");
	sciNote(gain, 6);
	Serial.println("]");
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

void GainReg::convert(){
	read32(out);
	//Serial.println("\nGain Conversion");
	//print32(out);
	//Serial.print("\n");
	gain = 0.0;
	for(int i=2;i<32;++i){
		gain = gain + float(d32[i])*pow(2,7-i);
	}
	//Serial.println(gain,10);
}

OffsetReg::OffsetReg(String _a){
	name = _a;
}

void OffsetReg::merge(){
	out = o1 | o2 | o3 | o4 | o5 | o6 | o7 | o8 | o9 | o10 | o11 | o12 | o13 | o14 | o15 | o16 | o17 | o18 | o19 | o20 | o21 | o22 | o23 | o24;
}

void OffsetReg::parse(){
	read32(out);
	int i = 31;
	o1 = d32[i--];
	o2 = d32[i--];
	o3 = d32[i--];
	o4 = d32[i--];
	o5 = d32[i--];
	o6 = d32[i--];
	o7 = d32[i--];
	o8 = d32[i--];
	o9 = d32[i--];
	o10 = d32[i--];
	o11 = d32[i--];
	o12 = d32[i--];
	o13 = d32[i--];
	o14 = d32[i--];
	o15 = d32[i--];
	o16 = d32[i--];
	o17 = d32[i--];
	o18 = d32[i--];
	o19 = d32[i--];
	o20 = d32[i--];
	o21 = d32[i--];
	o22 = d32[i--];
	o23 = d32[i--];
	o24 = d32[i--];
}

void OffsetReg::reset(){
	out = 0;
	parse();
}

void OffsetReg::read32(uint32_t _a){
	for(int i=0;i<32;++i){
		d32[i] = (_a>>i)&1;
	}
}

void OffsetReg::print32(uint32_t _a){
	int k = 0;
	for(int i=0;i<32;++i){
		d32[i] = 0;
	}
	for(int i=0;i<32;++i){
		++k;
		if(k>4){
			Serial.print(" | ");
			k=1;
		}
		if(_a>=pow(2,31-i)){
			d32[i] = 1;
			_a = _a - pow(2,31-i);
		}
		Serial.print(d32[i]);
	}
}

void OffsetReg::rolecall(){
	Serial.print("\n------ ");Serial.print(name);Serial.println(" ------");
	Serial.print("OUT:\t");
	print32(out);
	Serial.print("\t( ");
	Serial.print(out);
	Serial.print(" )");
	parse();
	convert();
	Serial.print("\nOffset:\t");
	sciNote(offset, 6);
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

void OffsetReg::convert(){
	read32(out);
	offset = 0.0;
	offset = float(-d32[0])*pow(2,-1);
	for(int i=1;i<24;++i){
		offset = offset + float(d32[i])*pow(2,-(i+1));
	}
}

void ConvReg::rolecall(){
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
	if(bipolar){
		Serial.print("Bipolar:\tkBi = ");
		Serial.println(kBi);
	}
	else{
		Serial.print("Unipolar:\tkBi = ");
		Serial.println(kBi);
	}
	Serial.print("Vref Range:\t\t\t");
	if(vRefLow){
		Serial.println("1V <= Vref <= 2.5V");
	}
	else{
		Serial.println("2.5V < Vref <= [(VA+)-(VA-)]");
	}
	Serial.print("Vref (Approx):\t\t\t");
	Serial.println(Vref);
	Serial.print("VA (Approx):\t\t\t");
	Serial.println(Va);
	Serial.print("\nSampling Gain:\t\t\tG = ");
	Serial.println(G);
	Serial.print("\t\t\t\tA = ");
	Serial.println(A);
	Serial.print("\nFull-Scale Input Range:\t\tVfs = ");
	Serial.print(Vfs,6);
	Serial.print(" V\n\t\t\t\tVspan = ");
	Serial.print(Vspan,6);
	Serial.print(" V\n\t\t\t\tLSB = ");
	//Serial.println(LSB);
	sciNote(LSB,6);
	Serial.print(" V\n\nChannel-Setup Used:\t\t");
	Serial.println(setupName);
	Serial.print("Offset Calibration Method:\t");
	Serial.println(OcalType);
	Serial.print("Offset Calibration Value:\tO = ");
	sciNote(offCal,6);
	Serial.print("\nGain Calibration Method:\t");
	Serial.println(GcalType);
	Serial.print("Gain Calibration Value:\t\tG = ");
	sciNote(gainCal,6);
	Serial.print("\nOutput Word Rate:\t\t");
	Serial.print(wr);
	Serial.println(" Sps");
}

CS5532::CS5532(String _a, int _cs, int _sdo, int _osc){
	name = _a;
	cs = _cs;
	sdoFlag = _sdo;
	osc = _osc;

	timeKeeper = true;

	setup1.name = setups[0];
	setup2.name = setups[1];
	setup3.name = setups[2];
	setup4.name = setups[3];
	setup5.name = setups[4];
	setup6.name = setups[5];
	setup7.name = setups[6];
	setup8.name = setups[7];
	offsetReg1.name = offsetRegs[0];
	offsetReg2.name = offsetRegs[1];
	gainReg1.name = gainRegs[0];
	gainReg2.name = gainRegs[1];
	
	//pinMode(sdoFlag, INPUT_PULLUP);
	//attachInterrupt(digitalPinToInterrupt(sdoFlag), raiseFlag, FALLING);
	if(cs>0 && !pinsSet){
		pinMode(cs, OUTPUT);
		digitalWrite(cs, LOW);
		pinsSet = true;
	}
}

CS5532::CS5532(String _a, int _cs, int _sdo){
	name = _a;
	cs = _cs;
	sdoFlag = _sdo;

	setup1.name = setups[0];
	setup2.name = setups[1];
	setup3.name = setups[2];
	setup4.name = setups[3];
	setup5.name = setups[4];
	setup6.name = setups[5];
	setup7.name = setups[6];
	setup8.name = setups[7];
	offsetReg1.name = offsetRegs[0];
	offsetReg2.name = offsetRegs[1];
	gainReg1.name = gainRegs[0];
	gainReg2.name = gainRegs[1];
}

CS5532::CS5532(){

	setup1.name = setups[0];
	setup2.name = setups[1];
	setup3.name = setups[2];
	setup4.name = setups[3];
	setup5.name = setups[4];
	setup6.name = setups[5];
	setup7.name = setups[6];
	setup8.name = setups[7];
	offsetReg1.name = offsetRegs[0];
	offsetReg2.name = offsetRegs[1];
	gainReg1.name = gainRegs[0];
	gainReg2.name = gainRegs[1];

}

void CS5532::init(){

	Serial.print("\n-----------------------------------------------------\nADC:\t");
	Serial.println(name);

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
	
	if(!SPIStarted){
		startSPI();
		//pinMode(sdoFlag, INPUT_PULLUP);
		//attachInterrupt(digitalPinToInterrupt(sdoFlag), raiseFlag, FALLING);
		Serial.print("\nSPI Started\n");
	}
	//pinMode(sdoFlag, INPUT);
	//if(cs>0 && !pinsSet){
	//	pinMode(cs, OUTPUT);
	//	digitalWrite(cs, LOW);
	//	pinsSet = true;
	//}

	Serial.print("\n\t\t-------- ");Serial.print(name);Serial.println(" Clock Check --------");
	Serial.print("\nClock Active?\t");
	Serial.println(clockStarted);
	Serial.print("Is this ADC the Timekeeper?\t");
	Serial.println(timeKeeper);

	if(!pinsSet){
		Serial.println("\n WARNING: UNDECLARED PINS ");
	}
	else{
		if(timeKeeper && !clockStarted){
			startClock();
			Serial.print("Clock Active?\t");
			Serial.println(clockStarted);
		}
		int k = 0;
		while(!ADCReset){
			++k;
			SPIS();
			SRS();
			if(!ADCReset){
				Serial.println("\nADC Reset Failed... Trying Again");
			}
			if(k>10){
				Serial.print("Could not initialize ADC... Please check wiring");
				while(1){}
			}
		}
		VRSinit();
		CSRinit();
		
	}
	flightCheck();
}

void CS5532::flightCheck(){
	Serial.print("\n-------- ");Serial.print(name);Serial.println(" Flight Check --------");
	Serial.print("Pins Declared?\t\t\t");Serial.println(pinsSet);
	Serial.print("Clock Started?\t\t\t");Serial.println(clockStarted);
	Serial.print("Arduino SPI ready?\t\t");Serial.println(SPIStarted);
	Serial.print("ADC SPI Initialized?\t\t");Serial.println(ADCSerialInit);
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
	if(pinsSet && clockStarted && SPIStarted && ADCSerialInit && ADCReset && VRSset && OGSset && CSRset){
		measReady = true;
	}
	else{
		measReady = false;
	}
	convReg.configured = measReady;
	Serial.print("Ready to Measure?\t\t");Serial.println(measReady);
	Serial.print("Array Fully Prepped?\t\t");Serial.println(allClear);
	Serial.println("------------------------");
}

void CS5532::startClock(){
	Serial.print("\tStarting Clock...\n\t");
	int m = 84/Fclock;
	int m2 = m/2;
	int32_t mask_PWM_pin = digitalPinToBitMask(osc);
	MCLK = float(84)/float(m);
	REG_PMC_PCER1 = 1<<4; // activate clock for PWM controller
	REG_PIOC_PDR |= mask_PWM_pin; // activate peripheral functions for pin (disables all PIO functionality)
	REG_PIOC_ABSR |= mask_PWM_pin; // choose peripheral option B
	REG_PWM_CLK = 0; // choose clock rate, 0 → full MCLK as reference 84MHz
	REG_PWM_CMR6 = 0<<9; // select clock and polarity for PWM channel (pin7) → (CPOL = 0)
	REG_PWM_CPRD6 = m; // initialize PWM period → T = value/84MHz (value: up to 16bit), value=8 → 10.5MHz
	REG_PWM_CDTY6 = m2; // initialize duty cycle, REG_PWM_CPRD6 / value = duty cycle, for 8/4 = 50%
	REG_PWM_ENA = 1<<6; // enable PWM on PWM channel (pin 7 = PWML6)
	Serial.print(MCLK,3);
	Serial.println(" MHz MCLK Activated");
	clockStarted = true;
}

void CS5532::startSPI(){
	Serial.print("\n\t\t-------- ");Serial.print(name);Serial.println(" Start SPI --------");
	SPI.begin();
	SPI.beginTransaction(SPISettings(fosc,MSBFIRST,SPI_MODE0));
	SPIStarted = true;
}

void CS5532::SPIS(){
	Serial.print("\n\t\t-------- ");Serial.print(name);Serial.println(" SPIS --------");
	for(int i=0;i<16;++i){
		command8(SYNC1);
	}
	command8(SYNC0);
	ADCSerialInit = true;
	Serial.print("\nSYNC Commands Sent: ADC ");
	Serial.print(name);
	Serial.println("'s Serial Port Initialized");
}

void CS5532::SRS(){
	Serial.print("\n\t\t-------- ");Serial.print(name);Serial.println(" SRS --------");
	confReg.RS = RS_RESET;
	writeReg(Config);
	
	delayMicroseconds(20);
	
	confReg.RS = RS_NORM;
	writeReg(Config);
	
	readReg(Config);
	
	confReg.rolecall();
	if(confReg.RV){
		ADCReset = true;
	}
}

void CS5532::VRSinit(){
	Serial.print("\n\t\t-------- ");Serial.print(name);Serial.println(" Set VRS --------");
	confReg.VRS = VRS_HIGH;
	confReg.OGS = OGS_OG;
	writeReg(Config);
	readReg(Config);
	confReg.rolecall();
	if(confReg.VRS==VRS_HIGH){
		VRSset = true;
	}
	if(confReg.OGS==OGS_OG){
		OGSset = true;
	}
}

void CS5532::CSRinit(){
	Serial.print("\n\t\t-------- ");Serial.print(name);Serial.println(" Set CSRs --------");
	setup1.CS = C1;
	setup2.CS = C1;
	setup3.CS = C1;
	setup4.CS = C1;
	//setup5.CS = C1;
	//setup6.CS = C1;
	//setup7.CS = C1;
	//setup8.CS = C1;
	
	setup1.G = G32;
	setup2.G = G32;
	setup3.G = G32;
	setup4.G = G32;
	//setup5.G = G1;
	//setup6.G = G32;
	//setup7.G = G32;
	//setup8.G = G32;
	
	setup1.WR = WR7_5;
	setup2.WR = WR120;
	setup3.WR = WR240;
	setup4.WR = WR3840;
	//setup5.WR = WR7_5;
	//setup6.WR = WR7_5;
	//setup7.WR = WR120;
	//setup8.WR = WR120;
	
	setup1.UB = UB_BI;
	setup2.UB = UB_BI;
	setup3.UB = UB_BI;
	setup4.UB = UB_BI;
	//setup5.UB = UB_BI;
	//setup6.UB = UB_BI;
	//setup7.UB = UB_BI;
	//setup8.UB = UB_BI;
	
	setup1.OL = OLB_0;
	setup2.OL = OLB_0;
	setup3.OL = OLB_0;
	setup4.OL = OLB_0;
	//setup5.OL = OLB_0;
	//setup6.OL = OLB_0;
	//setup7.OL = OLB_0;
	//setup8.OL = OLB_0;
	
	setup1.DT = DT_IM;
	setup2.DT = DT_IM;
	setup3.DT = DT_IM;
	setup4.DT = DT_IM;
	//setup5.DT = DT_IM;
	//setup6.DT = DT_IM;
	//setup7.DT = DT_IM;
	//setup8.DT = DT_IM;
	
	setup1.OCD = OCD_NORM;
	setup2.OCD = OCD_NORM;
	setup3.OCD = OCD_NORM;
	setup4.OCD = OCD_NORM;
	//setup5.OCD = OCD_NORM;
	//setup6.OCD = OCD_NORM;
	//setup7.OCD = OCD_NORM;
	//setup8.OCD = OCD_NORM;
	
	setup1.OG = OG_1;
	setup2.OG = OG_1;
	setup3.OG = OG_1;
	setup4.OG = OG_1;
	//setup5.OG = OG_1;
	//setup6.OG = OG_1;
	//setup7.OG = OG_1;
	//setup8.OG = OG_2;
	
	setup1.merge();
	setup2.merge();
	setup3.merge();
	setup4.merge();
	
	writeReg(Setup1);
	writeReg(Setup2);
	writeReg(Setup3);
	writeReg(Setup4);
	//writeReg(Setup5);
	//writeReg(Setup6);
	//writeReg(Setup7);
	//writeReg(Setup8);
	
	readReg(Setup1);
	readReg(Setup2);
	readReg(Setup3);
	readReg(Setup4);
	//readReg(Setup5);
	//readReg(Setup6);
	//readReg(Setup7);
	//readReg(Setup8);
	
	setup1.rolecall();
	setup2.rolecall();
	setup3.rolecall();
	setup4.rolecall();
	//setup5.rolecall();
	//setup6.rolecall();
	//setup7.rolecall();
	//setup8.rolecall();
	
	CSRset = true;
}

void CS5532::calPrep(reg r){
	switch(r){
		case Setup1:
			src = setup1.src;
			break;
		case Setup2:
			src = setup2.src;
			break;
		case Setup3:
			src = setup3.src;
			break;
		case Setup4:
			src = setup4.src;
			break;
		case Setup5:
			src = setup5.src;
			break;
		case Setup6:
			src = setup6.src;
			break;
		case Setup7:
			src = setup7.src;
			break;
		case Setup8:
			src = setup8.src;
			break;
		default:
			Serial.println("Invalid Entry");
			break;
	}
}

void CS5532::selfOffCal(reg r){
	Serial.print("\n\t\t-------- ");Serial.print(name);Serial.println(" Self Offset Cal --------");
	comB.reset();
	comB.CC = CC_SELF_OC;
	calPrep(r);
	reg r2 = SetupN;
	bool checkPoint = false;
	switch(src){
		case 1:
			r2 = Offset1;
			checkPoint = true;
			break;
		case 2:
			r2 = Offset2;
			checkPoint = true;
			break;
		default:
			Serial.println("Register Not Available");
			break;
	}
	if(checkPoint){
		comB.merge();
		hold8(comB.out);
		while(digitalRead(sdoFlag)){}
		readReg(r2);
		comB.rolecall();
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
				Serial.println("Invalid Entry");
				break;
		}
	}
}

void CS5532::sysOffCal(reg r, bool _v){			//Use this for G > 1
	if(_v){
		Serial.print("\n\t\t-------- ");Serial.print(name);Serial.println(" System Offset Cal --------");
	}
	comB.reset();
	comB.CC = CC_SYS_OC;
	calPrep(r);
	reg r2 = SetupN;
	bool checkPoint = false;
	switch(src){
		case 1:
			r2 = Offset1;
			checkPoint = true;
			break;
		case 2:
			r2 = Offset2;
			checkPoint = true;
			break;
		default:
			Serial.println("Register Not Available");
			break;
	}
	if(checkPoint){
		comB.merge();
		hold8(comB.out);
		while(digitalRead(sdoFlag)){}
		readReg(r2);
		if(_v){
			comB.rolecall();
			rolecall(r2);
		}
		//else{
			//switch(r2){
				//case Offset1:
					//Serial.print(offsetReg1.out);
				//case Offset2:
					//Serial.print(offsetReg2.out);
				//default:
					//break;
			//}
		//}
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
				Serial.println("Invalid Entry");
				break;
		}
	}
}

void CS5532::selfGainCal(reg r){
	Serial.print("\n\t\t-------- ");Serial.print(name);Serial.println(" Self Gain Cal --------");
	comB.reset();
	comB.CC = CC_SELF_GC;
	calPrep(r);
	reg r2 = SetupN;
	bool checkPoint = false;
	switch(src){
		case 1:
			r2 = Gain1;
			checkPoint = true;
			break;
		case 2:
			r2 = Gain2;
			checkPoint = true;
			break;
		default:
			Serial.println("Register Not Available");
			break;
	}
	if(checkPoint){
		comB.merge();
		hold8(comB.out);
		while(digitalRead(sdoFlag)){}
		readReg(r2);
		comB.rolecall();
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
				Serial.println("Invalid Entry");
				break;
		}
	}
}

void CS5532::sysGainCal(reg r, bool _v){			//Use this for VREF > 2.5
	if(_v){
		Serial.print("\n\t\t-------- ");Serial.print(name);Serial.println(" System Gain Cal --------");
	}
	comB.reset();
	comB.CC = CC_SYS_GC;
	calPrep(r);
	reg r2 = SetupN;
	bool checkPoint = false;
	switch(src){
		case 1:
			r2 = Gain1;
			checkPoint = true;
			break;
		case 2:
			r2 = Gain2;
			checkPoint = true;
			break;
		default:
			Serial.println("Register Not Available");
			break;
	}
	if(checkPoint){
		comB.merge();
		hold8(comB.out);
		while(digitalRead(sdoFlag)){}
		readReg(r2);
		if(_v){
			comB.rolecall();
			rolecall(r2);
		}
		//else{
			//switch(r2){
				//case Gain1:
					//Serial.print(gainReg1.out);
				//case Gain2:
					//Serial.print(gainReg2.out);
				//default:
					//break;
			//}
		//}
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
				Serial.println("Invalid Entry");
				break;
		}
	}
}

void CS5532::uploadOffset(reg r, uint32_t o){
	Serial.print("\n\t\t-------- ");Serial.print(name);Serial.println(" Upload Offset Cal --------");
	switch(r){
		case Offset1:
			offsetReg1.out = o;
			Serial.println(offsetReg1.out);
			offsetReg1.parse();
			break;
		case Offset2:
			offsetReg2.out = o;
			offsetReg2.parse();
			break;
		default:
			Serial.println("Register Not Available");
			break;
	}
	writeReg(r);
	readReg(r);
	rolecall(r);
}

void CS5532::uploadOffset(reg r, float offAve, bool z){
	int offAveD[24];
	for(int i=0;i<24;++i){
		offAveD[i] = 0;
	}
	if(offAve<0){
		offAveD[0] = 1;
	}
	offAve = offAve + offAveD[0]*.5;
	for(int i=1;i<24;++i){
		if(offAve>pow(2,-(i+1))){
			offAveD[i] = 1;
			offAve = offAve - pow(2,-(i+1));
		}
	}
	uint32_t o = 0;
	for(int i=0;i<24;++i){
		o = o + offAveD[i]*pow(2,23-i);
	}
	o = o<<8;
	Serial.print("\n\t\t-------- ");Serial.print(name);Serial.println(" Upload Offset Cal --------");
	switch(r){
		case Offset1:
			offsetReg1.out = o;
			offsetReg1.parse();
			break;
		case Offset2:
			offsetReg2.out = o;
			offsetReg2.parse();
			break;
		default:
			Serial.println("Register Not Available");
			break;
	}
	writeReg(r);
	readReg(r);
	rolecall(r);
}

void CS5532::uploadGain(reg r, uint32_t g){
	Serial.print("\n\t\t-------- ");Serial.print(name);Serial.println(" Upload Gain Cal --------");
	switch(r){
		case Gain1:
			gainReg1.out = g;
			Serial.println(gainReg1.out);
			gainReg1.parse();
			break;
		case Gain2:
			gainReg2.out = g;
			gainReg2.parse();
			break;
		default:
			Serial.println("Register Not Available");
			break;
	}
	writeReg(r);
	readReg(r);
	rolecall(r);
}

void CS5532::singMeas(reg r){
	Serial.print("\n\t\t-------- ");Serial.print(name);Serial.println(" Single Measurement --------");
	comB.reset();
	comB.MC = MC_SINGLE;
	convReg.cont = false;
	convReg.setup = r;
	bool _go = false;
	switch(r){
		case Setup1:
			comB.CSRP = SETUP1;
			_go = true;
			break;
		case Setup2:
			comB.CSRP = SETUP2;
			_go = true;
			break;
		case Setup3:
			comB.CSRP = SETUP3;
			_go = true;
			break;
		case Setup4:
			comB.CSRP = SETUP4;
			_go = true;
			break;
		case Setup5:
			comB.CSRP = SETUP5;
			_go = true;
			break;
		case Setup6:
			comB.CSRP = SETUP6;
			_go = true;
			break;
		case Setup7:
			comB.CSRP = SETUP7;
			_go = true;
			break;
		case Setup8:
			comB.CSRP = SETUP8;
			_go = true;
			break;
		default:
			Serial.println("Invalid Entry");
			break;
	}
	if(_go){
		comB.merge();
		comB.rolecall();
		updateConv();
		hold8(comB.out);
		scanMeas();
		digitalWrite(cs, LOW);
		convert();
		comB.reset();
	}
	convReg.configured = false;
	convReg.ready = false;
	convReg.active = false;
}

void CS5532::contMeas(reg r){
	Serial.print("\n\t\t-------- ");Serial.print(name);Serial.println(" Continuous Measurement --------");
	comB.reset();
	comB.MC = MC_CONT;
	comB.CC = CC_NORM_CV;
	convReg.cont = true;
	convReg.setup = r;
	bool _go = false;
	switch(r){
		case Setup1:
			comB.CSRP = SETUP1;
			_go = true;
			break;
		case Setup2:
			comB.CSRP = SETUP2;
			_go = true;
			break;
		case Setup3:
			comB.CSRP = SETUP3;
			_go = true;
			break;
		case Setup4:
			comB.CSRP = SETUP4;
			_go = true;
			break;
		case Setup5:
			comB.CSRP = SETUP5;
			_go = true;
			break;
		case Setup6:
			comB.CSRP = SETUP6;
			_go = true;
			break;
		case Setup7:
			comB.CSRP = SETUP7;
			_go = true;
			break;
		case Setup8:
			comB.CSRP = SETUP8;
			_go = true;
			break;
		default:
			Serial.println("Invalid Entry");
			break;
	}
	if(_go){
		comB.merge();
		comB.rolecall();
		updateConv();
		hold8(comB.out);
		scanMeas();
		//digitalWrite(cs, LOW);
		convert();
		comB.reset();
	}
	convReg.configured = true;
	convReg.ready = true;
	convReg.active = true;
}

void CS5532::stopMeas(){
	while(digitalRead(sdoFlag)){}
	hold8(SYNC1);
	command32(0x00000000);
	convReg.configured = false;
	convReg.ready = false;
	convReg.active = false;
}

void CS5532::convert(){
	//Serial.print("Conv BIN::\t");
	convReg.measDec = 0;
	int k = 0;
	if(convReg.bipolar){
		convReg.measDec = -d24[0]*pow(2,23);
		//Serial.print(d24[0]);
		++k;
		for(int i=1;i<24;++i){
			convReg.measDec = convReg.measDec + d24[i]*pow(2,23-i);
			++k;
			if(k>4){
				//Serial.print(" | ");
				k=1;
			}
			//Serial.print(d24[i]);
		}
		//convReg.measRat = convReg.measDec / pow(2,23);
		//convReg.measV = convReg.measRat * convReg.Vfs;
		//convReg.measV = convReg.measDec * pow(2,-23) * convReg.Vfs;// * (1/1.835007966);
		//Serial.print("\n");
		convReg.measV = convReg.LSB * convReg.measDec;
	}
	else{
		for(int i=0;i<24;++i){
			convReg.measDec = convReg.measDec + d24[i]*pow(2,23-i);
		}
		convReg.measRat = convReg.measDec / (pow(2,24)-1);
		convReg.measV = convReg.measRat * convReg.Vfs;
	}
	//Serial.print("\n");
	//print32(out);
	//Serial.print("\n");
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
}

void CS5532::rolecall(){
	parse();
	comA.rolecall();
	comB.rolecall();
	confReg.rolecall();
	setup1.rolecall();
	setup2.rolecall();
	setup3.rolecall();
	setup4.rolecall();
	setup5.rolecall();
	setup6.rolecall();
	setup7.rolecall();
	setup8.rolecall();
	offsetReg1.rolecall();
	offsetReg2.rolecall();
	gainReg1.rolecall();
	gainReg2.rolecall();
	convReg.rolecall();
}

void CS5532::rolecall(reg r){
	parse();
	switch(r){
		case CommandA:
			comA.rolecall();
			break;
		case CommandB:
			comB.rolecall();
			break;
		case Config:
			confReg.rolecall();
			break;
		case Setup1:
			setup1.rolecall();
			break;
		case Setup2:
			setup2.rolecall();
			break;
		case Setup3:
			setup3.rolecall();
			break;
		case Setup4:
			setup4.rolecall();
			break;
		case Setup5:
			setup5.rolecall();
			break;
		case Setup6:
			setup6.rolecall();
			break;
		case Setup7:
			setup7.rolecall();
			break;
		case Setup8:
			setup8.rolecall();
			break;
		case Offset1:
			offsetReg1.rolecall();
			break;
		case Offset2:
			offsetReg2.rolecall();
			break;
		case Gain1:
			gainReg1.rolecall();
			break;
		case Gain2:
			gainReg2.rolecall();
			break;
		case Conv:
			convReg.rolecall();
			break;
		default:
			Serial.println("Invalid Entry");
			break;
	}
}

void CS5532::updateConv(){
	confReg.rolecall();
	switch(confReg.VRS){
		case VRS_HIGH:
			convReg.vRefLow = false;
			convReg.A = 2.0;
			break;
		case VRS_LOW:
			convReg.vRefLow = true;
			convReg.A = 1.0;
			break;
		default:
			break;
	}
	bool _go = true;
	uint16_t _ub = 0;
	switch(convReg.setup){
		case Setup1:
			convReg.chCal = setup1.src;
			convReg.G = setup1.g;
			convReg.wr = setup1.wr;
			convReg.setupName = setup1.name;
			_ub = setup1.UB;
			break;
		case Setup2:
			convReg.chCal = setup2.src;
			convReg.G = setup2.g;
			convReg.wr = setup2.wr;
			convReg.setupName = setup2.name;
			_ub = setup2.UB;
			break;
		case Setup3:
			convReg.chCal = setup3.src;
			convReg.G = setup3.g;
			convReg.wr = setup3.wr;
			convReg.setupName = setup3.name;
			_ub = setup3.UB;
			break;
		case Setup4:
			convReg.chCal = setup4.src;
			convReg.G = setup4.g;
			convReg.wr = setup4.wr;
			convReg.setupName = setup4.name;
			_ub = setup4.UB;
			break;
		case Setup5:
			convReg.chCal = setup5.src;
			convReg.G = setup5.g;
			convReg.wr = setup5.wr;
			convReg.setupName = setup5.name;
			_ub = setup5.UB;
			break;
		case Setup6:
			convReg.chCal = setup6.src;
			convReg.G = setup6.g;
			convReg.wr = setup6.wr;
			convReg.setupName = setup6.name;
			_ub = setup6.UB;
			break;
		case Setup7:
			convReg.chCal = setup7.src;
			convReg.G = setup7.g;
			convReg.wr = setup7.wr;
			convReg.setupName = setup7.name;
			_ub = setup7.UB;
			break;
		case Setup8:
			convReg.chCal = setup8.src;
			convReg.G = setup8.g;
			convReg.wr = setup8.wr;
			convReg.setupName = setup8.name;
			_ub = setup8.UB;
			break;
		default:
			_go = false;
			Serial.println("Invalid Entry");
			break;
	}
	switch(_ub){
		case UB_BI:
			convReg.bipolar = true;
			convReg.kBi = 2.0;
			break;
		case UB_UNI:
			convReg.bipolar = false;
			convReg.kBi = 1.0;
			break;
		default:
			Serial.println("Invalid Polarity");
			_go = false;
			break;
	}
	switch(convReg.chCal){
		case 1:
			convReg.gainCal = gainReg1.gain;
			convReg.offCal = offsetReg1.offset;
			convReg.GcalType = gainReg1.calType;
			convReg.OcalType = offsetReg1.calType;
			break;
		case 2:
			convReg.gainCal = gainReg2.gain;
			convReg.offCal = offsetReg2.offset;
			convReg.GcalType = gainReg2.calType;
			convReg.OcalType = offsetReg2.calType;
			break;
		default:
			Serial.println("Invalid Register");
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
	flightCheck();
	if(_go && convReg.configured){
		convReg.ready = true;
	}
	else{
		convReg.ready = false;
	}
	convReg.Vfs = convReg.Vref/(convReg.G*convReg.A*convReg.gainCal);
	convReg.Vspan = convReg.kBi*convReg.Vfs;
	convReg.LSB = convReg.Vspan/pow(2,24);
	convReg.rolecall();
}

void CS5532::read8(uint8_t val){
	for(int i=0;i<8;++i){
		d8[i] = 0;
	}
	for(int i=0;i<8;++i){
		if(val>=pow(2,7-i)){
			d8[i] = 1;
			val = val - pow(2,7-i);
		}
		//Serial.print(d8[i]);
	}
}

void CS5532::read16(uint16_t val){
	for(int i=0;i<16;++i){
		d16[i] = 0;
	}
	for(int i=0;i<16;++i){
		if(val>=pow(2,15-i)){
			d16[i] = 1;
			val = val - pow(2,15-i);
		}
	//Serial.print(d16[i]);
	}
}

void CS5532::read32(uint32_t val){
	for(int i=0;i<32;++i){
		d32[i] = 0;
	}
	for(int i=0;i<32;++i){
		if(val>=pow(2,31-i)){
			d32[i] = 1;
			val = val - pow(2,31-i);
		}
	//Serial.print(d32[i]);
	}
}

void CS5532::command8(uint8_t val){
	digitalWrite(cs, HIGH);

	SPI.transfer(val);

	digitalWrite(cs, LOW);
}

void CS5532::hold8(uint8_t val){
	digitalWrite(cs, HIGH);
	SPI.transfer(val);
}

void CS5532::command16(uint16_t val){
	digitalWrite(cs, HIGH);

	SPI.transfer16(val);

	digitalWrite(cs, LOW);
}

void CS5532::command32(){
	digitalWrite(cs, HIGH);

	SPI.transfer16(outA);
	SPI.transfer16(outB);

	digitalWrite(cs, LOW);
}

void CS5532::command32(uint32_t val){
	wordSplit(val);
	digitalWrite(cs, HIGH);

	SPI.transfer16(outA);
	SPI.transfer16(outB);

	digitalWrite(cs, LOW);
}

void CS5532::scan(){
	digitalWrite(cs, HIGH);

	inA = SPI.transfer16(0x0000);
	inB = SPI.transfer16(0x0000);

	digitalWrite(cs, LOW);

	int b16a = inA;
	int b16b = inB;
	in = (inA << 16) | inB;
	
	for(int i=0;i<32;++i){
		d32[i] = 1&(in>>i);
	}
}

uint32_t CS5532::quickScan(){
	flag = false;
	while(!flag){}

	SPI.transfer(0x00);
	inA = SPI.transfer16(0x0000);
	inB = SPI.transfer16(0x0000);
	
	return (inA << 16) | inB;
}

void CS5532::scanMeas(){

	flag = false;
	while(!flag){}

	SPI.transfer(0x00);
	inA = SPI.transfer16(0x0000);
	inB = SPI.transfer16(0x0000);

	uint16_t b16a = inA;
	uint16_t b16b = inB;
	in = 0;
	
	for(int i=0;i<32;++i){
		d32[i] = 0;
		if(i<24){
			d24[i] = 0;
		}
		//Serial.print(d[i]);
	}
	//Serial.print("\nd32:\t\t");
	int k = 0;
	for(int i=0;i<16;++i){
		++k;
		if(k>4){
			//Serial.print(" | ");
			k=1;
		}
		if(b16a>=pow(2,15-i)){
			d32[i] = 1;
			b16a = b16a - pow(2,15-i);
		}
		//Serial.print(d32[i]);
	}
	for(int i=0;i<16;++i){
		++k;
		if(k>4){
			//Serial.print(" | ");
			k=1;
		}
		if(b16b>=pow(2,15-i)){
			d32[i+16] = 1;
			b16b = b16b - pow(2,15-i);
		}
		//Serial.print(d32[i+16]);
	}
	k = 0;
	//Serial.print("\nd24:\t\t");
	for(int i=0;i<32;++i){
		in = in + d32[i]*pow(2,31-i);
		if(i<24){
			++k;
			if(k>4){
				//Serial.print(" | ");
				k=1;
			}
			d24[i] = d32[i];
			//Serial.print(d24[i]);
		}
	}
	//Serial.print("\n");
	convReg.of = d32[29];
	convert();
}

float CS5532::scanMeas(int _notUsed){

	while(digitalRead(sdoFlag)){}

	SPI.transfer(0x00);
	inA = SPI.transfer16(0x0000);
	inB = SPI.transfer16(0x0000);

	int b16a = inA;
	int b16b = inB;
	in = 0;
	
	for(int i=0;i<32;++i){
		d32[i] = 0;
		if(i<24){
			d24[i] = 0;
		}
		//Serial.print(d[i]);
	}
	//Serial.print("\nd32:\t\t");
	int k = 0;
	for(int i=0;i<16;++i){
		++k;
		if(k>4){
			//Serial.print(" | ");
			k=1;
		}
		if(b16a>=pow(2,15-i)){
			d32[i] = 1;
			b16a = b16a - pow(2,15-i);
		}
		//Serial.print(d32[i]);
	}
	for(int i=0;i<16;++i){
		++k;
		if(k>4){
			//Serial.print(" | ");
			k=1;
		}
		if(b16b>=pow(2,15-i)){
			d32[i+16] = 1;
			b16b = b16b - pow(2,15-i);
		}
		//Serial.print(d32[i+16]);
	}
	k = 0;
	//Serial.print("\nd24:\t\t");
	for(int i=0;i<32;++i){
		in = in + d32[i]*pow(2,31-i);
		if(i<24){
			++k;
			if(k>4){
				//Serial.print(" | ");
				k=1;
			}
			d24[i] = d32[i];
			//Serial.print(d24[i]);
		}
	}
	//Serial.print("\n");
	convReg.of = d32[29];
	convert();
	return convReg.measV;
}

void CS5532::wordSplit(uint32_t val){
	outA = val >> 16;
	outB = val & 0xFFFF;
}

void CS5532::readReg(reg r){
	comA.reset();
	comA.RW = RW_R;
	switch(r){
		case(Config):
			comA.RSB = RSB_CONF;
			break;
		case(Setup1):
			comA.CS = CH1;
			comA.RSB = RSB_CSR;
			break;
		case(Setup2):
			comA.CS = CH1;
			comA.RSB = RSB_CSR;
			break;
		case(Setup3):
			comA.CS = CH2;
			comA.RSB = RSB_CSR;
			break;
		case(Setup4):
			comA.CS = CH2;
			comA.RSB = RSB_CSR;
			break;
		case(Setup5):
			comA.CS = CH3;
			comA.RSB = RSB_CSR;
			break;
		case(Setup6):
			comA.CS = CH3;
			comA.RSB = RSB_CSR;
			break;
		case(Setup7):
			comA.CS = CH4;
			comA.RSB = RSB_CSR;
			break;
		case(Setup8):
			comA.CS = CH4;
			comA.RSB = RSB_CSR;
			break;
		case(Gain1):
			comA.CS = CH1;
			comA.RSB = RSB_GAIN;
			break;
		case(Gain2):
			comA.CS = CH2;
			comA.RSB = RSB_GAIN;
			break;
		case(Offset1):
			comA.CS = CH1;
			comA.RSB = RSB_OFFSET;
			break;
		case(Offset2):
			comA.CS = CH2;
			comA.RSB = RSB_OFFSET;
			break;
		default:
			Serial.println("Invalid Entry");
			break;
	}
	comA.merge();
	hold8(comA.out);
	scan();
	comA.reset();
	
	switch(r){
		case(Config):
			confReg.out = in;
			confReg.parse();
			FRS = confReg.FRS;
			setup1.FRS = FRS;
			setup2.FRS = FRS;
			setup3.FRS = FRS;
			setup4.FRS = FRS;
			setup5.FRS = FRS;
			setup6.FRS = FRS;
			setup7.FRS = FRS;
			setup8.FRS = FRS;
			OGS = confReg.OGS;
			setup1.OGS = OGS;
			setup2.OGS = OGS;
			setup3.OGS = OGS;
			setup4.OGS = OGS;
			setup5.OGS = OGS;
			setup6.OGS = OGS;
			setup7.OGS = OGS;
			setup8.OGS = OGS;
			break;
		case(Setup1):
			setup1.out = inA;
			setup1.parse();
			break;
		case(Setup2):
			setup2.out = inB;
			setup2.parse();
			break;
		case(Setup3):
			setup3.out = inA;
			setup3.parse();
			break;
		case(Setup4):
			setup4.out = inB;
			setup4.parse();
			break;
		case(Setup5):
			setup5.out = inA;
			setup5.parse();
			break;
		case(Setup6):
			setup6.out = inB;
			setup6.parse();
			break;
		case(Setup7):
			setup7.out = inA;
			setup7.parse();
			break;
		case(Setup8):
			setup8.out = inB;
			setup8.parse();
			break;
		case(Gain1):
			gainReg1.out = in;
			gainReg1.parse();
			gainReg1.convert();
			break;
		case(Gain2):
			gainReg2.out = in;
			gainReg2.parse();
			gainReg2.convert();
			break;
		case(Offset1):
			offsetReg1.out = in;
			offsetReg1.parse();
			offsetReg1.convert();
			break;
		case(Offset2):
			offsetReg2.out = in;
			offsetReg2.parse();
			offsetReg2.convert();
			break;
		default:
			Serial.println("Invalid Entry");
			break;
	}
}

void CS5532::writeReg(reg r){
	comA.RW = RW_W;
	outA = 0;
	outB = 0;
	bool whole = false;
	switch(r){
		case(Config):
			comA.RSB = RSB_CONF;
			confReg.merge();
			out = confReg.out;
			whole = true;
			FRS = confReg.FRS;
			setup1.FRS = FRS;
			setup2.FRS = FRS;
			setup3.FRS = FRS;
			setup4.FRS = FRS;
			setup5.FRS = FRS;
			setup6.FRS = FRS;
			setup7.FRS = FRS;
			setup8.FRS = FRS;
			break;
		case(Setup1):
			comA.CS = CH1;
			comA.RSB = RSB_CSR;
			setup1.merge();
			setup2.merge();
			outA = setup1.out;
			outB = setup2.out;
			break;
		case(Setup2):
			comA.CS = CH1;
			comA.RSB = RSB_CSR;
			setup1.merge();
			setup2.merge();
			outA = setup1.out;
			outB = setup2.out;
			break;
		case(Setup3):
			comA.CS = CH2;
			comA.RSB = RSB_CSR;
			setup3.merge();
			setup4.merge();
			outA = setup3.out;
			outB = setup4.out;
			break;
		case(Setup4):
			comA.CS = CH2;
			comA.RSB = RSB_CSR;
			setup3.merge();
			setup4.merge();
			outA = setup3.out;
			outB = setup4.out;
			break;
		case(Setup5):
			comA.CS = CH3;
			comA.RSB = RSB_CSR;
			setup5.merge();
			setup6.merge();
			outA = setup5.out;
			outB = setup6.out;
			break;
		case(Setup6):
			comA.CS = CH3;
			comA.RSB = RSB_CSR;
			setup5.merge();
			setup6.merge();
			outA = setup5.out;
			outB = setup6.out;
			break;
		case(Setup7):
			comA.CS = CH4;
			comA.RSB = RSB_CSR;
			setup7.merge();
			setup8.merge();
			outA = setup7.out;
			outB = setup8.out;
			break;
		case(Setup8):
			comA.CS = CH4;
			comA.RSB = RSB_CSR;
			setup7.merge();
			setup8.merge();
			outA = setup7.out;
			outB = setup8.out;
			break;
		case(Gain1):
			comA.CS = CH1;
			comA.RSB = RSB_GAIN;
			//gainReg1.merge();
			out = gainReg1.out;
			whole = true;
			break;
		case(Gain2):
			comA.CS = CH2;
			comA.RSB = RSB_GAIN;
			//gainReg2.merge();
			out = gainReg2.out;
			whole = true;
			break;
		case(Offset1):
			comA.CS = CH1;
			comA.RSB = RSB_OFFSET;
			//offsetReg1.merge();
			out = offsetReg1.out;
			whole = true;
			break;
		case(Offset2):
			comA.CS = CH2;
			comA.RSB = RSB_OFFSET;
			//offsetReg2.merge();
			out = offsetReg2.out;
			whole = true;
			break;
		default:
			Serial.println("Invalid Entry");
			break;
	}
	comA.merge();
	hold8(comA.out);
	if(whole){
		command32(out);
	}
	else{
		command32();
	}
	comA.reset();
}