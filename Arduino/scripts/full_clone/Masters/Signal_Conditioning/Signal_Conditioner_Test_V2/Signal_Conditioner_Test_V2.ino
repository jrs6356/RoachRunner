#include <SPI.h>
#include "Digipot.h"

//Pin Assignments
const byte cs = 8;
const byte osc = 7;
const byte sdo_flag = 5;

const byte inc_H = 2;
const byte cs_pot_H = 4;
const byte ud_H = 3;
const byte inc_L = 9;
const byte cs_pot_L = 11;
const byte ud_L = 10;

int fosc = 2e6;
double osc_freq = 4.9152;   //MHz
uint8_t com_in = 0;
uint8_t com_out = 0;
uint32_t data_in = 0;
uint32_t data_in_a = 0;
uint32_t data_out = 0;
uint16_t data_out_16_a = 0;
uint16_t data_out_16_b = 0;
uint16_t data_in_16_a = 0;
uint16_t data_in_16_b = 0;
uint8_t reg = 0;
uint32_t measBin = 0;
int meas2C = 0;
int bins = pow(2,24) - 1;
int bins2C = pow(2,23) - 1;
float V_O = 0.0;
int d8[8];
int d16[16];
int d24[24];
int d32[32];
float A = 2.0;
float G = 1.0;
float V_S = 4.549;

//----------------------Classes-----------------------
class Root {
public:
  void reset();
  void parse();
  void merge();
  void command8();
  void command16();
  void command32();
  void read32();
};

class ComA {
public:
  //comA(){}
  uint8_t out =   0x00;
  uint8_t ARA =   0x00;
  uint8_t CS =    0x00;
  uint8_t RW =    0x00;
  uint8_t RSB =   0x00;
  void merge(){
    out = ARA | CS | RW | RSB;
  }
  void print(){
    Serial.print("ARA:\t");
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
        Serial.println("001 - ");
        break;
      case RSB_GAIN:
        Serial.println("010 - ");
        break;
      case RSB_CONF:
        Serial.println("");
        break;
      case RSB_CSR:
        Serial.println("");
        break;
      default:
        Serial.println("Invalid Word");
        break;
    }
  }
};

class ComB {
public:
  //comB(){}
  uint8_t out =   0x00;
  uint8_t MC =    0x00;
  uint8_t CSRP =  0x00;
  uint8_t CC =    0x00;
  void merge(){
    out = 0x80 | MC | CSRP | CC;
  }
  void print(){
    Serial.print(":\t");
    switch(){
      case :
        Serial.println("");
        break;
      case :
        Serial.println("");
        break;
    }
  }
};

class ConfReg {
public:
  //confReg(){}
  uint32_t out = 0x00;
  uint32_t PSS = 0x00;
  uint32_t PDW = 0x00;
  uint32_t RS = 0x00;
  uint32_t RV = 0x00;   // Read only
  uint32_t IS = 0x00;
  uint32_t GB = 0x00;
  uint32_t VRS = 0x00;
  uint32_t A1 = 0x00;
  uint32_t A0 = 0x00;
  uint32_t OLS = 0x00;
  uint32_t OGS = 0x00;
  uint32_t FRS = 0x00;
  void merge(){
    out = PSS | PDW | RS | IS | GB | VRS | A1 | A0 | OLS | OGS | FRS;
  }
  void print(){
    Serial.print(":\t");
    switch(){
      case :
        Serial.println("");
        break;
      case :
        Serial.println("");
        break;
    }
  }
};

class Setup {
public:
  //Setup(){}
  String name = "";
  uint16_t out = 0x00;
  uint16_t CS = 0x00;
  uint16_t G = 0x00;
  uint16_t WR = 0x00;
  uint16_t UB = 0x00;
  uint16_t OL = 0x00;
  uint16_t DT = 0x00;
  uint16_t OCD = 0x00;
  uint16_t OG = 0x00;
  void merge(){
    out = CS | G | WR | UB | OL | DT | OCD | OG;
  }
  void print(){
    Serial.print(":\t");
    switch(){
      case :
        Serial.println("");
        break;
      case :
        Serial.println("");
        break;
    }
  }
};

class GainReg {
public:
  //gainReg(){}
  int channel = 0;
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
  uint32_t gain = 0x01000000;
  void print(){
    Serial.print(":\t");
    switch(){
      case :
        Serial.println("");
        break;
      case :
        Serial.println("");
        break;
    }
  }
};

class OffsetReg {
public:
  //offsetReg(){}
  int channel = 0;
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
  String calType = calTypes[1];
  uint32_t offset = 0x00000000;
  void calibrate(){
    
  }
  void print(){
    Serial.print(":\t");
    switch(){
      case :
        Serial.println("");
        break;
      case :
        Serial.println("");
        break;
    }
  }
};

class CS5532 {
public:
  CS5532(){
    
  }
  
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
};

//ADC adc1;

//Command Headers
#define commandA    0x00    //0:  ARA:  CS1:    CS0:    RW:     RSB2:   RSB1:   RSB0
#define commandB    0x80    //1:  MC:   CSRP2:  CSRP1:  CSRP0:  CC2:    CC1:    CC0

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
#define MC_MULT     0xC0
#define SETUP1      0x80
#define SETUP2      0x88
#define SETUP3      0x90
#define SETUP4      0x98
#define SETUP5      0xA0
#define SETUP6      0xA8
#define SETUP7      0xB0
#define SETUP8      0xB8
#define CC_NORM     0x80
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
#define G64           0x2000
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

void setup() {
  Serial.begin(115200);
  CS5532 adc1;
  adc1.offsetReg1.calType = adc1.offsetReg1.calTypes[8];
  Serial.println("-------------------------------------------");
  Serial.println(adc1.offsetReg1.calType);
  Serial.println(adc1.offsetReg2.calType);

}

void loop() {
  // put your main code here, to run repeatedly:

}
