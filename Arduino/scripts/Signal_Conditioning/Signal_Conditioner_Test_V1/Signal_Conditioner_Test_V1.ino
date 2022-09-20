#include <SPI.h>
#include "Digipot.h"
//#include "RoachRunner_CS5532.h"

//Pin Assignments
//const byte shunt1 = 2;
//const byte shunt2 = 3;
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
#define CS1           0x0000
#define CS2           0x4000
#define CS3           0x8000
#define CS4           0x1200
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
    
//Conversion Data Output Register Options


void setup() {
  Serial.begin(115200);
  while(!Serial){}
  pinMode(sdo_flag, INPUT);
  makeTimer(osc,osc_freq);
  Serial.print("\n--------------------------\n");
  Digipot dpotH(cs_pot_H, inc_H, ud_H);
  Digipot dpotL(cs_pot_L, inc_L, ud_L);
  dpotH.reset(50,0,0,5);
  dpotL.reset(50,0,0,5);
  delay(2000);
  SPI.begin();
  SPI.beginTransaction(SPISettings((fosc),MSBFIRST,SPI_MODE0));
  pinMode(cs, OUTPUT);
  digitalWrite(cs, HIGH);
  initialize();
  
  command(commandB | MC_SINGLE | SETUP3);
  G = 1.0;
  while(digitalRead(sdo_flag)){}
  Serial.print("\nMeas Setup3:\t\t");
  command(0);
  read32();
  Serial.print("Data In:\t\t");extractMeas();twosComp();//wordWrite(data_in,32);

  command(commandB | MC_SINGLE | SETUP2);
  G = 2.0;
  delay(20);
  Serial.print("\nMeas Setup2:\t\t");
  command(0);
  read32();
  Serial.print("Data In:\t\t");extractMeas();twosComp();//wordWrite(data_in,32);

  command(commandB | MC_MULT | SETUP2);
}

void loop() {
  while(digitalRead(sdo_flag)){}
  //Serial.print("\nMeas Setup3:\t\t");
  command(0);
  read32();
  //Serial.print("Data In:\t\t");
  extractMeas();
  twosComp();
}

void makeTimer(int p, float freq){
  int m = 84/freq;
  int m2 = m/2;
  int32_t mask_PWM_pin = digitalPinToBitMask(p);
  REG_PMC_PCER1 = 1<<4; // activate clock for PWM controller
  REG_PIOC_PDR |= mask_PWM_pin; // activate peripheral functions for pin (disables all PIO functionality)
  REG_PIOC_ABSR |= mask_PWM_pin; // choose peripheral option B
  REG_PWM_CLK = 0; // choose clock rate, 0 → full MCLK as reference 84MHz
  REG_PWM_CMR6 = 0<<9; // select clock and polarity for PWM channel (pin7) → (CPOL = 0)
  REG_PWM_CPRD6 = m; // initialize PWM period → T = value/84MHz (value: up to 16bit), value=8 → 10.5MHz
  REG_PWM_CDTY6 = m2; // initialize duty cycle, REG_PWM_CPRD6 / value = duty cycle, for 8/4 = 50%
  REG_PWM_ENA = 1<<6; // enable PWM on PWM channel (pin 7 = PWML6)
}

void initialize() {
  delay(20);
  SPIS();
  SRS();
  initVRS();
  initCSR();
  initCal();
}

void SPIS(){
  for(int i=0;i<16;++i){
    command(SYNC1);
  }
  command(SYNC0);
}

void SRS(){
  command(RW_W | RSB_CONF);
  command32(RS_RESET);
  delayMicroseconds(20);
  command(RW_W | RSB_CONF);
  command32(RS_NORM);
  
  Serial.print("\nConf:\t\t\t");
  command(RW_R | RSB_CONF);
  read32();
  
  command(RW_W | RSB_CONF);
  command32(OGS_OG);
  
  Serial.print("Conf OGS:\t\t");
  command(RW_R | RSB_CONF);
  read32();
  
  Serial.print("Offset:\t\t\t");
  command(RW_R | RSB_OFFSET);
  read32();
  
  Serial.print("Gain:\t\t\t");
  command(RW_R | RSB_GAIN);
  read32();
  
  Serial.print("CSR:\t\t\t");
  command(RW_R | RSB_CH);
  read32();
  Serial.print("\n");
}

void initVRS(){
  command(RW_W | RSB_CONF);
  command32(VRS_HIGH | OGS_OG);
  
  Serial.print("VRS:\t\t\t");
  command(RW_R | RSB_CONF);
  read32();
  Serial.print("\n");
}

void initCSR(){
  command(CH1 | RW_W | RSB_CH);
  command16(CS1 | G1 | WR480 | UB_BI | OG_1);
  command16(CS1 | G2 | WR480 | UB_BI | OG_1);
  command(CH2 | RW_W | RSB_CH);
  command16(CS1 | G1 | WR480 | UB_BI | OG_2);
  command16(CS1 | G2 | WR480 | UB_BI | OG_2);
  
  Serial.print("Setups 1&2:\t\t");
  command(CH1 | RW_R | RSB_CH);
  read32();
  Serial.print("Setups 3&4:\t\t");
  command(CH2 | RW_R | RSB_CH);
  read32();
  Serial.print("Setups 1&2:\t\t");
  command(CH1 | RW_R | RSB_CH);
  read32();
  Serial.print("\n");
}

void initCal(){
  command(commandB | MC_SINGLE | SETUP3);
  G = 1.0;
  delay(20);
  Serial.print("Meas Setup3:\t\t");
  command(0);
  read32();
  Serial.print("Data In:\t\t");extractMeas();twosComp();//wordWrite(data_in,32);
  
  Serial.print("\nCh1 Offset Pre-Cal:\t");
  command(CH1 | RW_R | RSB_OFFSET);read32();
  
  command(commandB | SETUP3 | CC_SELF_OC);
  delay(20);
  
  Serial.print("Ch1 Offset:\t\t");
  command(CH1 | RW_R | RSB_OFFSET);
  read32();
  //extractMeas();twosComp();
  
  Serial.print("Ch2 Offset:\t\t");
  command(CH2 | RW_R | RSB_OFFSET);
  read32();
  
  Serial.print("Ch1 Gain:\t\t");
  command(CH1 | RW_R | RSB_GAIN);
  read32();
  
  Serial.print("Ch2 Gain:\t\t");
  command(CH2 | RW_R | RSB_GAIN);
  read32();
  Serial.print("\n");

  command(RW_W | RSB_CONF);
  command32(IS_SHORT | OGS_OG);
  command(SETUP2 | CC_SYS_OC);
  delay(20);
  command(RW_W | RSB_CONF);
  command32(IS_NORM | OGS_OG);

  Serial.print("Ch1 Offset:\t\t");
  command(CH1 | RW_R | RSB_OFFSET);
  read32();
  //extractMeas();twosComp();
  
  Serial.print("Ch2 Offset:\t\t");
  command(CH2 | RW_R | RSB_OFFSET);
  read32();
  
  Serial.print("Ch1 Gain:\t\t");
  command(CH1 | RW_R | RSB_GAIN);
  read32();
  
  Serial.print("Ch2 Gain:\t\t");
  command(CH2 | RW_R | RSB_GAIN);
  read32();
}

void wordSplit(uint32_t val){
  data_out_16_a = val >> 16;
  data_out_16_b = val & 0xFFFF;
}

void command(uint8_t val){
  digitalWrite(cs, LOW);

  SPI.transfer(val);

  digitalWrite(cs, HIGH);
  //delay(10);
}

void command8(uint8_t val){
  digitalWrite(cs, LOW);

  SPI.transfer(val);

  digitalWrite(cs, HIGH);
  //delay(10);
}

void command16(uint16_t val){
  digitalWrite(cs, LOW);

  SPI.transfer16(val);

  digitalWrite(cs, HIGH);
  //delay(10);
}

void command32(uint32_t val){
  wordSplit(val);
  digitalWrite(cs, LOW);
  
  SPI.transfer16(data_out_16_a);
  SPI.transfer16(data_out_16_b);

  digitalWrite(cs, HIGH);
  //delay(10);
}

void read32(){
  digitalWrite(cs, LOW);

  data_in_16_a = SPI.transfer16(0x0000);
  data_in_16_b = SPI.transfer16(0x0000);
  
  digitalWrite(cs, HIGH);
  
  int b16a = data_in_16_a;
  int b16b = data_in_16_b;
  
  //Serial.print("\nB16+B16:\n");
  //wordWrite(data_in_16_a,16);wordWrite(data_in_16_b,16);Serial.print("\n");
  
  data_in = 0;
  for(int i=0;i<32;++i){
    d32[i] = 0;
    //Serial.print(d[i]);
  }
  for(int i=0;i<16;++i){
    if(b16a>=pow(2,15-i)){
      d32[i] = 1;
      b16a = b16a - pow(2,15-i);
    }
    //Serial.print(d32[i]);
  }
  for(int i=0;i<16;++i){
    if(b16b>=pow(2,15-i)){
      d32[i+16] = 1;
      b16b = b16b - pow(2,15-i);
    }
    //Serial.print(d32[i+16]);
  }
  for(int i=0;i<32;++i){
    data_in = data_in + d32[i]*pow(2,31-i);
  }
  //Serial.println("B32:");
  //Serial.print("\n");
  //Serial.println(data_in);
  //wordWrite(data_in,32);Serial.print("\n");
}

void writeReg(uint32_t val, uint8_t r){
  digitalWrite(cs, LOW);

  SPI.transfer(r);
  SPI.transfer(val);

  digitalWrite(cs, HIGH);
  delay(10);
}

void byteWrite(int val){
  for(int i=0;i<8;++i){
    d8[i] = 0;
  }
  for(int i=0;i<8;++i){
    if(val>=pow(2,7-i)){
      d8[i] = 1;
      val = val - pow(2,7-i);
    }
    Serial.print(d8[i]);
  }
}

void wordWrite(uint32_t val, int b){
  int d[b];
  int k = 0;
  //Serial.println(val);
  for(int i=0; i<b; ++i){
    d[i] = 0;
  }
  for(int i=0; i<b; ++i){
    if(k==4){
      k=1;
      Serial.print(" | ");
    }
    else{
      ++k;
    }
    if(val>=pow(2,(b-1)-i)){
      d[i] = 1;
      val = val - pow(2,(b-1)-i);
    }
    Serial.print(d[i]);
  }
}

void extractMeas(){
  int b16a = data_in_16_a;
  int b16b = data_in_16_b;
  
  for(int i=0;i<24;++i){
    d24[i] = 0;
  }
  for(int i=0;i<16;++i){
    if(b16a>=pow(2,15-i)){
      d24[i] = 1;
      b16a = b16a - pow(2,15-i);
    }
    //Serial.print(d24[i]);
  }
  for(int i=0;i<8;++i){
    if(b16b>=pow(2,15-i)){
      d24[i+16] = 1;
      b16b = b16b - pow(2,15-i);
    }
    //Serial.print(d24[i+16]);
  }
  measBin = 0;
  for(int i=0;i<24;++i){
    measBin = measBin + d24[i]*pow(2,23-i);
  }
  //Serial.print("\n");Serial.println(measBin);
  //wordWrite(measBin,32);Serial.print("\n");
}

void binOffset(){
  V_O = V_S * float(measBin)/float(bins);
}

void twosComp(){
  meas2C = 0;
  meas2C = -d24[0]*pow(2,23);
  for(int i=1;i<24;++i){
    meas2C = meas2C + d24[i]*pow(2,23-i);
  }
  //Serial.println(meas2C);
  V_O = (V_S/(A*G)) * float(meas2C)/float(bins2C);
  Serial.println(V_O,10);
}
