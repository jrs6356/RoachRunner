#include <SPI.h>
#include <DueFlashStorage.h>
#include "RoachRunner_MUX.h"
#include "RoachRunner_LCD.h"
#include "RoachRunner_SGSC.h"
#include "RoachRunner_ADC.h"
#include "RoachRunner_IMU.h"
#include "RoachRunner_BLDC.h"

DueFlashStorage dueFlashStorage;

//Pins
byte rx0      = 0;
byte tx0      = 1;
byte gui_sw   = 2;
byte gui_b    = 3;
byte gui_a    = 4;
byte motor_b  = 5;
byte motor_a  = 6;
byte osc2     = 7;
byte fg       = 8;
byte esc_pwm  = 9;
byte esc_e    = 10;
byte esc_dir  = 11;
byte le       = 12;
byte lcd2_sw  = 13;
byte a3_1     = 22;
byte a2_1     = 23;
byte a1_1     = 24;
byte a0_1     = 25;
byte rs_lcd   = 26;
byte ud_digi  = 27;
byte inc_digi = 28;
byte a0_0     = 29;
byte a1_0     = 30;
byte a2_0     = 31;
byte a3_0     = 32;
byte e_mux_0  = 33;
byte e_mux_1  = 34;
byte e_mux_3  = 35;
byte sdo0     = 36;
byte sdo1     = 37;
byte sdo2     = 38;
byte sdo3     = 39;
byte sdo4     = 40;
byte sdo5     = 41;
byte sdo6     = 42;
byte sdo7     = 43;
byte sdo8     = 44;
byte sdo9     = 45;
byte sdo10    = 46;
byte sdo11    = 47;
byte sdo_sd   = 48;
byte sdo_imu  = 49;
byte led_clr  = 50;
byte led_clk  = 51;
byte lcd2_b   = 52;
byte lcd2_a   = 53;
byte lcd2_bkl = 68;
byte buzzer   = 69;

//MUX Channels
byte cs_0 = 0;
byte cs_1 = 1;
byte cs_2 = 2;
byte cs_3 = 3;
byte cs_4 = 4;
byte cs_5 = 5;
byte cs_6 = 6;
byte cs_7 = 7;
byte cs_8 = 8;
byte cs_9 = 9;
byte cs_10 = 10;
byte cs_11 = 11;
byte cs_lcd = 12;
byte cs_sd = 13;
byte cs_ag = 14;
byte cs_m = 15;
byte cs_digi = 28;
byte cs_lcd2 = 30;
byte cs_all = 31;

byte e_0 = 16;
byte e_1 = 17;
byte e_2 = 18;
byte e_3 = 19;
byte e_4 = 20;
byte e_5 = 21;
byte e_6 = 22;
byte e_7 = 23;
byte e_8 = 24;
byte e_9 = 25;
byte e_10 = 26;
byte e_11 = 27;
byte e_mux_2 = 29;
byte e_sdo_0 = 48;
byte e_sdo_1 = 49;
byte e_sdo_2 = 50;
byte e_sdo_3 = 51;
byte e_sdo_4 = 52;
byte e_sdo_5 = 53;
byte e_sdo_6 = 54;
byte e_sdo_7 = 55;
byte e_sdo_8 = 56;
byte e_sdo_9 = 57;
byte e_sdo_10 = 58;
byte e_sdo_11 = 59;
byte e_sdo_sd = 60;
byte e_sdo_imu = 61;

byte r2s30 = 64;
byte r2s31 = 65;
byte r2s20 = 66;
byte r2s21 = 67;
byte r2sg = 68;
byte r2sh = 69;
byte r3sg = 70;
byte r3sh = 71;
byte r3s10 = 72;
byte r3s11 = 73;
byte r3s20 = 74;
byte r3s21 = 75;
byte r3s30 = 76;
byte r3s31 = 77;
byte jk = 78;

//Variables
bool verboseRR = false;
bool verboseMux = false;
bool verboseFSM = true;
bool pinsSet = false;
bool muxReady = false;
bool clockStarted = false;
bool SPIStartedADC = false;
bool ADCSerialInit = false;
bool allClear = false;
bool syncADCs = true;

byte adcN = 0;
byte adcSetN = 0;
byte adcMic = 0;
uint32_t CONF = 0;
uint32_t CSR1 = 0;
uint32_t CSR2 = 0;
uint32_t CSR3 = 0;
uint32_t CSR4 = 0;
uint32_t CSR5 = 0;
uint32_t CSR6 = 0;
uint32_t CSR7 = 0;
uint32_t CSR8 = 0;
bool* flagPtrs[6] = {&verboseRR,&pinsSet,&clockStarted,&SPIStartedADC,&ADCSerialInit,&allClear};
byte* bytePtrs[5] = {&adcN,&adcSetN,&cs_all,&osc2,&adcMic};
uint32_t* adcSettings[9] = {&CONF,&CSR1,&CSR2,&CSR3,&CSR4,&CSR5,&CSR6,&CSR7,&CSR8};

volatile bool a_state = false;
volatile bool b_state = false;
volatile bool btnF = false;
volatile bool sdoF = false;
volatile bool scroll = false;
volatile bool bip = true;
volatile int dir = 0;
volatile int enc_val = 0;
volatile char enc_f = 'A';
volatile double esconIn = 0;
volatile double ti = 0;
volatile double tf = 0;
volatile double tc = 0;
//volatile float dt = 0;
float tspan = 2000000.0;
double tend = 0;
volatile float p = -1.0;
double speedIn = 0;
float vb = 0.0;
float gr = 3375.0/64.0;
int s10 = 0;
int s1 = 0;
int s01 = 0;
int sOutI = 0;
float sOutP = 0.0;
int sOut = 0;
int sPos = 2;
String lcdOut = "Buffer";
int loops = 0;
double kp = 10;
double ki = .5;
double kd = 50;

int num_blurbs = 0;
String topline = "";
String midline = "";
String botline = "";
String caption = "";
char buf[18] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
char ct = ' ';
char cm = ' ';
char cb = ' ';
int sel = 0;
int frame = 0;
int frame_prev = 0;
int screens = 0;
int rows = 3;
int row = 0;
int ind = 0;
int len = 0;
int bias = 0;
bool scrollText = false;
int rendersA = 0;
int renderA = 0;
int rendersB = 0;
int renderB = 0;
unsigned long tScroll = 300;
unsigned long t1 = 0;
unsigned long t2 = 0;
unsigned long dt = 0;
unsigned long t1A = 0;
unsigned long t2A = 0;
unsigned long dtA = 0;
unsigned long t1B = 0;
unsigned long t2B = 0;
unsigned long dtB = 0;
int light = 6;
int cont = 25;

bool adcTog = false;
bool accelTog = true;
bool bldcTog = false;

//Pages
enum pages{
  main,
  trial,
  gains,
  checkIAs,
  setIAs,
  setADCs,
  quickSet,
  motor,
  boards,
  calibrate,
  offsetCal,
  setFullScale,
  tuneConversion,
  freqCal,
  settings,
  setFs,
  setComms,
  setBrightness,
  setContrast,
  setBuzzer,
  lock
};
pages page = main;
pages ipage = page;

byte muxBytes[13] = {le,e_mux_0,e_mux_1,e_mux_2,e_mux_3,a0_0,a1_0,a2_0,a3_0,a0_1,a1_1,a2_1,a3_1};
byte bldcBytes[4] = {esc_e,esc_dir,esc_pwm,fg};
double bldcGains[3] = {kp,ki,kd};

MUX mux(muxBytes,&verboseMux,&pinsSet,&muxReady);
Digipot dpot(&cs_digi,&inc_digi,&ud_digi,&mux);
LCD lcd(&cs_lcd,&rs_lcd,&mux,&dpot);
IMU imu(cs_ag, cs_m, e_sdo_imu, &mux);
BLDC bldc(bldcBytes, bldcGains, &mux);
SGSC sc0("X1",&cs_0,&e_0,&e_sdo_0,&sdoF,bytePtrs,flagPtrs,adcSettings,&mux);
SGSC sc1("X2",&cs_1,&e_1,&e_sdo_1,&sdoF,bytePtrs,flagPtrs,adcSettings,&mux);
SGSC sc2("X3",&cs_2,&e_2,&e_sdo_2,&sdoF,bytePtrs,flagPtrs,adcSettings,&mux);
SGSC sc3("X4",&cs_3,&e_3,&e_sdo_3,&sdoF,bytePtrs,flagPtrs,adcSettings,&mux);
SGSC sc4("Y1",&cs_4,&e_4,&e_sdo_4,&sdoF,bytePtrs,flagPtrs,adcSettings,&mux);
SGSC sc5("Y2",&cs_5,&e_5,&e_sdo_5,&sdoF,bytePtrs,flagPtrs,adcSettings,&mux);
SGSC sc6("Y3",&cs_6,&e_6,&e_sdo_6,&sdoF,bytePtrs,flagPtrs,adcSettings,&mux);
SGSC sc7("Y4",&cs_7,&e_7,&e_sdo_7,&sdoF,bytePtrs,flagPtrs,adcSettings,&mux);
SGSC sc8("Z1",&cs_8,&e_8,&e_sdo_8,&sdoF,bytePtrs,flagPtrs,adcSettings,&mux);
SGSC sc9("Z2",&cs_9,&e_9,&e_sdo_9,&sdoF,bytePtrs,flagPtrs,adcSettings,&mux);
SGSC sc10("Z3",&cs_10,&e_10,&e_sdo_10,&sdoF,bytePtrs,flagPtrs,adcSettings,&mux);
SGSC sc11("Z4",&cs_11,&e_11,&e_sdo_11,&sdoF,bytePtrs,flagPtrs,adcSettings,&mux);

struct Setups {
  char* name[8] = {
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    " ",
    " "
  };
  uint32_t out[8] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
  };
  byte ga[8] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
  };
  byte gi[8] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
  };
  byte fs[8] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
  };
};

struct OutputSettings {
  float mSpeed;
  bool mDir;
  byte mUnits;
  float convM;
  float bodLen;
  
  bool togAccel;
  bool togBLDC;
  bool togTime;
  bool togADC[14];
  byte adcUnits;
  float convA;
};

struct Configuration {
  uint32_t x1Gcal;
  uint32_t x2Gcal;
  uint32_t x3Gcal;
  uint32_t x4Gcal;
  uint32_t y1Gcal;
  uint32_t y2Gcal;
  uint32_t y3Gcal;
  uint32_t y4Gcal;
  uint32_t z1Gcal;
  uint32_t z2Gcal;
  uint32_t z3Gcal;
  uint32_t z4Gcal;
  uint32_t x1Ocal;
  uint32_t x2Ocal;
  uint32_t x3Ocal;
  uint32_t x4Ocal;
  uint32_t y1Ocal;
  uint32_t y2Ocal;
  uint32_t y3Ocal;
  uint32_t y4Ocal;
  uint32_t z1Ocal;
  uint32_t z2Ocal;
  uint32_t z3Ocal;
  uint32_t z4Ocal;
  byte x1_Gia;
  byte x2_Gia;
  byte x3_Gia;
  byte x4_Gia;
  byte y1_Gia;
  byte y2_Gia;
  byte y3_Gia;
  byte y4_Gia;
  byte z1_Gia;
  byte z2_Gia;
  byte z3_Gia;
  byte z4_Gia;
  byte x1_Gadc;
  byte x2_Gadc;
  byte x3_Gadc;
  byte x4_Gadc;
  byte y1_Gadc;
  byte y2_Gadc;
  byte y3_Gadc;
  byte y4_Gadc;
  byte z1_Gadc;
  byte z2_Gadc;
  byte z3_Gadc;
  byte z4_Gadc;
  Setups x1_stps;
  Setups x2_stps;
  Setups x3_stps;
  Setups x4_stps;
  Setups y1_stps;
  Setups y2_stps;
  Setups y3_stps;
  Setups y4_stps;
  Setups z1_stps;
  Setups z2_stps;
  Setups z3_stps;
  Setups z4_stps;
  OutputSettings outSet;
  int Fs_i;
  int lcdB;
  int lcdC;
  float uO[3];
  float uGx[3];
  float uGy[3];
  float uGz[3];
  float pitchCals[4];
  float rollCals[4];
};
Configuration configuration;
bool loadIAGs = true;
bool firstRodeo = true;

void setup(){
  Serial.begin(250000);
  //SerialUSB.begin(2000000);
  while(!Serial){}
  //while(!SerialUSB){}
  delay(500);
  initFlash();
  delay(50);
  SPI.begin();
  delay(50);

  pinMode(gui_sw,INPUT);
  pinMode(gui_a,INPUT);
  pinMode(gui_b,INPUT);
  pinMode(sdo_imu,INPUT);
  pinMode(sdo0,INPUT_PULLUP);
  pinMode(sdo1,INPUT_PULLUP);
  pinMode(sdo2,INPUT_PULLUP);
  pinMode(sdo3,INPUT_PULLUP);
  pinMode(sdo4,INPUT_PULLUP);
  pinMode(sdo5,INPUT_PULLUP);
  pinMode(sdo6,INPUT_PULLUP);
  pinMode(sdo7,INPUT_PULLUP);
  pinMode(sdo8,INPUT_PULLUP);
  pinMode(sdo9,INPUT_PULLUP);
  pinMode(sdo10,INPUT_PULLUP);
  pinMode(sdo11,INPUT_PULLUP);
  pinMode(fg,INPUT_PULLUP);
  //pinMode(esc_pwm,OUTPUT);
  //pinMode(esc_e,OUTPUT);
  //pinMode(esc_dir,OUTPUT);
  
  //analogWriteResolution(12);
  //digitalWrite(esc_e,LOW);
  //digitalWrite(esc_dir,HIGH);
  //analogWrite(esc_pwm,0);
  attachInterrupt(digitalPinToInterrupt(gui_sw),btn,FALLING);
  attachInterrupt(digitalPinToInterrupt(gui_a),encA,CHANGE);
  attachInterrupt(digitalPinToInterrupt(gui_b),encB,CHANGE);
  attachInterrupt(digitalPinToInterrupt(fg),bldcPulse,RISING);
  attachInterrupt(digitalPinToInterrupt(sdo0),sdoF_isr,FALLING);
  attachInterrupt(digitalPinToInterrupt(sdo1),sdo1F_isr,FALLING);
  attachInterrupt(digitalPinToInterrupt(sdo2),sdo2F_isr,FALLING);
  attachInterrupt(digitalPinToInterrupt(sdo3),sdo3F_isr,FALLING);
  attachInterrupt(digitalPinToInterrupt(sdo4),sdo4F_isr,FALLING);
  attachInterrupt(digitalPinToInterrupt(sdo5),sdo5F_isr,FALLING);
  attachInterrupt(digitalPinToInterrupt(sdo6),sdo6F_isr,FALLING);
  attachInterrupt(digitalPinToInterrupt(sdo7),sdo7F_isr,FALLING);
  attachInterrupt(digitalPinToInterrupt(sdo8),sdo8F_isr,FALLING);
  attachInterrupt(digitalPinToInterrupt(sdo9),sdo9F_isr,FALLING);
  attachInterrupt(digitalPinToInterrupt(sdo10),sdo10F_isr,FALLING);
  attachInterrupt(digitalPinToInterrupt(sdo11),sdo11F_isr,FALLING);
  a_state = digitalRead(gui_a);
  b_state = digitalRead(gui_b);

  mux.init();
  lcd.init();
  lcd.contrast(cont);
  lcd.clear();
  bldc.init();
  imu.begin();
  imu.setupAccel(imu.LSM9DS1_ACCELRANGE_2G);
  imu.setupGyro(imu.LSM9DS1_GYROSCALE_245DPS);
  delay(10);

  setupADCs();
  //checkPGIAs();
  
  scroll = false;
  page = checkIAs;
}

void loop() {
  if(verboseFSM){
    Serial.println("-------------------------------------------------------");
    Serial.print("Page: ");Serial.println(page);
  }
  btnF = false;
  scroll = true;
  scrollText = false;
  dir = 0;
  frame = 0;
  frame_prev = 0;
  row = 0;
  lcd.clear();
  taskMaster();
}

void taskMaster(){
  switch(page){
    case main:
      page = pmain();
      break;
    case trial:
      page = ptrial();
      break;
    case checkIAs:
      page = pcheckIAs();
      break;
    case setIAs:
      page = psetIAs();
      break;
    case quickSet:
      page = pquickSet();
      break;
    case setADCs:
      page = psetADCs();
      break;
    case motor:
      page = pmotor();
      break;
    case gains:
      page = pgains();
      break;
    case calibrate:
      page = pcalibrate();
      break;
    case offsetCal:
      page = poffsetCal();
      break;
    case setFullScale:
      page = psetFullScale();
      break;
    case tuneConversion:
      page = ptuneConversion();
      break;
    case freqCal:
      page = pfreqCal();
      break;
    case settings:
      page = psettings();
      break;
    case setFs:
      page = psetFs();
      break;
    case setComms:
      page = psetComms();
      break;
    case setBrightness:
      page = psetBrightness();
      break;
    case setContrast:
      page = psetContrast();
      break;
    case setBuzzer:
      page = psetBuzzer();
      break;
    case lock:
      page = plock();
      break;
    default:
      page = pmain();
      break;
  }
}

pages pmain(){
  // Menu
  caption = "Main Menu";
  if(verboseFSM){
    Serial.println(caption);
  }
  num_blurbs = 7;
  String blurbs[] = {
    "Run Trial",
    "Quick Set",
    "Calibrate",
    "PGIA Gains",
    "Setup ADCs",
    "Setup Motor",
    "Settings"
  };
  pages states[] = {
    trial,
    quickSet,
    calibrate,
    gains,
    setADCs,
    motor,
    settings,
    main
  };
  ipage = menu(num_blurbs, blurbs, states);
  return ipage;
}

pages ptrial(){
  // Trial
  caption = "Run Trial";
  if(verboseFSM){
    Serial.println(caption);
  }
  lcd.makeExtraChars2();

  syncADCs = true;

  int cur = 0;
  int opt = 0;
  int prop = 0;
  int attr = 0;
  int val = 0;
  int dec = 0;
  int decs[5] = {0,0,0,0,0};
  int bods[4] = {0,0,0,0};
  int blck[9] = {2,2,2,3,5,1,1,3,2};
  bool ind = false;
  bool oSel = false;
  bool pSel = false;
  bool aSel = false;
  bool vSel = false;
  bool iSel = false;
  bool dSel = false;
  int row1 = 0;
  int row2 = 0;
  int row3 = 0;
  int t1 = 0;
  int m1 = 0;
  int b1 = 0;
  int t2 = 0;
  int m2 = 0;
  int b2 = 0;
  int t3 = 0;
  int m3 = 0;
  int b3 = 0;
  int aRng[3] = {4,4,5};
  
  float aclM[3] = {0,0,0};
  float aclU[3] = {0,0,0};
  float pitch = 0.0;
  float roll = 0.0;
  float mag = 0.0;
  uint32_t m[12];
  uint32_t mui[12];
  int mi[12];
  float mf[12];
  double md[12];
  String ms[12];
  unsigned long stamp = 0;
  double speedMeas = 0.0;
  SGSC* sc[12] = {
    &sc0,
    &sc1,
    &sc2,
    &sc3,
    &sc4,
    &sc5,
    &sc6,
    &sc7,
    &sc8,
    &sc9,
    &sc10,
    &sc11
  };
  uint32_t* gCals[12] = {
    &configuration.x1Gcal,
    &configuration.x2Gcal,
    &configuration.x3Gcal,
    &configuration.x4Gcal,
    &configuration.y1Gcal,
    &configuration.y2Gcal,
    &configuration.y3Gcal,
    &configuration.y4Gcal,
    &configuration.z1Gcal,
    &configuration.z2Gcal,
    &configuration.z3Gcal,
    &configuration.z4Gcal
  };
  uint32_t* oCals[12] = {
    &configuration.x1Ocal,
    &configuration.x2Ocal,
    &configuration.x3Ocal,
    &configuration.x4Ocal,
    &configuration.y1Ocal,
    &configuration.y2Ocal,
    &configuration.y3Ocal,
    &configuration.y4Ocal,
    &configuration.z1Ocal,
    &configuration.z2Ocal,
    &configuration.z3Ocal,
    &configuration.z4Ocal
  };
  String adcG[7] = {
    "1 ",
    "2 ",
    "4 ",
    "8 ",
    "16",
    "32",
    "64"
  };
  String pgiaG[4] = {
    "1   ",
    "10  ",
    "100 ",
    "1000"
  };
  String fs[10] = {
    "7.5 ",
    "15  ",
    "30  ",
    "60  ",
    "120 ",
    "240 ",
    "480 ",
    "960 ",
    "1920",
    "3840"
  };
  String opts[3] = {
    "Resume",
    "Modify Output",
    "End Trial"
  };
  String props[4] = {
    "Adjust Motor",
    "Toggle Data",
    "Modify Data Type",
    "Back"
  };
  String bldcA[5] = {
    " Enable: OFF     ",
    "Speed: 00.00 cm/s",
    " Direction: CW   ",
    "Units: cm/s      ",
    "Back"
  };
  String bldcU[10] = {
    "cm/s",
    "BL/s",
    "RPM - Belt",
    "RPM - Driver",
    "RPM - Motor",
    "Hz - Belt",
    "Hz - Driver",
    "Hz - Commutation F",
    "PWM",
    "Back"
  };
  String bldcSU[9] = {
    "cm/s ",
    "BL/s ",
    "RPM.b",
    "RPM.d",
    "RPM.m",
    "Hz.b ",
    "Hz.d ",
    "Hz.m ",
    "%    "
  };
  String togA[5] = {
    "ADC:               ",
    " Motor: OFF        ",
    " Accelerometer: OFF",
    " Timestamp: OFF    ",
    "Back"
  };
  String togADCsS[15] = {
    "Select All ",
    "X1: ON     ",
    "X2: ON     ",
    "X3: ON     ",
    "X4: ON     ",
    "Y1: ON     ",
    "Y2: ON     ",
    "Y3: ON     ",
    "Y4: ON     ",
    "Z1: ON     ",
    "Z2: ON     ",
    "Z3: ON     ",
    "Z4: ON     ",
    "Back",
    "All: ON    "
  };
  String datA[6] = {
    "Raw Conversion",
    "Measurement Bits",
    "Signed Int",
    "Ratio",
    "Force Estimate",
    "Back"
  };

  uint8_t dotIO[2] = {lcd.offDot,lcd.onDot};
  uint8_t dotSel[2] = {lcd.selDot,lcd.deselDot};
  OutputSettings* outS = &configuration.outSet;
  bool eMot = false;
  float mSpeed = outS->mSpeed;
  bool mDir = outS->mDir;
  byte mUnits = outS->mUnits;
  float* convM = &(outS->convM);
  float bodLen = outS->bodLen;
  byte adcU = outS->adcUnits;
  float* convA = &(outS->convA);
  bool togs[3] = {
    outS->togBLDC,
    outS->togAccel,
    outS->togTime
  };
  if(togs[0]){
    togA[1] = " Motor: ON ";
  }
  if(togs[1]){
    togA[2] = " Accelerometer: ON ";
  }
  if(togs[2]){
    togA[3] = " Timestamp: ON ";
  }
  if(mDir){
    bldcA[2] = " Direction: CCW";
  }
  bool togADC[14];
  for(int i=0;i<14;++i){
    togADC[i] = outS->togADC[i];
  }
  for(int i=0;i<12;++i){
    if(togADC[i+1]){
      togADCsS[i+1] = sc[i]->name+": "+"ON ";
    }
    else{
      togADCsS[i+1] = sc[i]->name+": "+"OFF";
    }
  }
  if(togADC[13]){
    togADCsS[14] = "All: ON ";
  }
  else{
    togADCsS[14] = "All: OFF";
  }
  bldcA[1] = "Speed: ";
  for(int i=0;i<5;++i){
    if(i<blck[mUnits]){
      decs[i] = (int)(mSpeed/pow(10,blck[mUnits]-(i+1)));
      mSpeed = mSpeed - decs[i]*pow(10,blck[mUnits]-(i+1));
      bldcA[1] = bldcA[1] + String(decs[i]);
    }
    else if(i>blck[mUnits]){
      decs[i] = (int)(mSpeed/pow(10,blck[mUnits]-i));
      mSpeed = mSpeed - decs[i]*pow(10,blck[mUnits]-i);
      bldcA[1] = bldcA[1] + String(decs[i]);
    }
    else{
      bldcA[1] = bldcA[1] + ".";
    }
  }
  mSpeed = outS->mSpeed;
  bldcA[1] = bldcA[1] + " " + bldcSU[mUnits];
  bldcA[3] = "Units: "+bldcSU[mUnits];
  
  syncBridges(r2sg);
  syncBridges(r3sg);
  for(int i=0;i<12;++i){
    sc[i]->setPGIA(sc[i]->adc.setup1.pgiaG);
    sc[i]->adc.uploadOffset(Offset1,*oCals[i],0);
    sc[i]->adc.uploadGain(Gain1,*gCals[i],0);
    Serial.print(sc[i]->name);
    Serial.print(": Gia="+pgiaG[sc[i]->pgiaG]);
    Serial.print(", Gadc="+adcG[sc[i]->adc.setup1.stpVals[2]]);
    Serial.print(", Fs="+fs[sc[i]->adc.setup1.stpVals[3]]+" Sps");
    Serial.print(", Ro=");
    Serial.print(sc[i]->adc.offsetReg1.offset,8);
    Serial.print(", Rg=");
    Serial.println(sc[i]->adc.gainReg1.gain,8);
  }
  
  sdoF = false;
  btnF = false;
  bool key = false;
  bool probe = false;
  verboseRR = false;
  scroll = false;
  bool scroll2 = false;
  
  lcd.command(lcd.row0[5]);
  lcd.print(caption);
  lcd.command(lcd.row3[0]);
  lcd.print(">Start          Back");

  syncADCs = true;
  
  while(!key){
    while(!scroll2){
      if(scroll){
        if(!cur){
          cur = 1;
          lcd.command(lcd.row3[0]);
          lcd.print(" ");
          lcd.command(lcd.row3[15]);
          lcd.print(">");
          scroll = false;
        }
        else if(cur==1){
          cur = 0;
          lcd.command(lcd.row3[0]);
          lcd.print(">");
          lcd.command(lcd.row3[15]);
          lcd.print(" ");
          scroll = false;
        }
        else{
          if(oSel){
            if(pSel){
              if(aSel){
                if(!prop){ //Motor Attributes
                  if(attr==1){ //Motor Speed
                    if(iSel){
                      if(dSel){
                        decs[dec] = decs[dec] + dir;
                        if(decs[dec]<0){
                          decs[dec] = 9;
                        }
                        if(decs[dec]>9){
                          decs[dec] = 0;
                        }
                        mSpeed = 0.0;
                        bldcA[1] = "Speed: ";
                        for(int i=0;i<5;++i){
                          if(i<blck[mUnits]){
                            mSpeed = mSpeed + decs[i]*pow(10,blck[mUnits]-(i+1));
                            bldcA[1] = bldcA[1] + decs[i];
                          }
                          else if(i>blck[mUnits]){
                            mSpeed = mSpeed + decs[i]*pow(10,blck[mUnits]-i);
                            bldcA[1] = bldcA[1] + decs[i];
                          }
                          else{
                            bldcA[1] = bldcA[1] + ".";
                          }
                        }
                        bldc.SET_SPEED(mu2PWM(mSpeed,bodLen,mUnits,convM));
                        bldcA[1] = bldcA[1] + " " + bldcSU[mUnits];
                        lcd.command(lcd.row2[7+dec]);
                        lcd.print(String(decs[dec]));
                        lcd.command(lcd.row2[7+dec]);
                        scroll = false;
                      }
                      else{
                        dec = dec + dir;
                        if(dec<0){
                          dec = 5;
                        }
                        if(dec>5){
                          dec = 0;
                        }
                        switch(dec){
                          case 0:
                            lcd.command(lcd.row2[7]);
                            break;
                          case 1:
                            lcd.command(lcd.row2[8]);
                            break;
                          case 2:
                            lcd.command(lcd.row2[9]);
                            break;
                          case 3:
                            lcd.command(lcd.row2[10]);
                            break;
                          case 4:
                            lcd.command(lcd.row2[11]);
                            break;
                          case 5:
                            lcd.command(lcd.row2[5]);
                            break;
                        }
                      }
                    }
                    else{
                      ind = !ind;
                      if(!ind){
                        lcd.command(lcd.row3[7]);
                      }
                      else{
                        lcd.command(lcd.row2[5]);
                      }
                      lcd.setCursor(0,1);
                    }
                    scroll = false;
                  }
                  else{ //Motor Units
                    val = val + dir;
                    row3 = row3 + dir;
                    scroll = false;
                    if(val>9){
                      val = 9;
                    }
                    if(val<0){
                      val = 0;
                    }
                    if(row3>2){
                      row3 = 2;
                    }
                    if(row3<0){
                      row3 = 0;
                    }
                    t3 = val - row3;
                    m3 = t3 + 1;
                    b3 = t3 + 2;

                    lcd.clear();
                    lcd.command(lcd.row0[0]);
                    lcd.print("Change Units - Motor");
                    lcd.command(lcd.row1[1]);
                    lcd.writeData(dotIO[mUnits==t3]);
                    lcd.print(bldcU[t3]);
                    lcd.command(lcd.row2[1]);
                    lcd.writeData(dotIO[mUnits==m3]);
                    lcd.print(bldcU[m3]);
                    lcd.command(lcd.row3[1]);
                    if(b3!=9){
                      lcd.writeData(dotIO[mUnits==b3]);
                    }
                    lcd.print(bldcU[b3]);
                    switch(row3){
                      case 0:
                        lcd.command(lcd.row1[1]);
                        lcd.writeData(dotSel[mUnits==t3]);
                        lcd.command(lcd.row1[0]);
                        break;
                      case 1:
                        lcd.command(lcd.row2[1]);
                        lcd.writeData(dotSel[mUnits==m3]);
                        lcd.command(lcd.row2[0]);
                        break;
                      case 2:
                        if(b3!=9){
                          lcd.command(lcd.row3[1]);
                          lcd.writeData(dotSel[mUnits==b3]);
                        }
                        lcd.command(lcd.row3[0]);
                        break;
                    }
                    lcd.print(">");
                    scroll = false;
                  }
                }
                else{ //Toggle ADCs
                  val = val + dir;
                  row3 = row3 + dir;
                  scroll = false;
                  int bnd = 13;
                  if(togADC[0]){
                    bnd = 2;
                  }
                  if(val>bnd){
                    val = bnd;
                  }
                  if(val<0){
                    val = 0;
                  }
                  if(row3<0){
                    row3 = 0;
                  }
                  if(row3>2){
                    row3 = 2;
                  }
                  if(togADC[0]){
                    t3 = 0;
                    m3 = 14;
                    b3 = 13;
                  }
                  else{
                    t3 = val - row3;
                    m3 = t3 + 1;
                    b3 = t3 + 2;
                  }
                  
                  lcd.clear();
                  lcd.command(lcd.row0[0]);
                  lcd.print("    Toggle ADCs");
                  lcd.command(lcd.row1[1]);
                  if(togADC[t3]){
                    lcd.writeData(lcd.onDot);
                  }
                  else{
                    lcd.writeData(lcd.offDot);
                  }
                  lcd.print(togADCsS[t3]);
                  lcd.command(lcd.row2[1]);
                  if(m3!=14){
                    if(togADC[m3]){
                      lcd.writeData(lcd.onDot);
                    }
                    else{
                      lcd.writeData(lcd.offDot);
                    }
                  }
                  else{
                    if(togADC[13]){
                      lcd.writeData(lcd.onDot);
                    }
                    else{
                      lcd.writeData(lcd.offDot);
                    }
                  }
                  lcd.print(togADCsS[m3]);
                  lcd.command(lcd.row3[1]);
                  if(b3!=13){
                    if(togADC[b3]){
                      lcd.writeData(lcd.onDot);
                    }
                    else{
                      lcd.writeData(lcd.offDot);
                    }
                  }
                  lcd.print(togADCsS[b3]);
                  switch(row3){
                    case 0:
                      if(togADC[0]){
                        val = 0;
                      }
                      lcd.command(lcd.row1[0]);
                      lcd.print(">");
                      if(togADC[t3]){
                        lcd.writeData(lcd.deselDot);
                      }
                      else{
                        lcd.writeData(lcd.selDot);
                      }
                      break;
                    case 1:
                      if(togADC[0]){
                        val = 14;
                      }
                      lcd.command(lcd.row2[0]);
                      lcd.print(">");
                      if(m3!=14){
                        if(togADC[m3]){
                          lcd.writeData(lcd.deselDot);
                        }
                        else{
                          lcd.writeData(lcd.selDot);
                        }
                      }
                      else{
                        if(togADC[13]){
                          lcd.writeData(lcd.deselDot);
                        }
                        else{
                          lcd.writeData(lcd.selDot);
                        }
                      }
                      break;
                    case 2:
                      lcd.command(lcd.row3[0]);
                      lcd.print(">");
                      if(togADC[0]){
                        val = 13;
                      }
                      else{
                        if(val==13){
                          
                        }
                        else if(togADC[b3]){
                          lcd.writeData(lcd.deselDot);
                        }
                        else{
                          lcd.writeData(lcd.selDot);
                        }
                      }
                      break;
                  }
                  lcd.command(lcd.row1[1]);
                  scroll = false;
                }
              }
              else{
                scroll = false;
                attr = attr + dir;
                row2 = row2 + dir;
                if(attr<0){
                  attr = 0;
                }
                if(attr>aRng[prop]){
                  attr = aRng[prop];
                }
                if(row2<0){
                  row2 = 0;
                }
                if(row2>2){
                  row2 = 2;
                }
                t2 = attr - row2;
                m2 = t2 + 1;
                b2 = t2 + 2;
                lcd.clear();
                int L = (20-props[prop].length())/2;
                lcd.command(lcd.row0[L]);
                lcd.print(props[prop]);
                switch(prop){
                  case 0: //Adjust Motor
                    lcd.command(lcd.row1[1]);
                    lcd.print(bldcA[t2]);
                    lcd.command(lcd.row2[1]);
                    lcd.print(bldcA[m2]);
                    lcd.command(lcd.row3[1]);
                    lcd.print(bldcA[b2]);
                    switch(t2){
                      case 0:
                        switch(row2){
                          case 0:
                            lcd.command(lcd.row1[1]);
                            lcd.writeData(dotSel[eMot]);
                            lcd.command(lcd.row3[1]);
                            lcd.writeData(dotIO[mDir]);
                            break;
                          case 1:
                            lcd.command(lcd.row1[1]);
                            lcd.writeData(dotIO[eMot]);
                            lcd.command(lcd.row3[1]);
                            lcd.writeData(dotIO[mDir]);
                            break;
                          case 2:
                            lcd.command(lcd.row1[1]);
                            lcd.writeData(dotIO[eMot]);
                            lcd.command(lcd.row3[1]);
                            lcd.writeData(dotSel[mDir]);
                            break;
                        }
                        break;
                      case 1:
                        lcd.command(lcd.row2[1]);
                        if(row2==1){
                          lcd.writeData(dotSel[mDir]);
                        }
                        else{
                          lcd.writeData(dotIO[mDir]);
                        }
                        break;
                      case 2:
                        lcd.command(lcd.row1[1]);
                        if(row2==0){
                          lcd.writeData(dotSel[mDir]);
                        }
                        else{
                          lcd.writeData(dotIO[mDir]);
                        }
                        break;
                    }
                    break;
                  case 1: //Toggle 
                    lcd.command(lcd.row1[1]);
                    lcd.print(togA[t2]);
                    lcd.command(lcd.row2[1]);
                    lcd.print(togA[m2]);
                    lcd.command(lcd.row3[1]);
                    lcd.print(togA[b2]);
                    switch(t2){
                      case 0:
                        lcd.command(lcd.row2[1]);
                        if(row2==1){
                          lcd.writeData(dotSel[togs[0]]);
                        }
                        else{
                          lcd.writeData(dotIO[togs[0]]);
                        }
                        lcd.command(lcd.row3[1]);
                        if(row2==2){
                          lcd.writeData(dotSel[togs[1]]);
                        }
                        else{
                          lcd.writeData(dotIO[togs[1]]);
                        }
                        break;
                      case 1:
                        lcd.command(lcd.row1[1]);
                        lcd.writeData(dotIO[togs[0]]);
                        lcd.command(lcd.row2[1]);
                        lcd.writeData(dotIO[togs[1]]);
                        lcd.command(lcd.row3[1]);
                        lcd.writeData(dotIO[togs[2]]);
                        switch(row2){
                          case 0:
                            lcd.command(lcd.row1[0]);
                            lcd.print(">");
                            lcd.writeData(dotSel[togs[0]]);
                            break;
                          case 1:
                            lcd.command(lcd.row2[0]);
                            lcd.print(">");
                            lcd.writeData(dotSel[togs[1]]);
                            break;
                          case 2:
                            lcd.command(lcd.row3[0]);
                            lcd.print(">");
                            lcd.writeData(dotSel[togs[2]]);
                            break;
                        }
                        break;
                      case 2:
                        lcd.command(lcd.row1[1]);
                        if(row2==0){
                          lcd.writeData(dotSel[togs[1]]);
                        }
                        else{
                          lcd.writeData(dotIO[togs[1]]);
                        }
                        lcd.command(lcd.row2[1]);
                        if(row2==1){
                          lcd.writeData(dotSel[togs[2]]);
                        }
                        else{
                          lcd.writeData(dotIO[togs[2]]);
                        }
                        break;
                    }
                    break;
                  case 2: //ADC Units
                    lcd.command(lcd.row1[1]);
                    lcd.writeData(dotIO[adcU==t2]);
                    lcd.print(datA[t2]);
                    lcd.command(lcd.row2[1]);
                    lcd.writeData(dotIO[adcU==m2]);
                    lcd.print(datA[m2]);
                    lcd.command(lcd.row3[1]);
                    if(b2!=5){
                      lcd.writeData(dotIO[adcU==b2]);
                    }
                    lcd.print(datA[b2]);
                    switch(row2){
                      case 0:
                        lcd.command(lcd.row1[1]);
                        lcd.writeData(dotSel[adcU==t2]);
                        break;
                      case 1:
                        lcd.command(lcd.row2[1]);
                        lcd.writeData(dotSel[adcU==m2]);
                        break;
                      case 2:
                        if(b2!=5){
                          lcd.command(lcd.row3[1]);
                          lcd.writeData(dotSel[adcU==b2]);
                        }
                        break;
                    }
                    scroll = false;
                    break;
                }
                switch(row2){
                  case 0:
                    lcd.command(lcd.row1[0]);
                    break;
                  case 1:
                    lcd.command(lcd.row2[0]);
                    break;
                  case 2:
                    lcd.command(lcd.row3[0]);
                    break;
                }
                lcd.print(">");
                scroll = false;
              }
            }
            else{
              scroll = false;
              btnF = false;
              prop = prop + dir;
              row1 = row1 + dir;
              if(prop<0){
                prop = 0;
              }
              if(prop>3){
                prop = 3;
              }
              if(row1>2){
                row1 = 2;
              }
              if(row1<0){
                row1 = 0;
              }
              t1 = prop - row1;
              m1 = t1 + 1;
              b1 = t1 + 2;
              lcd.clear();
              lcd.command(lcd.row0[0]);
              lcd.print("   Modify Output");
              lcd.command(lcd.row1[1]);
              lcd.print(props[t1]);
              lcd.command(lcd.row2[1]);
              lcd.print(props[m1]);
              lcd.command(lcd.row3[1]);
              lcd.print(props[b1]);
              switch(row1){
                case 0:
                  lcd.command(lcd.row1[0]);
                  break;
                case 1:
                  lcd.command(lcd.row2[0]);
                  break;
                case 2:
                  lcd.command(lcd.row3[0]);
                  break;
              }
              lcd.print(">");
              scroll = false;
            }
          }
          else{
            scroll = false;
            opt = opt + dir;
            if(opt<0){
              opt = 0;
            }
            if(opt>2){
              opt = 2;
            }
            switch(opt){
              case 0:
                lcd.command(lcd.row1[0]);
                lcd.print(">");
                lcd.command(lcd.row2[0]);
                lcd.print(" ");
                lcd.command(lcd.row3[0]);
                lcd.print(" ");
                break;
              case 1:
                lcd.command(lcd.row1[0]);
                lcd.print(" ");
                lcd.command(lcd.row2[0]);
                lcd.print(">");
                lcd.command(lcd.row3[0]);
                lcd.print(" ");
                break;
              case 2:
                lcd.command(lcd.row1[0]);
                lcd.print(" ");
                lcd.command(lcd.row2[0]);
                lcd.print(" ");
                lcd.command(lcd.row3[0]);
                lcd.print(">");
                break;
            }
            scroll = false;
          }
        }
      }
      if(btnF){
        if(!cur){
          cur = 2;
          scroll = false;
          scroll2 = true;
          lcd.clear();
          lcd.command(lcd.row0[3]);
          lcd.print("Activate Motor");
          lcd.command(lcd.row2[3]);
          lcd.writeData(lcd.omegaLC);
          lcd.print(" = ");
          for(int i=0;i<5;++i){
            if(i==blck[mUnits]){
              lcd.print(".");
            }
            else{
              lcd.print(String(decs[i]));
            }
          }
          lcd.print(" "+bldcSU[mUnits]);
          lcd.command(lcd.row3[0]);
          lcd.print("No ");
          lcd.writeData(lcd.backArrow);
          lcd.command(lcd.row3[15]);
          lcd.print("Yes ");
          lcd.writeData(lcd.arrowR);
          lcd.command(lcd.row3[19]);
          lcd.setCursor(0,1);
          btnF = false;
          probe = false;
          oSel = true;
          while(!btnF){
            if(scroll){
              oSel = !oSel;
              if(oSel){
                lcd.command(lcd.row3[19]);
              }
              else{
                lcd.command(lcd.row3[3]);
              }
              scroll = false;
            }
          }
          lcd.setCursor(0,0);
          delay(300);
          btnF = false;
          if(oSel){
            bldc.ON();
            bldc.DIR(mDir);
            bldc.SET_SPEED(mu2PWM(mSpeed,bodLen,mUnits,convM));
          }
          oSel = false;
          scroll = false;
          lcd.clear();
          lcd.command(lcd.row1[0]);
          lcd.print("  Trial Active...");
          lcd.command(lcd.row2[3]);
          lcd.writeData(lcd.omegaLC);
          lcd.print(" = ");
          for(int i=0;i<5;++i){
            if(i==blck[mUnits]){
              lcd.print(".");
            }
            else{
              lcd.print(String(decs[i]));
            }
          }
          lcd.print(" "+bldcSU[mUnits]);
          lcd.command(lcd.row3[15]);
          lcd.print("Menu");
          lcd.writeData(lcd.menuDown);
        }
        else if(cur==1){
          scroll = true;
          scroll2 = true;
          key = true;
          btnF = false;
          lcd.clear();
          lcd.command(lcd.row1[0]);
          lcd.print("  Exiting Trial...");
          delay(1500);
        }
        else{
          if(oSel){
            if(pSel){
              if(aSel){
                if(!prop){ //Motor
                  if(attr==1){
                    if(iSel){
                      if(dSel){
                        dSel = false;
                        lcd.setCursor(0,1);
                        btnF = false;
                      }
                      else{
                        if(dec==5){
                          iSel = false;
                          lcd.command(lcd.row3[7]);
                          ind = false;
                          btnF = false;
                        }
                        else{
                          if(dec!=blck[mUnits]){
                            dSel = true;
                            lcd.setCursor(1,0);
                            btnF = false;
                          }
                        }
                      }
                    }
                    else{
                      if(ind){
                        iSel = true;
                        lcd.command(lcd.row2[7]);
                        lcd.setCursor(0,1);
                        btnF = false;
                        delay(100);
                      }
                      else{
                        aSel = false;
                        iSel = false;
                        val = 0;
                        ind = false;
                        dec = 0;
                        t2 = 0;
                        m2 = 1;
                        b2 = 2;
                        row2 = 1;
                        lcd.clear();
                        int L = (20-props[prop].length())/2;
                        lcd.command(lcd.row0[L]);
                        lcd.print(props[prop]);
                        
                        lcd.command(lcd.row1[1]);
                        lcd.print(bldcA[0]);
                        lcd.command(lcd.row1[1]);
                        lcd.writeData(dotIO[eMot]);
                        lcd.command(lcd.row2[0]);
                        lcd.print(">"+bldcA[1]);
                        lcd.command(lcd.row3[1]);
                        lcd.print(bldcA[2]);
                        lcd.command(lcd.row3[1]);
                        lcd.writeData(dotIO[mDir]);
                        btnF = false;
                        scroll = false;
                        lcd.setCursor(0,0);
                      }
                    }
                  }
                  else{
                    if(val==9){ //Back
                      aSel = false;
                      val = 0;
                      t3 = 0;
                      m3 = 1;
                      b3 = 2;
                      row3 = 2;
                      t2 = 1;
                      m2 = 2;
                      b2 = 3;
                      row2 = 2;
                      lcd.clear();
                      int L = (20-props[prop].length())/2;
                      lcd.command(lcd.row0[L]);
                      lcd.print(props[prop]);
                      
                      lcd.command(lcd.row1[1]);
                      lcd.print(bldcA[1]);
                      lcd.command(lcd.row2[1]);
                      lcd.print(bldcA[2]);
                      lcd.command(lcd.row2[1]);
                      lcd.writeData(dotIO[mDir]);
                      lcd.command(lcd.row3[0]);
                      lcd.print(">"+bldcA[3]);
                      btnF = false;
                      scroll = false;
                    }
                    else{ //Set Motor Units
                      int oldU = mUnits;
                      mUnits = val;
                      if(val==1){
                        float blTemp = bodLen;
                        for(int i=0;i<4;++i){
                          bods[i] = (int)floor(blTemp);
                          blTemp = (blTemp - (int)blTemp)*10.0;
                        }
                        lcd.clear();
                        lcd.command(lcd.row0[0]);
                        lcd.print("Set Body Length [cm]");
                        lcd.command(lcd.row1[3]);
                        lcd.print("BL = ");
                        lcd.print(String(bods[0]));
                        lcd.print(".");
                        lcd.print(String(bods[1]));
                        lcd.print(String(bods[2]));
                        lcd.print(String(bods[3]));
                        lcd.print(" cm");
                        lcd.command(lcd.row3[14]);
                        lcd.print("Done ");
                        lcd.writeData(lcd.arrowR);
                        bool stay = true;
                        dec = 0;
                        ind = false;
                        btnF = false;
                        scroll = false;
                        lcd.command(lcd.row1[8]);
                        lcd.setCursor(0,1);
                        while(stay){
                          if(scroll){
                            if(ind){
                              bods[dec] = bods[dec] + dir;
                              if(bods[dec]<0){
                                bods[dec] = 9;
                              }
                              if(bods[dec]>9){
                                bods[dec] = 0;
                              }
                              bodLen = 0.0;
                              for(int i=0;i<4;++i){
                                bodLen = bodLen + bods[i]/pow(10.0,i);
                              }
                              lcd.print(String(bods[dec]));
                              switch(dec){
                                case 0:
                                  lcd.command(lcd.row1[8]);
                                  break;
                                case 1:
                                  lcd.command(lcd.row1[10]);
                                  break;
                                case 2:
                                  lcd.command(lcd.row1[11]);
                                  break;
                                case 3:
                                  lcd.command(lcd.row1[12]);
                                  break;
                              }
                              scroll = false;
                            }
                            else{
                              dec = dec + dir;
                              if(dec<0){
                                dec = 4;
                              }
                              if(dec>4){
                                dec = 0;
                              }
                              switch(dec){
                                case 0:
                                  lcd.command(lcd.row1[8]);
                                  break;
                                case 1:
                                  lcd.command(lcd.row1[10]);
                                  break;
                                case 2:
                                  lcd.command(lcd.row1[11]);
                                  break;
                                case 3:
                                  lcd.command(lcd.row1[12]);
                                  break;
                                case 4:
                                  lcd.command(lcd.row3[19]);
                                  break;
                              }
                              scroll = false;
                            }
                          }
                          if(btnF){
                            if(dec!=4){
                              ind = !ind;
                              if(ind){
                                lcd.setCursor(1,0);
                              }
                              else{
                                lcd.setCursor(0,1);
                              }
                            }
                            else{
                              lcd.setCursor(0,0);
                              lcd.clear();
                              lcd.command(lcd.row0[0]);
                              lcd.print("Change Units - Motor");
                              lcd.command(lcd.row1[1]);
                              lcd.writeData(dotIO[mUnits==t3]);
                              lcd.print(bldcU[t3]);
                              lcd.command(lcd.row2[1]);
                              lcd.writeData(dotIO[mUnits==m3]);
                              lcd.print(bldcU[m3]);
                              lcd.command(lcd.row3[1]);
                              if(b3!=9){
                                lcd.writeData(dotIO[mUnits==b3]);
                              }
                              lcd.print(bldcU[b3]);
                              switch(row3){
                                case 0:
                                  lcd.command(lcd.row1[1]);
                                  lcd.writeData(dotSel[mUnits==t3]);
                                  lcd.command(lcd.row1[0]);
                                  break;
                                case 1:
                                  lcd.command(lcd.row2[1]);
                                  lcd.writeData(dotSel[mUnits==m3]);
                                  lcd.command(lcd.row2[0]);
                                  break;
                                case 2:
                                  if(b3!=9){
                                    lcd.command(lcd.row3[1]);
                                    lcd.writeData(dotSel[mUnits==b3]);
                                  }
                                  lcd.command(lcd.row3[0]);
                                  break;
                              }
                              lcd.print(">");
                              stay = false;
                            }
                            btnF = false;
                          }
                        }
                      }
                      if(oldU!=mUnits){
                        mSpeed = convMU(mSpeed,bodLen,oldU,mUnits,convM);
                        float tempS = mSpeed;
                        bldcA[1] = "Speed: ";
                        for(int i=0;i<5;++i){
                          if(i<blck[mUnits]){
                            decs[i] = (int)(tempS/pow(10,blck[mUnits]-(i+1)));
                            tempS = tempS - decs[i]*pow(10,blck[mUnits]-(i+1));
                            bldcA[1] = bldcA[1] + String(decs[i]);
                          }
                          else if(i>blck[mUnits]){
                            decs[i] = (int)(tempS/pow(10,blck[mUnits]-i));
                            tempS = tempS - decs[i]*pow(10,blck[mUnits]-i);
                            bldcA[1] = bldcA[1] + String(decs[i]);
                          }
                          else{
                            bldcA[1] = bldcA[1] + ".";
                          }
                        }
                        bldcA[1] = bldcA[1] + " " + bldcSU[mUnits];
                      }
                      bldcA[3] = "Units: "+bldcSU[mUnits];
                      switch(row3){
                        case 0:
                          lcd.command(lcd.row1[1]);
                          lcd.writeData(dotSel[1]);
                          lcd.command(lcd.row2[1]);
                          lcd.writeData(dotIO[0]);
                          lcd.command(lcd.row3[1]);
                          lcd.writeData(dotIO[0]);
                          break;
                        case 1:
                          lcd.command(lcd.row1[1]);
                          lcd.writeData(dotIO[0]);
                          lcd.command(lcd.row2[1]);
                          lcd.writeData(dotSel[1]);
                          lcd.command(lcd.row3[1]);
                          lcd.writeData(dotIO[0]);
                          break;
                        case 2:
                          lcd.command(lcd.row1[1]);
                          lcd.writeData(dotIO[0]);
                          lcd.command(lcd.row2[1]);
                          lcd.writeData(dotIO[0]);
                          lcd.command(lcd.row3[1]);
                          lcd.writeData(dotSel[1]);
                          break;
                      }
                    }
                  }
                }
                else{ //Toggle ADCs
                  btnF = false;
                  if(!val){ //All ADCs
                    togADC[0] = !togADC[0];
                    if(togADC[0]){
                      lcd.command(lcd.row1[1]);
                      lcd.writeData(lcd.deselDot);
                      lcd.command(lcd.row2[1]);
                      lcd.writeData(dotIO[togADC[13]]);
                      lcd.print(togADCsS[14]);
                      lcd.command(lcd.row3[1]);
                      lcd.print("Back      ");
                    }
                    else{
                      lcd.command(lcd.row1[1]);
                      lcd.writeData(lcd.selDot);
                      lcd.command(lcd.row2[1]);
                      lcd.writeData(dotIO[togADC[1]]);
                      lcd.print(togADCsS[1]);
                      lcd.command(lcd.row3[1]);
                      lcd.writeData(dotIO[togADC[2]]);
                      lcd.print(togADCsS[2]);
                    }
                  }
                  else if(val==13){
                    btnF = false;
                    aSel = false;
                    scroll = false;
                    attr = 0;
                    row2 = 0;
                    t2 = 0;
                    m2 = 1;
                    b2 = 2;
                    lcd.clear();
                    int L = (20-props[prop].length())/2;
                    lcd.command(lcd.row0[L]);
                    lcd.print(props[prop]);
                    lcd.command(lcd.row1[0]);
                    lcd.print(">");
                    lcd.print(togA[0]);
                    lcd.command(lcd.row2[1]);
                    lcd.print(togA[1]);
                    lcd.command(lcd.row2[1]);
                    lcd.writeData(dotIO[togs[0]]);
                    lcd.command(lcd.row3[1]);
                    lcd.print(togA[2]);
                    lcd.command(lcd.row3[1]);
                    lcd.writeData(dotIO[togs[1]]);
                  }
                  else if(val==14){
                    togADC[13] = !togADC[13];
                    lcd.command(lcd.row2[1]);
                    if(togADC[13]){
                      togADCsS[14] = "All: ON  ";
                      lcd.writeData(lcd.deselDot);
                    }
                    else{
                      togADCsS[14] = "All: OFF";
                      lcd.writeData(lcd.selDot);
                    }
                    lcd.print(togADCsS[14]);
                  }
                  else{
                    togADC[val] = !togADC[val];
                    switch(row3){
                      case 0:
                        lcd.command(lcd.row1[1]);
                        break;
                      case 1:
                        lcd.command(lcd.row2[1]);
                        break;
                      case 2:
                        lcd.command(lcd.row3[1]);
                        break;
                    }
                    if(togADC[val]){
                      togADCsS[val] = sc[val-1]->name+": ON ";
                      lcd.writeData(lcd.deselDot);
                    }
                    else{
                      togADCsS[val] = sc[val-1]->name+": OFF";
                      lcd.writeData(lcd.selDot);
                    }
                    lcd.print(togADCsS[val]);
                    btnF = false;
                  }
                }
              }
              else{
                btnF = false;
                switch(prop){
                  case 0: //Adjust Motor
                    switch(attr){
                      case 0: //Enable
                        eMot = !eMot;
                        if(eMot){
                          bldcA[0] = " Enable: ON ";
                          bldc.ON();
                        }
                        else{
                          bldcA[0] = " Enable: OFF";
                          bldc.OFF();
                        }
                        lcd.command(lcd.row1[1]);
                        lcd.print(bldcA[0]);
                        lcd.command(lcd.row1[1]);
                        lcd.writeData(dotSel[eMot]);
                        break;
                      case 1: //Speed
                        aSel = true;
                        ind = true;
                        iSel = false;
                        lcd.clear();
                        lcd.command(lcd.row0[0]);
                        lcd.print("    Adjust Speed");
                        lcd.command(lcd.row2[3]);
                        lcd.writeData(lcd.omegaLC);
                        lcd.print(" = ");
                        for(int i=0;i<5;++i){
                          if(i==blck[mUnits]){
                            lcd.print(".");
                          }
                          else{
                            lcd.print(String(decs[i]));
                          }
                        }
                        lcd.print(" "+bldcSU[mUnits]);
                        lcd.command(lcd.row3[7]);
                        lcd.writeData(lcd.arrowL);
                        lcd.print(" Back");
                        lcd.command(lcd.row2[5]);
                        lcd.setCursor(0,1);
                        break;
                      case 2: //Direction
                        mDir = !mDir;
                        if(mDir){
                          bldcA[2] = " Direction: CCW";
                        }
                        else{
                          bldcA[2] = " Direction: CW ";
                        }
                        bldc.DIR(mDir);
                        switch(row2){
                          case 0:
                            lcd.command(lcd.row1[1]);
                            lcd.print(bldcA[2]);
                            lcd.command(lcd.row1[1]);
                            lcd.writeData(dotSel[mDir]);
                            break;
                          case 1:
                            lcd.command(lcd.row2[1]);
                            lcd.print(bldcA[2]);
                            lcd.command(lcd.row2[1]);
                            lcd.writeData(dotSel[mDir]);
                            break;
                          case 2:
                            lcd.command(lcd.row3[1]);
                            lcd.print(bldcA[2]);
                            lcd.command(lcd.row3[1]);
                            lcd.writeData(dotSel[mDir]);
                            break;
                        }
                        break;
                      case 3: //Units
                        aSel = true;
                        val = 0;
                        row3 = 0;
                        t3 = 0;
                        m3 = 1;
                        b3 = 2;
                        lcd.clear();
                        lcd.command(lcd.row0[0]);
                        lcd.print("Change Units - Motor");
                        lcd.command(lcd.row1[0]);
                        lcd.print("> "+bldcU[0]);
                        lcd.command(lcd.row1[1]);
                        lcd.writeData(dotSel[mUnits==0]);
                        lcd.command(lcd.row2[2]);
                        lcd.print(bldcU[1]);
                        lcd.command(lcd.row2[1]);
                        lcd.writeData(dotIO[mUnits==1]);
                        lcd.command(lcd.row3[2]);
                        lcd.print(bldcU[2]);
                        lcd.command(lcd.row3[1]);
                        lcd.writeData(dotIO[mUnits==2]);
                        break;
                      case 4: //Back
                        aSel = false;
                        pSel = false;
                        attr = 0;
                        lcd.clear();
                        lcd.command(lcd.row0[0]);
                        lcd.print("   Modify Output");
                        lcd.command(lcd.row1[0]);
                        lcd.print(">"+props[0]);
                        lcd.command(lcd.row2[1]);
                        lcd.print(props[1]);
                        lcd.command(lcd.row3[1]);
                        lcd.print(props[2]);
                        btnF = false;
                        scroll = false;
                        break;
                    }
                    break;
                  case 1: //Toggle
                    switch(attr){
                      case 0: //Toggle ADCs
                        aSel  = true;
                        row3 = 0;
                        val = 0;
                        t3 = 0;
                        m3 = 1;
                        b3 = 2;
                        lcd.clear();
                        lcd.command(lcd.row0[0]);
                        lcd.print("    Toggle ADCs");
                        lcd.command(lcd.row1[0]);
                        lcd.print("> Select All");
                        lcd.command(lcd.row1[1]);
                        if(togADC[0]){
                          lcd.writeData(lcd.deselDot);
                          lcd.command(lcd.row2[1]);
                          lcd.writeData(dotIO[togADC[13]]);
                          lcd.print(togADCsS[14]);
                          lcd.command(lcd.row3[1]);
                          lcd.print("Back");
                        }
                        else{
                          lcd.writeData(lcd.selDot);
                          lcd.command(lcd.row2[1]);
                          lcd.writeData(dotIO[togADC[1]]);
                          lcd.print(togADCsS[1]);
                          lcd.command(lcd.row3[1]);
                          lcd.writeData(dotIO[togADC[2]]);
                          lcd.print(togADCsS[2]);
                        }
                        break;
                      case 1: //Toggle Motor
                        togs[0] = !togs[0];
                        if(togs[0]){
                          togA[1] = " Motor: ON ";
                        }
                        else{
                          togA[1] = " Motor: OFF";
                        }
                        if(!row2){
                          lcd.command(lcd.row1[1]);
                          lcd.print(togA[1]);
                          lcd.command(lcd.row1[1]);
                          lcd.writeData(dotSel[togs[0]]);
                        }
                        else{
                          lcd.command(lcd.row2[1]);
                          lcd.print(togA[1]);
                          lcd.command(lcd.row2[1]);
                          lcd.writeData(dotSel[togs[0]]);
                        }
                        break;
                      case 2: //Toggle Accelerometer
                        togs[1] = !togs[1];
                        if(togs[1]){
                          togA[2] = " Accelerometer: ON ";
                        }
                        else{
                          togA[2] = " Accelerometer: OFF";
                        }
                        switch(row2){
                          case 0:
                            lcd.command(lcd.row1[1]);
                            lcd.print(togA[2]);
                            lcd.command(lcd.row1[1]);
                            lcd.writeData(dotSel[togs[1]]);
                            break;
                          case 1:
                            lcd.command(lcd.row2[1]);
                            lcd.print(togA[2]);
                            lcd.command(lcd.row2[1]);
                            lcd.writeData(dotSel[togs[1]]);
                            break;
                          case 2:
                            lcd.command(lcd.row3[1]);
                            lcd.print(togA[2]);
                            lcd.command(lcd.row3[1]);
                            lcd.writeData(dotSel[togs[1]]);
                            break;
                        }
                        break;
                      case 3: //Toggle Timestamp
                        togs[2] = !togs[2];
                        if(togs[2]){
                          togA[3] = " Timestamp: ON ";
                        }
                        else{
                          togA[3] = " Timestamp: OFF";
                        }
                        if(row2==1){
                          lcd.command(lcd.row2[1]);
                          lcd.print(togA[3]);
                          lcd.command(lcd.row2[1]);
                          lcd.writeData(dotSel[togs[2]]);
                        }
                        else{
                          lcd.command(lcd.row3[1]);
                          lcd.print(togA[3]);
                          lcd.command(lcd.row3[1]);
                          lcd.writeData(dotSel[togs[2]]);
                        }
                        break;
                      case 4: //Back
                        aSel = false;
                        pSel = false;
                        attr = 0;
                        lcd.clear();
                        lcd.command(lcd.row0[0]);
                        lcd.print("   Modify Output");
                        lcd.command(lcd.row1[1]);
                        lcd.print(props[0]);
                        lcd.command(lcd.row2[0]);
                        lcd.print(">"+props[1]);
                        lcd.command(lcd.row3[1]);
                        lcd.print(props[2]);
                        break;
                    }
                    break;
                  case 2: //ADC Units
                    if(attr==5){ //Back
                      aSel = false;
                      pSel = false;
                      attr = 0;
                      t1 = 0;
                      m1 = 1;
                      b1 = 2;
                      row1 = 2;
                      lcd.clear();
                      lcd.command(lcd.row0[0]);
                      lcd.print("   Modify Output");
                      lcd.command(lcd.row1[1]);
                      lcd.print(props[0]);
                      lcd.command(lcd.row2[1]);
                      lcd.print(props[1]);
                      lcd.command(lcd.row3[0]);
                      lcd.print(">"+props[2]);
                      btnF = false;
                      scroll = false;
                    }
                    else{ //Set ADC Units
                      adcU = attr;
                      switch(row2){
                        case 0:
                          lcd.command(lcd.row1[1]);
                          lcd.writeData(dotSel[1]);
                          lcd.command(lcd.row2[1]);
                          lcd.writeData(dotIO[0]);
                          lcd.command(lcd.row3[1]);
                          lcd.writeData(dotIO[0]);
                          break;
                        case 1:
                          lcd.command(lcd.row1[1]);
                          lcd.writeData(dotIO[0]);
                          lcd.command(lcd.row2[1]);
                          lcd.writeData(dotSel[1]);
                          lcd.command(lcd.row3[1]);
                          lcd.writeData(dotIO[0]);
                          break;
                        case 2:
                          lcd.command(lcd.row1[1]);
                          lcd.writeData(dotIO[0]);
                          lcd.command(lcd.row2[1]);
                          lcd.writeData(dotIO[0]);
                          lcd.command(lcd.row3[1]);
                          lcd.writeData(dotSel[1]);
                          break;
                      }
                    }
                    break;
                }
              }
            }
            else{
              if(prop==3){
                oSel = false;
                btnF = false;
                scroll = false;
                prop = 0;
                lcd.clear();
                lcd.command(lcd.row0[4]);
                lcd.print(caption);
                lcd.command(lcd.row1[1]);
                lcd.print("Resume");
                lcd.command(lcd.row2[0]);
                lcd.print(">Modify Output");
                lcd.command(lcd.row3[1]);
                lcd.print("End Trial");
              }
              else{
                pSel = true;
                btnF = false;
                scroll = false;
                attr = 0;
                row2 = 0;
                t2 = 0;
                m2 = 1;
                b2 = 2;
                lcd.clear();
                int L = (20-props[prop].length())/2;
                lcd.command(lcd.row0[L]);
                lcd.print(props[prop]);
                lcd.command(lcd.row1[0]);
                lcd.print(">");
                switch(prop){
                  case 0:
                    lcd.print(bldcA[0]);
                    lcd.command(lcd.row1[1]);
                    lcd.writeData(dotSel[eMot]);
                    lcd.command(lcd.row2[1]);
                    lcd.print(bldcA[1]);
                    lcd.command(lcd.row3[1]);
                    lcd.print(bldcA[2]);
                    lcd.command(lcd.row3[1]);
                    lcd.writeData(dotIO[mDir]);
                    break;
                  case 1:
                    lcd.print(togA[0]);
                    lcd.command(lcd.row2[1]);
                    lcd.print(togA[1]);
                    lcd.command(lcd.row2[1]);
                    lcd.writeData(dotIO[togs[0]]);
                    lcd.command(lcd.row3[1]);
                    lcd.print(togA[2]);
                    lcd.command(lcd.row3[1]);
                    lcd.writeData(dotIO[togs[1]]);
                    break;
                  case 2:
                    lcd.writeData(dotSel[adcU==t2]);
                    lcd.print(datA[0]);
                    lcd.command(lcd.row2[1]);
                    lcd.writeData(dotIO[adcU==m2]);
                    lcd.print(datA[1]);
                    lcd.command(lcd.row3[1]);
                    lcd.writeData(dotIO[adcU==b2]);
                    lcd.print(datA[2]);
                    break;
                }
              }
            }
          }
          else{
            if(!opt){
              lcd.clear();
              lcd.command(lcd.row1[0]);
              lcd.print(" Resuming Trial...");
              delay(1500);

              lcd.clear();
              lcd.command(lcd.row1[0]);
              lcd.print("  Trial Active...");
              lcd.command(lcd.row2[3]);
              lcd.writeData(lcd.omegaLC);
              lcd.print(" = ");
              for(int i=0;i<5;++i){
                if(i==blck[mUnits]){
                  lcd.print(".");
                }
                else{
                  lcd.print(String(decs[i]));
                }
              }
              lcd.print(" "+bldcSU[mUnits]);
              lcd.command(lcd.row3[15]);
              lcd.print("Menu");
              lcd.writeData(lcd.menuDown);
              scroll = false;
              scroll2 = true;
              btnF = false;
              probe = false;
            }
            else if(opt==1){
              oSel = true;
              btnF = false;
              scroll = false;
              prop = 0;
              row1 = 0;
              t1 = 0;
              m1 = 1;
              b1 = 2;
              lcd.clear();
              lcd.command(lcd.row0[0]);
              lcd.print("   Modify Output");
              lcd.command(lcd.row1[0]);
              lcd.print(">"+props[0]);
              lcd.command(lcd.row2[1]);
              lcd.print(props[1]);
              lcd.command(lcd.row3[1]);
              lcd.print(props[2]);
            }
            else{
              cur = 0;
              opt = 0;
              prop = 0;
              attr = 0;
              val = 0;
              oSel = true;
              pSel = false;
              aSel = false;
              scroll = false;
              scroll2 = false;
              btnF = false;
              probe = false;
              lcd.clear();
              lcd.command(lcd.row1[0]);
              lcd.print("  Pausing Trial...");
              delay(1500);

              lcd.clear();
              lcd.command(lcd.row1[0]);
              lcd.print("   Store Settings?");
              lcd.command(lcd.row3[0]);
              lcd.print(">Yes");
              lcd.command(lcd.row3[18]);
              lcd.print("No");
              while(!btnF){
                if(scroll){
                  oSel = !oSel;
                  if(oSel){
                    lcd.command(lcd.row3[0]);
                    lcd.print(">");
                    lcd.command(lcd.row3[17]);
                    lcd.print(" ");
                  }
                  else{
                    lcd.command(lcd.row3[0]);
                    lcd.print(" ");
                    lcd.command(lcd.row3[17]);
                    lcd.print(">");
                  }
                  scroll = false;
                }
              }
              if(oSel){
                oSel = false;

                lcd.clear();
                lcd.command(lcd.row1[0]);
                lcd.print(" Storing Settings...");
                delay(1500);
                
                outS->mSpeed = mSpeed;
                outS->mDir = mDir;
                outS->mUnits = mUnits;
                outS->bodLen = bodLen;
                outS->togBLDC = togs[0];
                outS->togAccel = togs[1];
                outS->togTime = togs[2];
                for(int i=0;i<14;++i){
                  outS->togADC[i] = togADC[i];
                }
                outS->adcUnits = adcU;
                updateFlash();
              }
              oSel = false;
              scroll = false;
              btnF = false;

              lcd.clear();
              lcd.command(lcd.row0[5]);
              lcd.print(caption);
              lcd.command(lcd.row3[0]);
              lcd.print(">Start          Back");
            }
          }
        }
        btnF = false;
      }
    }
    if(!key){
      syncADCs = true;
      sc0.adc.contMeas(Setup1,1);
      sdoF = false;
      scroll = false;
      float mc = *convM;
      float j = -1;
      unsigned long tstart = micros();
      while(!scroll){
        //Measure
        j+=1;
        sdoF = false;
        for(int i=0;i<12;++i){
          m[i] = sc[i]->adc.scanMeas();
        }
        Serial.print('\n');
        if(togs[2]){
          Serial.print(sc0.adc.timestamp);
        }
        Serial.print(',');
        Serial.print(j/((float)(sc0.adc.timestamp-tstart)/pow(10.0,6)));
        switch(adcU){
          case 0:
            for(int i=0;i<12;++i){
              Serial.print(',');
              Serial.print(m[i]);
            }
            break;
          case 1:
            for(int i=0;i<12;++i){
              Serial.print(',');
              Serial.print(m[i]>>8);
            }
            break;
          case 2:
            for(int i=0;i<12;++i){
              Serial.print(',');
              Serial.print(sc[i]->adc.conv2Dec());
            }
            break;
          case 3:
            for(int i=0;i<12;++i){
              Serial.print(',');
              Serial.print(((float)(sc[i]->adc.conv2Dec()))/pow(2.0,23),5);
            }
            break;
          case 4:
            for(int i=0;i<12;++i){
              Serial.print(',');
              Serial.print(sc[i]->adc.conv2F(),5);
            }
            break;
        }
        if(togs[1]){
          imu.read();
          sensors_event_t a, m, g, temp;
          imu.getEvent(&a, &m, &g, &temp);
          Serial.print(',');
          Serial.print(a.acceleration.x,5);
          Serial.print(',');
          Serial.print(a.acceleration.y,5);
          Serial.print(',');
          Serial.print(a.acceleration.z,5);
        }
        if(togs[0]){
          Serial.print(',');
          Serial.print((bldc.MEAS_SPEED())/mc);
        }

        //Print
        /*
        Serial.print("\n");
        Serial.print(j/((float)(stamp-tstart)/pow(10.0,6)));
        Serial.print(',');
        if(togs[2]){
          Serial.print(stamp);
        }
        if(togADC[0]){
          if(togADC[13]){
            for(int i=0;i<12;++i){
              Serial.print(',');
              Serial.print(ms[i]);
            }
          }
        }
        else{
          for(int i=0;i<12;++i){
            if(togADC[i+1]){
              Serial.print(',');
              Serial.print(ms[i]);
            }
          }
        }
        if(togs[1]){
          Serial.print(',');
          Serial.print(aclM[0],5);
          Serial.print(',');
          Serial.print(aclM[1],5);
          Serial.print(',');
          Serial.print(aclM[2],5);
        }
        if(togs[0]){
          Serial.print(',');
          Serial.print(speedMeas,4);
        }
        */
      }
      for(int i=0;i<12;++i){
        sc[i]->adc.stopMeas(0);
      }
      
      lcd.clear();
      lcd.command(lcd.row0[5]);
      lcd.print(caption);
      lcd.command(lcd.row1[0]);
      lcd.print(">Resume");
      lcd.command(lcd.row2[1]);
      lcd.print("Modify Output");
      lcd.command(lcd.row3[1]);
      lcd.print("End Trial");
      scroll2 = false;
      cur = 2;
    }
  }
  ipage = main;
  return ipage;
}

float convMU(float vIn, float bodLen, int uI, int uO, float* cm){
  float mrpm = mu2mrpm(vIn, bodLen, uI);
  float sO = 0.0;
  float mOmax = 12000.0; //@90%
  float m = 15000.0;
  float b = -1500.0;
  float mc = 0.0;
  switch(uO){
    case 0: //mRPM -> cm/s
      mc = ((1.0/(4.4*PI))*60.0*gr);
      sO = mrpm/mc;
      break;
    case 1: //mRPM -> bl/s
      mc = (bodLen*(1.0/(4.4*PI))*60.0*gr);
      sO = mrpm/mc;
      break;
    case 2: //mRPM -> bRPM
      mc = ((41.0/(4.4*PI))*gr);
      sO = mrpm/mc;
      break;
    case 3: //mRPM -> dRPM
      mc = gr;
      sO = mrpm/mc;
      break;
    case 4: //mRPM -> mRPM
      mc = 1.0;
      sO = mrpm;
      break;
    case 5: //mRPM -> bHz
      mc = (60.0*(41.0/(4.4*PI))*gr);
      sO = mrpm/mc;
      break;
    case 6: //mRPM -> dHz
      mc = (gr*60.0);
      sO = mrpm/mc;
      break;
    case 7: //mRPM -> mHz
      mc = 60.0;
      sO = mrpm/mc;
      break;
    case 8: //mRPM -> PWM(%)
      mc = 1.0;
      sO = 100.0*(mrpm - b)/m;
      break;
  }
  *cm = mc;
  return sO;
}

float mu2mrpm(float vIn, float bodLen, int uI){
  float mrpm = 0.0;
  float mOmax = 12000.0; //@90%
  float m = 15000.0;
  float b = -1500.0;
  float mc = 1.0;
  switch(uI){
    case 0: //cm/s -> mRPM
      mc = (1.0/(4.4*PI))*60.0*gr;
      mrpm = vIn*mc;
      break;
    case 1: //bl/s -> mRPM
      mc = bodLen*(1.0/(4.4*PI))*60.0*gr;
      mrpm = vIn*mc;
      break;
    case 2: //bRPM -> mRPM
      mc = (41.0/(4.4*PI))*gr;
      mrpm = vIn*mc;
      break;
    case 3: //dRPM -> mRPM
      mc = gr;
      mrpm = vIn*mc;
      break;
    case 4: //mRPM -> mRPM
      mc = 1.0;
      mrpm = vIn;
      break;
    case 5: //bHz -> mRPM
      mc = 60.0*(41.0/(4.4*PI))*gr;
      mrpm = vIn*mc;
      break;
    case 6: //dHz -> mRPM
      mc = gr*60.0;
      mrpm = vIn*mc;
      break;
    case 7: //mHz -> mRPM
      mc = 60.0;
      mrpm = vIn*mc;
      break;
    case 8: //PWM(%) -> mRPM
      mc = 1.0;
      mrpm = (vIn/100.0)*m + b;
      break;
  }
  return mrpm;
}

uint32_t mu2PWM(float vIn, float bodLen, int mU, float* cm){
  float pwmFrac = 0.0;
  uint32_t pwmI = 0;
  if(mU<8){
    pwmFrac = convMU(vIn, bodLen, mU, 8, cm)/100.0;
  }
  else{
    pwmFrac = vIn/100.0;
  }
  pwmI = pwmFrac*4096;
  return pwmI;
}

pages pcheckIAs(){
  lcd.makeExtraChars();
  lcd.command(lcd.row1[0]);
  lcd.print(" Resetting PGIAs...");
  checkPGIAs();
  lcd.clear();
  caption = "  Check PGIA Gains";
  if(verboseFSM){
    Serial.println(caption);
  }
  lcd.command(lcd.row0[0]);
  lcd.print(caption);
  lcd.command(lcd.row1[0]);
  lcd.print("Are all A0,A1 = 0,0?");
  lcd.command(lcd.row3[0]);
  lcd.print(">Yes              No");
  int brd = 0;
  String brds[13] = {"X1",
                     "X2",
                     "X3",
                     "X4",
                     "Y1",
                     "Y2",
                     "Y3",
                     "Y4",
                     "Z1",
                     "Z2",
                     "Z3",
                     "Z4",
                     "Done"};
  byte cur = 0;
  bool gSel = false;
  scroll = false;
  btnF = false;
  while(1){
    if(scroll){
      if(cur==0){
        cur = 2;
        lcd.command(lcd.row3[0]);
        lcd.print(" ");
        lcd.command(lcd.row3[17]);
        lcd.print(">");
      }
      else if(cur==1){
        if(gSel){
          switch(brd){
            case 0:
              sc0.jk();
              break;
            case 1:
              sc1.jk();
              break;
            case 2:
              sc2.jk();
              break;
            case 3:
              sc3.jk();
              break;
            case 4:
              sc4.jk();
              break;
            case 5:
              sc5.jk();
              break;
            case 6:
              sc6.jk();
              break;
            case 7:
              sc7.jk();
              break;
            case 8:
              sc8.jk();
              break;
            case 9:
              sc9.jk();
              break;
            case 10:
              sc10.jk();
              break;
            case 11:
              sc11.jk();
              break;
          }
        }
        else{
          brd = brd + dir;
          if(brd>12){
            brd = 11;
          }
          if(brd<0){
            brd = 0;
          }
          lcd.command(lcd.row2[7]);
          lcd.print("G: "+brds[brd]);
          lcd.command(lcd.row2[7]);
        }
      }
      else if(cur==2){
        cur = 0;
        lcd.command(lcd.row3[0]);
        lcd.print(">");
        lcd.command(lcd.row3[17]);
        lcd.print(" ");
      }
      scroll = false;
    }
    if(btnF){
      if(cur==0){
        ipage = main;
        scroll = false;
        btnF = false;
        if(loadIAGs){
          lcd.clear();
          lcd.command(lcd.row1[0]);
          lcd.print("  Loading PGIAs...");
          delay(1500);
          loadPGIAs();
        }
        return ipage;
      }
      else if(cur==1){
        if(gSel){
          gSel = false;
          lcd.setCursor(0,0);
          lcd.command(lcd.row2[7]);
        }
        else{
          if(brd==12){
            cur = 0;
            lcd.clear();
            lcd.command(lcd.row0[0]);
            lcd.print(caption);
            lcd.command(lcd.row1[0]);
            lcd.print("Are all A0,A1 = 0,0?");
            lcd.command(lcd.row3[0]);
            lcd.print(">Yes              No");
          }
          else{
            gSel = true;
            lcd.setCursor(0,1);
            lcd.command(lcd.row2[7]);
          }
        }
      }
      else{
        cur = 1;
        brd = 0;
        lcd.clear();
        lcd.command(lcd.row0[0]);
        lcd.print("   Set incorrect");
        lcd.command(lcd.row1[0]);
        lcd.print("   boards to 0,0");
        lcd.command(lcd.row2[7]);
        lcd.print("G: "+brds[brd]);
        lcd.command(lcd.row2[7]);
      }
      btnF = false;
    }
  }
}

pages psetIAs(){
  caption = "   Set PGIA Gains";
  lcd.command(lcd.row0[0]);
  lcd.print(caption);
  byte cur = 0;
  int brd = 0;
  String brds[13] = {"X1",
                     "X2",
                     "X3",
                     "X4",
                     "Y1",
                     "Y2",
                     "Y3",
                     "Y4",
                     "Z1",
                     "Z2",
                     "Z3",
                     "Z4",
                     "Done"};                     
  byte* Gb[12] = {&configuration.x1_Gia,
                  &configuration.x2_Gia,
                  &configuration.x3_Gia,
                  &configuration.x4_Gia,
                  &configuration.y1_Gia,
                  &configuration.y2_Gia,
                  &configuration.y3_Gia,
                  &configuration.y4_Gia,
                  &configuration.z1_Gia,
                  &configuration.z2_Gia,
                  &configuration.z3_Gia,
                  &configuration.z4_Gia};
  SGSC* sc[12] = {&sc0,
                  &sc1,
                  &sc2,
                  &sc3,
                  &sc4,
                  &sc5,
                  &sc6,
                  &sc7,
                  &sc8,
                  &sc9,
                  &sc10,
                  &sc11};
  String Gs[4] = {"1    ",
                  "10   ",
                  "100  ",
                  "1000 "};
  btnF = false;
  scroll = false;
  bool gSel = false;
  lcd.command(lcd.row1[0]);
  lcd.print("    "+brds[brd]+": G = "+String(Gs[*Gb[brd]]));
  lcd.command(lcd.row2[0]);
  lcd.print("  A0 = "+String(1&(*Gb[brd]))+" , A1 = "+String(1&(*Gb[brd]>>1)));
  lcd.command(lcd.row3[0]);
  lcd.print(">Set            Save");
  while(1){
    if(scroll){
      if(cur==0){
        cur = 2;
        lcd.command(lcd.row3[0]);
        lcd.print(" Set           >Save");
      }
      else if(cur==1){
        if(gSel){
          *Gb[brd] = *Gb[brd] + dir;
          if(*Gb[brd]>4){
            if(dir==1){
              *Gb[brd] = 4;
            }
            else{
              *Gb[brd] = 0;
            }
          }
          else if(*Gb[brd]<4){
            if(dir==1){
              sc[brd]->jk();
            }
            else{
              if(*Gb[brd]<3){
                sc[brd]->jk();
                sc[brd]->jk();
                sc[brd]->jk();
              }
            }
          }
        }
        else{
          brd = brd + dir;
          if(brd<0){
            brd = 0;
          }
          if(brd>12){
            brd = 12;
          }
        }
        if(brd==12){
          lcd.clear();
          lcd.command(lcd.row0[0]);
          lcd.print(caption);
          lcd.command(lcd.row2[8]);
          lcd.print("Done");
        }
        else{
          lcd.command(lcd.row1[0]);
          if(*Gb[brd]==4){
            lcd.print("    "+brds[brd]+": G = Reset");
            lcd.command(lcd.row2[0]);
            lcd.print("  A0 = "+String(1&(*Gb[brd]-1))+" , A1 = "+String(1&((*Gb[brd]-1)>>1)));
          }
          else{
            lcd.print("    "+brds[brd]+": G = "+String(Gs[*Gb[brd]]));
            lcd.command(lcd.row2[0]);
            lcd.print("  A0 = "+String(1&(*Gb[brd]))+" , A1 = "+String(1&(*Gb[brd]>>1)));
          }
          lcd.command(lcd.row1[10]);
        }
      }
      else if(cur==2){
        cur = 0;
        lcd.command(lcd.row3[0]);
        lcd.print(">Set            Save");
      }
      scroll = false;
    }
    if(btnF){
      if(cur==0){
        cur = 1;
        brd = 0;
        gSel = false;
        lcd.command(lcd.row3[0]);
        lcd.print("                    ");
        lcd.command(lcd.row1[0]);
        lcd.print("    "+brds[brd]+": G = "+String(Gs[*Gb[brd]]));
        lcd.command(lcd.row2[0]);
        lcd.print("  A0 = "+String(1&(*Gb[brd]))+" , A1 = "+String(1&(*Gb[brd]>>1)));
        lcd.command(lcd.row1[10]);
      }
      else if(cur==1){
        if(*Gb[brd]==4){
          sc[brd]->checkPGIA();
          sc[brd]->jk();
          *Gb[brd]=0;
          lcd.command(lcd.row1[12]);
          lcd.print("1     ");
          lcd.command(lcd.row2[2]);
          lcd.print("A0 = 0 , A1 = 0");
          lcd.command(lcd.row1[10]);
        }
        else{
          gSel = !gSel;
          if(brd==12){
            cur = 0;
            brd = 0;
            lcd.command(lcd.row1[0]);
            lcd.print("    "+brds[brd]+": G = "+String(Gs[*Gb[brd]]));
            lcd.command(lcd.row2[0]);
            lcd.print("  A0 = "+String(1&(*Gb[brd]))+" , A1 = "+String(1&(*Gb[brd]>>1)));
            lcd.command(lcd.row1[10]);
            lcd.command(lcd.row3[0]);
            lcd.print(">Set            Save");
          }
          else if(gSel){
            lcd.setCursor(0,1);
            lcd.command(lcd.row1[10]);
          }
          else{
            lcd.setCursor(0,0);
          }
        }
      }
      else{
        ipage = gains;
        btnF = false;
        scroll = false;
        lcd.clear();
        lcd.command(lcd.row1[0]);
        lcd.print("   Saving PGIAs...");
        delay(1500);
        updateFlash();
        return ipage;
      }
      btnF = false;
    }
  }
}

pages pquickSet(){
  caption = "  Configure Trial";
  if(verboseFSM){
    Serial.println(caption);
  }
  lcd.command(lcd.row0[0]);
  lcd.print(caption);
  int ax = 0;
  int rgst = 0;
  int attr = 0;
  int val = 0;
  int row1 = 0;
  int row2 = 0;
  int row3 = 0;
  int t1 = 0;
  int m1 = 1;
  int b1 = 2;
  int t2 = 0;
  int m2 = 1;
  int b2 = 2;
  int t3 = 0;
  int m3 = 1;
  int b3 = 2;
  bool dSel = false;
  bool aSel = false;
  bool rSel = false;
  int bnds[3] = {
    3,
    6,
    9
  };
  SGSC* sc[12] = {
    &sc0,
    &sc1,
    &sc2,
    &sc3,
    &sc4,
    &sc5,
    &sc6,
    &sc7,
    &sc8,
    &sc9,
    &sc10,
    &sc11
  };
  String axes[4] = {
    " X Axis",
    " Y Axis",
    " Z Axis",
    " Done"
  };
  String attrs[4] = {
    " Gpgia: ",
    " Gadc : ",
    " Fs   : ",
    " Done   "
  };
  String adcG[7] = {
    "1 ",
    "2 ",
    "4 ",
    "8 ",
    "16",
    "32",
    "64"
  };
  int adcGb[7] = {
    G1,
    G2,
    G4,
    G8,
    G16,
    G32,
    G64
  };
  String pgiaG[4] = {
    "1   ",
    "10  ",
    "100 ",
    "1000"
  };
  String fs[10] = {
    "7.5 ",
    "15  ",
    "30  ",
    "60  ",
    "120 ",
    "240 ",
    "480 ",
    "960 ",
    "1920",
    "3840"
  };
  int fsb[10] = {
    WR7_5,
    WR15,
    WR30,
    WR60,
    WR120,
    WR240,
    WR480,
    WR960,
    WR1920,
    WR3840
  };
  byte* Gb[12] = {
    &configuration.x1_Gia,
    &configuration.x2_Gia,
    &configuration.x3_Gia,
    &configuration.x4_Gia,
    &configuration.y1_Gia,
    &configuration.y2_Gia,
    &configuration.y3_Gia,
    &configuration.y4_Gia,
    &configuration.z1_Gia,
    &configuration.z2_Gia,
    &configuration.z3_Gia,
    &configuration.z4_Gia
  };
  byte* Ga[12] = {
    &configuration.x1_Gadc,
    &configuration.x2_Gadc,
    &configuration.x3_Gadc,
    &configuration.x4_Gadc,
    &configuration.y1_Gadc,
    &configuration.y2_Gadc,
    &configuration.y3_Gadc,
    &configuration.y4_Gadc,
    &configuration.z1_Gadc,
    &configuration.z2_Gadc,
    &configuration.z3_Gadc,
    &configuration.z4_Gadc
  };
  Setups* stps[12] = {
    &configuration.x1_stps,
    &configuration.x2_stps,
    &configuration.x3_stps,
    &configuration.x4_stps,
    &configuration.y1_stps,
    &configuration.y2_stps,
    &configuration.y3_stps,
    &configuration.y4_stps,
    &configuration.z1_stps,
    &configuration.z2_stps,
    &configuration.z3_stps,
    &configuration.z4_stps
  };
  reg regs[8] = {
    Setup1,
    Setup2,
    Setup3,
    Setup4,
    Setup5,
    Setup6,
    Setup7,
    Setup8
  };
  Setup* stp[12] = {
    &(sc[0]->adc.setup1),
    &(sc[1]->adc.setup1),
    &(sc[2]->adc.setup1),
    &(sc[3]->adc.setup1),
    &(sc[4]->adc.setup1),
    &(sc[5]->adc.setup1),
    &(sc[6]->adc.setup1),
    &(sc[7]->adc.setup1),
    &(sc[8]->adc.setup1),
    &(sc[9]->adc.setup1),
    &(sc[10]->adc.setup1),
    &(sc[11]->adc.setup1)
  };
  char* names[9] = {
    sc[0]->adc.setup1.name,
    sc[0]->adc.setup2.name,
    sc[0]->adc.setup3.name,
    sc[0]->adc.setup4.name,
    sc[0]->adc.setup5.name,
    sc[0]->adc.setup6.name,
    sc[0]->adc.setup7.name,
    sc[0]->adc.setup8.name,
    "Done"
  };
  String dirs[3] = {
    "X:",
    "Y:",
    "Z:"
  };
  lcd.command(lcd.row1[0]);
  lcd.print(axes[0]);
  lcd.command(lcd.row2[0]);
  lcd.print(axes[1]);
  lcd.command(lcd.row3[0]);
  lcd.print(axes[2]);
  lcd.command(lcd.row1[0]);
  lcd.print(">");
  while(1){
    if(scroll){
      if(dSel){
        if(rSel){
          if(aSel){
            val = val + dir;
            if(val<0){
              val = 0;
            }
            if(val>bnds[attr]){
              val = bnds[attr];
            }
            lcd.setCursor(0,0);
            switch(row3){
              case 0:
                lcd.command(lcd.row1[8]);
                break;
              case 1:
                lcd.command(lcd.row2[8]);
                break;
              case 2:
                lcd.command(lcd.row3[8]);
                break;
            }
            switch(attr){
              case 0:
                lcd.print(pgiaG[val]);
                for(int i=0;i<4;++i){
                  sc[(ax*4)+i]->setPGIA(val);
                  stp[(ax*4)+i]->pgiaG=val;
                  stps[(ax*4)+i]->gi[rgst] = val;
                  *Gb[(ax*4)+i]=val;
                }
                break;
              case 1:
                lcd.print(adcG[val]);
                for(int i=0;i<4;++i){
                  stp[(ax*4)+i]->G = adcGb[val];
                  sc[(ax*4)+i]->adc.writeReg(regs[rgst],0);
                  stps[(ax*4)+i]->out[rgst] = stp[(ax*4)+i]->out;
                  stps[(ax*4)+i]->ga[rgst] = val;
                  *Ga[(ax*4)+i]=val;
                }
                break;
              case 2:
                lcd.print(fs[val]);
                for(int i=0;i<12;++i){
                  stp[i]->WR = fsb[val];
                  sc[i]->adc.writeReg(regs[rgst],0);
                  stps[i]->out[rgst] = stp[i]->out;
                  stps[i]->fs[rgst] = val;
                }
                configuration.Fs_i = val;
                break;
            }
            updateFlash();
            lcd.setCursor(0,1);
            switch(row3){
              case 0:
                lcd.command(lcd.row1[6]);
                break;
              case 1:
                lcd.command(lcd.row2[6]);
                break;
              case 2:
                lcd.command(lcd.row3[6]);
                break;
            }
          }
          else{
            attr = attr + dir;
            row3 = row3 + dir;
            if(attr<0){
              attr = 0;
            }
            if(attr>3){
              attr = 3;
            }
            if(row3<0){
              row3 = 0;
            }
            if(row3>2){
              row3 = 2;
            }
            t3 = attr - row3;
            m3 = t3 + 1;
            b3 = t3 + 2;
            String vals[4] = {
              pgiaG[stp[ax*4]->pgiaG],
              adcG[stp[ax*4]->stpVals[2]],
              fs[stp[ax*4]->stpVals[3]],
              " "
            };
            String dirs[3] = {
              "X:",
              "Y:",
              "Z:"
            };
            lcd.clear();
            lcd.command(lcd.row0[0]);
            lcd.print(dirs[ax]+names[rgst]);
            lcd.command(lcd.row1[0]);
            lcd.print(attrs[t3]+vals[t3]);
            lcd.command(lcd.row2[0]);
            lcd.print(attrs[m3]+vals[m3]);
            lcd.command(lcd.row3[0]);
            lcd.print(attrs[b3]+vals[b3]);
            switch(row3){
              case 0:
                lcd.command(lcd.row1[0]);
                lcd.print(">");
                break;
              case 1:
                lcd.command(lcd.row2[0]);
                lcd.print(">");
                break;
              case 2:
                lcd.command(lcd.row3[0]);
                lcd.print(">");
                break;
            }
          }
        }
        else{
          rgst = rgst + dir;
          row2 = row2 + dir;
          if(rgst<0){
            rgst = 0;
          }
          if(rgst>8){
            rgst = 8;
          }
          if(row2<0){
            row2 = 0;
          }
          if(row2>2){
            row2 = 2;
          }
          t2 = rgst - row2;
          m2 = t2 + 1;
          b2 = t2 + 2;
          lcd.clear();
          lcd.command(lcd.row0[6]);
          lcd.print(axes[ax]);
          lcd.command(lcd.row1[1]);
          lcd.print(names[t2]);
          lcd.command(lcd.row2[1]);
          lcd.print(names[m2]);
          lcd.command(lcd.row3[1]);
          lcd.print(names[b2]);
          switch(row2){
            case 0:
              lcd.command(lcd.row1[0]);
              lcd.print(">");
              break;
            case 1:
              lcd.command(lcd.row2[0]);
              lcd.print(">");
              break;
            case 2:
              lcd.command(lcd.row3[0]);
              lcd.print(">");
              break;
          }
        }
      }
      else{
        ax = ax + dir;
        row1 = row1 + dir;
        if(ax<0){
          ax = 0;
        }
        if(ax>3){
          ax = 3;
        }
        if(row1<0){
          row1 = 0;
        }
        if(row1>2){
          row1 = 2;
        }
        t1 = ax - row1;
        m1 = t1 + 1;
        b1 = t1 + 2;
        lcd.clear();
        lcd.command(lcd.row0[0]);
        lcd.print(caption);
        lcd.command(lcd.row1[0]);
        lcd.print(axes[t1]);
        lcd.command(lcd.row2[0]);
        lcd.print(axes[m1]);
        lcd.command(lcd.row3[0]);
        lcd.print(axes[b1]);
        switch(row1){
          case 0:
            lcd.command(lcd.row1[0]);
            lcd.print(">");
            break;
          case 1:
            lcd.command(lcd.row2[0]);
            lcd.print(">");
            break;
          case 2:
            lcd.command(lcd.row3[0]);
            lcd.print(">");
            break;
        }
      }
      scroll = false;
    }
    if(btnF){
      if(dSel){
        if(rSel){
          if(aSel){
            aSel = false;
            lcd.setCursor(0,0);
            switch(row3){
              case 0:
                lcd.command(lcd.row1[0]);
                lcd.print(">");
                break;
              case 1:
                lcd.command(lcd.row2[0]);
                lcd.print(">");
                break;
              case 2:
                lcd.command(lcd.row3[0]);
                lcd.print(">");
                break;
            }
          }
          else{
            if(attr==3){
              rSel = false;
              aSel = false;
              attr = 0;
              row3 = 0;
              t3 = 0;
              m3 = 1;
              b3 = 2;
              lcd.clear();
              lcd.command(lcd.row0[6]);
              lcd.print(axes[ax]);
              lcd.command(lcd.row1[1]);
              lcd.print(names[t2]);
              lcd.command(lcd.row2[1]);
              lcd.print(names[m2]);
              lcd.command(lcd.row3[1]);
              lcd.print(names[b2]);
              switch(row2){
                case 0:
                  lcd.command(lcd.row1[0]);
                  lcd.print(">");
                  break;
                case 1:
                  lcd.command(lcd.row2[0]);
                  lcd.print(">");
                  break;
                case 2:
                  lcd.command(lcd.row3[0]);
                  lcd.print(">");
                  break;
              }
            }
            else{
              aSel = true;
              switch(attr){
                case 0:
                  val = stp[ax*4]->pgiaG;
                  break;
                case 1:
                  val = stp[ax*4]->stpVals[2];
                  break;
                case 2:
                  val = stp[ax*4]->stpVals[3];
                  break;
              }
              if(!row3){
                lcd.command(lcd.row1[0]);
                lcd.print(attrs[t3]);
                lcd.command(lcd.row1[6]);
              }
              else if(row3==1){
                lcd.command(lcd.row2[0]);
                lcd.print(attrs[m3]);
                lcd.command(lcd.row2[6]);
              }
              else{
                lcd.command(lcd.row3[0]);
                lcd.print(attrs[b3]);
                lcd.command(lcd.row3[6]);
              }
              lcd.setCursor(0,1);
            }
          }
        }
        else{
          if(rgst==8){
            aSel = false;
            rSel = false;
            dSel = false;
            rgst = 0;
            t2 = 0;
            m2 = 1;
            b2 = 2;
            row2 = 0;
            lcd.clear();
            lcd.command(lcd.row0[0]);
            lcd.print(caption);
            lcd.command(lcd.row1[0]);
            lcd.print(axes[t1]);
            lcd.command(lcd.row2[0]);
            lcd.print(axes[m1]);
            lcd.command(lcd.row3[0]);
            lcd.print(axes[b1]);
            switch(row1){
              case 0:
                lcd.command(lcd.row1[0]);
                lcd.print(">");
                break;
              case 1:
                lcd.command(lcd.row2[0]);
                lcd.print(">");
                break;
              case 2:
                lcd.command(lcd.row3[0]);
                lcd.print(">");
                break;
            }
          }
          else{
            rSel = true;
            for(int i=0;i<12;++i){
              switch(rgst){
                case 0:
                  stp[i] = &(sc[i]->adc.setup1);
                  break;
                case 1:
                  stp[i] = &(sc[i]->adc.setup2);
                  break;
                case 2:
                  stp[i] = &(sc[i]->adc.setup3);
                  break;
                case 3:
                  stp[i] = &(sc[i]->adc.setup4);
                  break;
                case 4:
                  stp[i] = &(sc[i]->adc.setup5);
                  break;
                case 5:
                  stp[i] = &(sc[i]->adc.setup6);
                  break;
                case 6:
                  stp[i] = &(sc[i]->adc.setup7);
                  break;
                case 7:
                  stp[i] = &(sc[i]->adc.setup8);
                  break;
              }
            }
            for(int i=0;i<4;++i){
              sc[(ax*4)+i]->setPGIA(stp[ax*4]->pgiaG);
              stp[(ax*4)+i]->pgiaG = stp[ax*4]->pgiaG;
              stp[(ax*4)+i]->G = stp[ax*4]->G;
              sc[(ax*4)+i]->adc.writeReg(regs[rgst],0);
            }
            attr = 0;
            lcd.clear();
            lcd.command(lcd.row0[0]);
            lcd.print(dirs[ax]+names[rgst]);
            lcd.command(lcd.row1[0]);
            lcd.print(attrs[0]+pgiaG[stp[ax*4]->pgiaG]);
            lcd.command(lcd.row2[0]);
            lcd.print(attrs[1]+adcG[stp[ax*4]->stpVals[2]]);
            lcd.command(lcd.row3[0]);
            lcd.print(attrs[2]+fs[stp[ax*4]->stpVals[3]]);
            lcd.command(lcd.row1[0]);
            lcd.print(">");
          }
        }
      }
      else{
        if(ax==3){
          ipage = main;
          btnF = false;
          scroll = false;
          lcd.clear();
          lcd.command(lcd.row1[0]);
          lcd.print("  Saving Boards...");
          delay(1500);
          //updateFlash();
          return ipage;
        }
        else{
          dSel = true;
          rgst = 0;
          names[0] = sc[ax*4]->adc.setup1.name;
          names[1] = sc[ax*4]->adc.setup2.name;
          names[2] = sc[ax*4]->adc.setup3.name;
          names[3] = sc[ax*4]->adc.setup4.name;
          names[4] = sc[ax*4]->adc.setup5.name;
          names[5] = sc[ax*4]->adc.setup6.name;
          names[6] = sc[ax*4]->adc.setup7.name;
          names[7] = sc[ax*4]->adc.setup8.name;
          lcd.clear();
          lcd.command(lcd.row0[6]);
          lcd.print(axes[ax]);
          lcd.command(lcd.row1[1]);
          lcd.print(names[0]);
          lcd.command(lcd.row2[1]);
          lcd.print(names[1]);
          lcd.command(lcd.row3[1]);
          lcd.print(names[2]);
          lcd.command(lcd.row1[0]);
          lcd.print(">");
        }
      }
      btnF = false;
    }
  }
}

pages psetADCs(){
  caption = "   Configure ADCs";
  if(verboseFSM){
    Serial.println(caption);
  }
  lcd.command(lcd.row0[0]);
  lcd.print(caption);
  byte cur = 0;
  int brd = 0;
  int rgst = 0;
  int attr = 0;
  reg regs[] = {Config,
                Setup1,
                Setup2,
                Setup3,
                Setup4,
                Setup5,
                Setup6,
                Setup7,
                Setup8};
  String brds[13] = {"X1",
                     "X2",
                     "X3",
                     "X4",
                     "Y1",
                     "Y2",
                     "Y3",
                     "Y4",
                     "Z1",
                     "Z2",
                     "Z3",
                     "Z4",
                     "Done"};
  String rgsts[] = {"Config",
                    "Setup1",
                    "Setup2",
                    "Setup3",
                    "Setup4",
                    "Setup5",
                    "Setup6",
                    "Setup7",
                    "Setup8",
                    "<- Done"};
  int stpEls[5] = {
    3,
    3,
    6,
    9,
    1
  };
  String stpCS[4] = {
    "     Channel 1      ",
    "     Channel 2      ",
    " Channel 3 - Locked ",
    " Channel 4 - Locked "
  };
  int stpCSb[4] = {
    C1,
    C2,
    C3,
    C4
  };
  String stpCAL[4] = {
    "     Channel 1      ",
    "     Channel 2      ",
    " Channel 3 - Locked ",
    " Channel 4 - Locked "
  };
  int stpCALb[4] = {
    OG_1,
    OG_2,
    OG_3,
    OG_4
  };
  String stpGAIN[7] = {
    "       G = 1 ",
    "       G = 2 ",
    "       G = 4 ",
    "       G = 8 ",
    "       G = 16",
    "       G = 32",
    "       G = 64"
  };
  int stpGAINb[7] = {
    G1,
    G2,
    G4,
    G8,
    G16,
    G32,
    G64
  };
  String stpWR[10] = {
    "    Fs = 7.5 Sps ",
    "    Fs = 15 Sps  ",
    "    Fs = 30 Sps  ",
    "    Fs = 60 Sps  ",
    "    Fs = 120 Sps ",
    "    Fs = 240 Sps ",
    "    Fs = 480 Sps ",
    "    Fs = 960 Sps ",
    "    Fs = 1920 Sps",
    "    Fs = 3840 Sps"
  };
  int stpWRb[10] = {
    WR7_5,
    WR15,
    WR30,
    WR60,
    WR120,
    WR240,
    WR480,
    WR960,
    WR1920,
    WR3840
  };
  String stpUB[2] = {
    "       Bipolar",
    "      Unipolar"
  };
  int stpUBb[2] = {
    UB_BI,
    UB_UNI
  };
  String cfgVRS[2] = {
    "2.5V<Vref<=VA+ - VA-",
    "1 V < Vref <= 2.5 V"
  };
  int cfgVRSb[2] = {
    VRS_HIGH,
    VRS_LOW
  };
  String cfgSRS[2] = {
    "Invalid System Reset",
    " Valid System Reset"
  };
  int cfgSRSb[2] = {
    RV_NORM,
    RV_VALID
  };
  String cfgOGS[2] = {
    "   Set by CS Bits",
    "   Set by OG Bits"
  };
  int cfgOGSb[2] = {
    OGS_CS,
    OGS_OG
  };
  String cfgPSS[2] = {
    "    Standby Mode",
    "     Sleep Mode"
  };
  int cfgPSSb[2] = {
    PSS_STB,
    PSS_SLP
  };
  String cfgPDW[2] = {
    "    Normal Mode",
    "   Low Power Mode"
  };
  int cfgPDWb[2] = {
    PDW_NORM,
    PDW_SAVE
  };
  String cfgStgs[5] = {
    " ",
    " ",
    " ",
    " ",
    " "
  };
  String stpStgs[5] = {
    " ",
    " ",
    " ",
    " ",
    " "
  };
  String attrsConf[] = {" Voltage Ref Select",
                        "    Reset Valid",
                        " Offset/Gain Select",
                        " Power Save Select",
                        "     Power Mode",
                        "      <- Done"};
  String attrsCSR[] = {"   Channel Select",
                       "     Cal Source",
                       "        Gain",
                       "     Word Rate",
                       "      Polarity",
                       "      <- Done"};
  SGSC* sc[12] = {&sc0,
                  &sc1,
                  &sc2,
                  &sc3,
                  &sc4,
                  &sc5,
                  &sc6,
                  &sc7,
                  &sc8,
                  &sc9,
                  &sc10,
                  &sc11};
  byte* Ga[12] = {
    &configuration.x1_Gadc,
    &configuration.x2_Gadc,
    &configuration.x3_Gadc,
    &configuration.x4_Gadc,
    &configuration.y1_Gadc,
    &configuration.y2_Gadc,
    &configuration.y3_Gadc,
    &configuration.y4_Gadc,
    &configuration.z1_Gadc,
    &configuration.z2_Gadc,
    &configuration.z3_Gadc,
    &configuration.z4_Gadc
  };
  Setups* stps[12] = {
    &configuration.x1_stps,
    &configuration.x2_stps,
    &configuration.x3_stps,
    &configuration.x4_stps,
    &configuration.y1_stps,
    &configuration.y2_stps,
    &configuration.y3_stps,
    &configuration.y4_stps,
    &configuration.z1_stps,
    &configuration.z2_stps,
    &configuration.z3_stps,
    &configuration.z4_stps
  };
  ConfReg cfg = sc[0]->adc.confReg;
  Setup* stp = &(sc[0]->adc.setup4);
  btnF = false;
  scroll = false;
  bool bSel = false;
  bool rSel = false;
  bool aSel = false;
  lcd.command(lcd.row2[0]);
  lcd.print("      Board: "+brds[brd]);
  lcd.command(lcd.row3[0]);
  lcd.print(">Select         Save");
  while(1){
    if(scroll){
      if(cur==0){
        cur = 2;
        lcd.command(lcd.row3[0]);
        lcd.print(" Select        >Save");
      }
      else if(cur==1){
        if(bSel){
          if(rSel){
            if(aSel){
              if(rgst==0){
                cfg.cfgVals[attr] = cfg.cfgVals[attr] + dir;
                if(cfg.cfgVals[attr]<0){
                  cfg.cfgVals[attr]=0;
                }
                if(cfg.cfgVals[attr]>1){
                  cfg.cfgVals[attr]=1;
                }
                switch(attr){
                  case 0:
                    cfg.VRS = cfgVRSb[cfg.cfgVals[attr]];
                    cfgStgs[0] = cfgVRS[cfg.cfgVals[0]];
                    break;
                  case 1:
                    //cfg.SRS = cfgSRSb[cfg.cfgVals[attr]];
                    //cfgStgs[1] = cfgSRS[cfg.cfgVals[1]];
                    break;
                  case 2:
                    cfg.OGS = cfgVRSb[cfg.cfgVals[attr]];
                    cfgStgs[2] = cfgOGS[cfg.cfgVals[2]];
                    break;
                  case 3:
                    cfg.PSS = cfgPSSb[cfg.cfgVals[attr]];
                    cfgStgs[3] = cfgPSS[cfg.cfgVals[3]];
                    break;
                  case 4:
                    cfg.PDW = cfgPDWb[cfg.cfgVals[attr]];
                    cfgStgs[4] = cfgPDW[cfg.cfgVals[4]];
                    break;
                }
                lcd.command(lcd.row3[0]);
                lcd.print(cfgStgs[attr]);
              }
              else{
                stp->stpVals[attr] = stp->stpVals[attr] + dir;
                if(stp->stpVals[attr]<0){
                  stp->stpVals[attr]=0;
                }
                if(stp->stpVals[attr]>stpEls[attr]){
                  stp->stpVals[attr]=stpEls[attr];
                }
                switch(attr){
                  case 0:
                    stp->CS = stpCSb[stp->stpVals[attr]];
                    stpStgs[0] = stpCS[stp->stpVals[0]];
                    if(cfg.OGS==OGS_CS){
                      stpStgs[1]=stpStgs[0];
                    }
                    break;
                  case 1:
                    if(cfg.OGS==OGS_OG){
                      stp->OG = stpCALb[stp->stpVals[attr]];
                      stpStgs[1] = stpCAL[stp->stpVals[1]];
                    }
                    else{
                      if(stpStgs[1]!=stpStgs[0]){
                        stpStgs[1]=stpStgs[0];
                      }
                      else{
                        stpStgs[1] = "Ignored:Cfg->OGS->CS";
                      }
                    }
                    break;
                  case 2:
                    stp->G = stpGAINb[stp->stpVals[attr]];
                    stpStgs[2] = stpGAIN[stp->stpVals[2]];
                    break;
                  case 3:
                    stp->WR = stpWRb[stp->stpVals[attr]];
                    stpStgs[3] = stpWR[stp->stpVals[3]];
                    break;
                  case 4:
                    stp->UB = stpUBb[stp->stpVals[attr]];
                    stpStgs[4] = stpUB[stp->stpVals[4]];
                    break;
                }
                lcd.command(lcd.row3[0]);
                lcd.print(stpStgs[attr]);
              }
            }
            else{
              attr = attr + dir;
              if(attr>5){
                attr=5;
              }
              if(attr<0){
                attr=0;
              }
              lcd.clear();
              lcd.command(lcd.row0[0]);
              lcd.print("B:"+brds[brd]+",R:"+rgsts[rgst]);
              lcd.command(lcd.row1[0]);
              lcd.print("  Select Attribute");
              lcd.command(lcd.row2[0]);
              if(rgst==0){
                lcd.print(attrsConf[attr]);
                if(attr<5){
                  lcd.command(lcd.row3[0]);
                  lcd.print(cfgStgs[attr]);
                }
              }
              else if(rgst<9){
                lcd.print(attrsCSR[attr]);
                if(attr<5){
                  lcd.command(lcd.row3[0]);
                  lcd.print(stpStgs[attr]);
                }
              }
              else{
                lcd.print("      <- Done");
              }
            }
          }
          else{
            rgst = rgst + dir;
            if(rgst>9){
              rgst=9;
            }
            if(rgst<0){
              rgst=0;
            }
            lcd.clear();
            lcd.command(lcd.row0[0]);
            lcd.print("B:"+brds[brd]);
            lcd.command(lcd.row1[0]);
            lcd.print("  Select Register");
            lcd.command(lcd.row2[7]);
            lcd.print(rgsts[rgst]);
            lcd.command(lcd.row3[0]);
            switch(rgst){
              case 1:
                lcd.print(sc[brd]->adc.setup1.name);
                break;
              case 2:
                lcd.print(sc[brd]->adc.setup2.name);
                break;
              case 3:
                lcd.print(sc[brd]->adc.setup3.name);
                break;
              case 4:
                lcd.print(sc[brd]->adc.setup4.name);
                break;
              case 5:
                lcd.print(sc[brd]->adc.setup5.name);
                break;
              case 6:
                lcd.print(sc[brd]->adc.setup6.name);
                break;
              case 7:
                lcd.print(sc[brd]->adc.setup7.name);
                break;
              case 8:
                lcd.print(sc[brd]->adc.setup8.name);
                break;
            }
          }
        }
        else{
          brd = brd + dir;
          if(brd<0){
            brd = 0;
          }
          if(brd>12){
            brd=12;
          }
          lcd.clear();
          lcd.command(lcd.row1[0]);
          lcd.print("    Select Board");
          lcd.command(lcd.row2[0]);
          lcd.print("      Board: "+brds[brd]);
        }
      }
      else{
        cur = 0;
        lcd.command(lcd.row3[0]);
        lcd.print(">Select         Save");
      }
      scroll = false;
    }
    if(btnF){
      if(cur==0){
        cur = 1;
        lcd.clear();
        lcd.command(lcd.row1[0]);
        lcd.print("    Select Board");
        lcd.command(lcd.row2[0]);
        lcd.print("      Board: "+brds[brd]);
      }
      else if(cur==1){
        if(brd==12){
          cur = 0;
          lcd.clear();
          lcd.command(lcd.row0[0]);
          lcd.print(caption);
          lcd.command(lcd.row2[0]);
          lcd.print("      Board: "+brds[brd]);
          lcd.command(lcd.row3[0]);
          lcd.print(">Select         Save");
        }
        else{
          if(bSel){
            if(rgst==9){
              bSel = false;
              rgst = 0;
              lcd.clear();
              lcd.command(lcd.row1[0]);
              lcd.print("    Select Board");
              lcd.command(lcd.row2[0]);
              lcd.print("      Board: "+brds[brd]);
            }
            else{
              if(rSel){
                if(attr==5){
                  rSel = false;
                  attr = 0;
                  lcd.clear();
                  lcd.command(lcd.row0[0]);
                  lcd.print("B:"+brds[brd]);
                  lcd.command(lcd.row1[0]);
                  lcd.print("  Select Register");
                  lcd.command(lcd.row2[7]);
                  lcd.print(rgsts[rgst]);
                  lcd.command(lcd.row3[0]);
                  
                  if(!rgst){
                    sc[brd]->adc.confReg = cfg;
                    sc[brd]->adc.writeReg(Config,0);
                    cfg = sc[brd]->adc.confReg;
                  }
                  else{
                    sc[brd]->adc.writeReg(regs[rgst],0);
                    lcd.print(stp->name);
                    stps[brd]->name[rgst-1] = stp->name;
                    stps[brd]->out[rgst-1] = stp->out;
                    stps[brd]->ga[rgst-1] = stp->stpVals[2];
                    stps[brd]->fs[rgst-1] = stp->stpVals[3];
                    if(rgst==1){
                      *Ga[brd] = stp->stpVals[2];
                      if(!brd){
                        configuration.Fs_i = stp->stpVals[3];
                      }
                    }
                  }
                  updateFlash();
                }
                else{
                  if(aSel){
                    aSel = false;
                    lcd.clear();
                    lcd.command(lcd.row0[0]);
                    lcd.print("B:"+brds[brd]+",R:"+rgsts[rgst]);
                    lcd.command(lcd.row1[0]);
                    lcd.print("  Select Attribute");
                    lcd.command(lcd.row2[0]);
                    if(rgst==0){
                      lcd.print(attrsConf[attr]);
                      lcd.command(lcd.row3[0]);
                      lcd.print(cfgStgs[attr]);
                    }
                    else{
                      lcd.print(attrsCSR[attr]);
                      lcd.command(lcd.row3[0]);
                      lcd.print(stpStgs[attr]);
                    }
                  }
                  else{
                    aSel = true;
                    lcd.clear();
                    lcd.command(lcd.row0[0]);
                    lcd.print("B:"+brds[brd]+",R:"+rgsts[rgst]);
                    lcd.command(lcd.row2[0]);
                    lcd.command(lcd.row2[0]);
                    if(rgst==0){
                      lcd.print(attrsConf[attr]);
                      lcd.command(lcd.row3[0]);
                      lcd.print(cfgStgs[attr]);
                    }
                    else{
                      lcd.print(attrsCSR[attr]);
                      lcd.command(lcd.row3[0]);
                      lcd.print(stpStgs[attr]);
                    }
                  }
                }
              }
              else{
                rSel = true;
                lcd.clear();
                lcd.command(lcd.row0[0]);
                lcd.print("B:"+brds[brd]+",R:"+rgsts[rgst]);
                lcd.command(lcd.row1[0]);
                lcd.print("  Select Attribute");
                lcd.command(lcd.row2[0]);
                if(rgst==0){
                  lcd.print(attrsConf[attr]);
                  lcd.command(lcd.row3[0]);
                  lcd.print(cfgStgs[attr]);
                }
                else{
                  switch(rgst){
                    case 1:
                      stp = &(sc[brd]->adc.setup1);
                      break;
                    case 2:
                      stp = &(sc[brd]->adc.setup2);
                      break;
                    case 3:
                      stp = &(sc[brd]->adc.setup3);
                      break;
                    case 4:
                      stp = &(sc[brd]->adc.setup4);
                      break;
                    case 5:
                      stp = &(sc[brd]->adc.setup5);
                      break;
                    case 6:
                      stp = &(sc[brd]->adc.setup6);
                      break;
                    case 7:
                      stp = &(sc[brd]->adc.setup7);
                      break;
                    case 8:
                      stp = &(sc[brd]->adc.setup8);
                      break;
                  }
                  stpStgs[0] = stpCS[stp->stpVals[0]];
                  stpStgs[1] = stpCAL[stp->stpVals[1]];
                  stpStgs[2] = stpGAIN[stp->stpVals[2]];
                  stpStgs[3] = stpWR[stp->stpVals[3]];
                  stpStgs[4] = stpUB[stp->stpVals[4]];
                  
                  lcd.print(attrsCSR[attr]);
                  lcd.command(lcd.row3[0]);
                  lcd.print(stpStgs[attr]);
                }
              }
            }
          }
          else{
            bSel = true;
            for(int i=0;i<9;++i){
              sc[brd]->adc.readReg(regs[i]);
            }
            cfg = sc[brd]->adc.confReg;
            cfgStgs[0] = cfgVRS[cfg.cfgVals[0]];
            cfgStgs[1] = cfgSRS[cfg.cfgVals[1]];
            cfgStgs[2] = cfgOGS[cfg.cfgVals[2]];
            cfgStgs[3] = cfgPSS[cfg.cfgVals[3]];
            cfgStgs[4] = cfgPDW[cfg.cfgVals[4]];
            
            lcd.clear();
            lcd.command(lcd.row0[0]);
            lcd.print("B:"+brds[brd]);
            lcd.command(lcd.row1[0]);
            lcd.print("  Select Register");
            lcd.command(lcd.row2[7]);
            lcd.print(rgsts[rgst]);
          }
        }
      }
      else if(cur==2){
        ipage = main;
        btnF = false;
        scroll = false;
        lcd.clear();
        lcd.command(lcd.row1[0]);
        lcd.print("  Saving Boards...");
        delay(1500);
        //updateFlash();
        return ipage;
      }
      btnF = false;
    }
  }
}

pages pmotor(){
  // 
  caption = "Setup Motor";
  if(verboseFSM){
    Serial.println(caption);
  }
  num_blurbs = 6;
  String blurbs[] = {
    "Back",
    "Enable",
    "Direction",
    "Speed"
  };
  return ipage;
}

pages pgains(){
  // 
  caption = " Adjust PGIAs";
  if(verboseFSM){
    Serial.println(caption);
  }
  num_blurbs = 3;
  lcd.writeRow(caption,0);
  String blurbs[] = {
    "Back",
    "Check PGIA Gains",
    "Set PGIA Gains",
  };
  pages states[] = {
    main,
    checkIAs,
    setIAs
  };
  ipage = menu(num_blurbs, blurbs, states);
  return ipage;
}

pages pcalibrate(){
  // Menu
  caption = "Calibration";
  if(verboseFSM){
    Serial.println(caption);
  }
  lcd.command(lcd.row0[4]);
  lcd.print(caption);
  num_blurbs = 5;
  String blurbs[] = {
    "Back",
    "Zero-Offset",
    "Set Full Scale",
    "Strain Sensitivity",
    "Frequency Content"
  };
  pages states[] = {
    main,
    offsetCal,
    setFullScale,
    tuneConversion,
    freqCal
  };
  ipage = menu(num_blurbs, blurbs, states);
  return ipage;
}

pages poffsetCal(){
  caption = "Zero-Offset Cal";
  lcd.makeExtraChars();
  if(verboseFSM){
    Serial.println(caption);
  }
  lcd.command(lcd.row0[2]);
  lcd.print(caption);
  scroll = false;
  btnF = false;
  byte cur = 0;
  int opt = 0;
  SGSC* sc[12] = {
    &sc0,
    &sc1,
    &sc2,
    &sc3,
    &sc4,
    &sc5,
    &sc6,
    &sc7,
    &sc8,
    &sc9,
    &sc10,
    &sc11
  };
  syncBridges(r2sg);
  syncBridges(r3sg);
  for(int i=0;i<12;++i){
    sc[i]->setPGIA(sc[i]->adc.setup2.pgiaG);
  }
  lcd.command(lcd.row2[7]);
  lcd.print("Ready?");
  lcd.command(lcd.row3[0]);
  lcd.print(">Start          Back");
  while(1){
    if(scroll){
      if(cur==1){
        if(opt){
          opt = 0;
          lcd.command(lcd.row2[0]);
          lcd.print(">");
          lcd.command(lcd.row3[0]);
          lcd.print(" ");
        }
        else{
          opt = 1;
          lcd.command(lcd.row2[0]);
          lcd.print(" ");
          lcd.command(lcd.row3[0]);
          lcd.print(">");
        }
      }
      else if(!cur){
        cur = 2;
        lcd.command(lcd.row3[0]);
        lcd.print(" Start         >Back");
      }
      else{
        cur = 0;
        lcd.command(lcd.row3[0]);
        lcd.print(">Start          Back");
      }
      scroll = false;
    }
    if(btnF){
      if(!cur){
        cur = 3;
        lcd.clear();
        lcd.command(lcd.row0[0]);
        lcd.print("Please remove tread-");
        lcd.command(lcd.row1[0]);
        lcd.print("mill. Press knob");
        lcd.command(lcd.row2[0]);
        lcd.print("when complete.");
        lcd.command(lcd.row3[10]);
        lcd.print("Continue ");
        lcd.writeData(lcd.arrowR);
        btnF = false;
        while(!btnF){}
        delay(300);
        
        lcd.clear();
        lcd.command(lcd.row0[0]);
        lcd.print("Carefully place mass");
        lcd.command(lcd.row1[0]);
        lcd.print("insert on arms and");
        lcd.command(lcd.row2[0]);
        lcd.print("add 200g masses.");
        lcd.command(lcd.row3[10]);
        lcd.print("Continue ");
        lcd.writeData(lcd.arrowR);
        btnF = false;
        while(!btnF){}
        delay(300);
      }
      else if(cur==3){
        btnF = false;
        setOFF();
        
        btnF = false;
        scroll = false;
        opt = 0;
        cur = 1;
      }
      else if(cur==1){
        cur = 3;
        if(!opt){
          cur = 0;
          lcd.clear();
          lcd.command(lcd.row1[0]);
          lcd.print(" Storing Offsets...");
          delay(1500);
          
          lcd.clear();
          lcd.command(lcd.row0[2]);
          lcd.print(caption);
          lcd.command(lcd.row2[7]);
          lcd.print("Ready?");
          lcd.command(lcd.row3[0]);
          lcd.print(">Start          Back");
          btnF = false;
          scroll = false;
        }
      }
      else{
        ipage = calibrate;
        btnF = false;
        scroll = false;
        return ipage;
      }
    }
  }
}

bool setOFF(){
  uint32_t ocals[50][12];
  uint64_t ocalsM[12];
  uint32_t omin[12];
  uint32_t omax[12];
  uint32_t orng[12];
  float ocalsmF[12];
  float aclMeas[50][3];
  float aclM[3] = {0,0,0};
  float aclU[3] = {0,0,0};
  float pitch = 0.0;
  float roll = 0.0;
  float mag = 0.0;
  float* uAs[4] = {
    configuration.uO,
    configuration.uGx,
    configuration.uGy,
    configuration.uGz
  };
  float* pr[2] = {
    configuration.pitchCals,
    configuration.rollCals
  };
  SGSC* sc[12] = {
    &sc0,
    &sc1,
    &sc2,
    &sc3,
    &sc4,
    &sc5,
    &sc6,
    &sc7,
    &sc8,
    &sc9,
    &sc10,
    &sc11
  };
  uint32_t* cfgCals[12] = {
    &configuration.x1Ocal,
    &configuration.x2Ocal,
    &configuration.x3Ocal,
    &configuration.x4Ocal,
    &configuration.y1Ocal,
    &configuration.y2Ocal,
    &configuration.y3Ocal,
    &configuration.y4Ocal,
    &configuration.z1Ocal,
    &configuration.z2Ocal,
    &configuration.z3Ocal,
    &configuration.z4Ocal
  };
  for(int i=0;i<12;++i){
    sc[i]->setPGIA(sc[i]->adc.setup2.pgiaG);
    sc[i]->adc.uploadGain(Gain1,16777216,0);
    sc[i]->adc.readReg(Gain1);
    if(verboseFSM){
      Serial.println(sc[i]->adc.gainReg1.out);
    }
  }
  
  lcd.clear();
  lcd.command(lcd.row0[0]);
  lcd.print("Set pitch to 0");
  lcd.writeData(lcd.degree);
  lcd.command(lcd.row1[0]);
  lcd.print(" and roll to 0");
  lcd.writeData(lcd.degree);
  lcd.command(lcd.row2[0]);
  lcd.print("P:       , R:       ");
  lcd.command(lcd.row3[10]);
  lcd.print("Continue ");
  lcd.writeData(lcd.arrowR);
  btnF = false;
  while(!btnF){
    imu.read();
    sensors_event_t a, m, g, temp;
    imu.getEvent(&a, &m, &g, &temp);
    aclM[0] = a.acceleration.x;
    aclM[1] = a.acceleration.y;
    aclM[2] = a.acceleration.z;
    mag = sqrt(pow(aclM[0],2)+pow(aclM[1],2)+pow(aclM[2],2));
    aclU[0] = aclM[0]/mag;
    aclU[1] = aclM[1]/mag;
    aclU[2] = aclM[2]/mag;
    pitch = (asin(aclU[0])/PI)*180.0;
    roll = (atan(aclU[1]/aclU[2])/PI)*180.0;

    lcd.command(lcd.row2[2]);
    if(pitch>=0){
      lcd.print(" ");
    }
    lcd.print(String(pitch,2));   //P:-12.34o, R:-56.78o
    lcd.writeData(lcd.degree);
    lcd.writeData(' ');
    lcd.command(lcd.row2[12]);
    if(roll>=0){
      lcd.print(" ");
    }
    lcd.print(String(roll,2));
    lcd.writeData(lcd.degree);
    delay(100);
  }
  aclM[0] = 0;
  aclM[1] = 0;
  aclM[2] = 0;
  delay(300);
  lcd.clear();
  btnF = false;

  syncADCs = true;
  sdoF = false;
  verboseRR = false;
  verboseFSM = true;
  lcd.command(lcd.row1[0]);
  lcd.print("   Calibrating...");
  lcd.command(lcd.row2[0]);
  lcd.print("    Do not touch");
  delay(1500);

  for(int i=0;i<50;++i){
    imu.read();
    sensors_event_t a, m, g, temp;
    imu.getEvent(&a, &m, &g, &temp);
    aclMeas[i][0] = a.acceleration.x;
    aclMeas[i][1] = a.acceleration.y;
    aclMeas[i][2] = a.acceleration.z;
    aclM[0] = aclM[0] + aclMeas[i][0];
    aclM[1] = aclM[1] + aclMeas[i][1];
    aclM[2] = aclM[2] + aclMeas[i][2];
    
    sdoF = false;
    sc0.adc.sysOffCal(Setup2,1);
    for(int j=0;j<12;++j){
      sc[j]->adc.readReg(Offset1);
      ocals[i][j] = sc[j]->adc.offsetReg1.out;
    }
  }

  aclM[0] = aclM[0]/50.0;
  aclM[1] = aclM[1]/50.0;
  aclM[2] = aclM[2]/50.0;
  mag = sqrt(pow(aclM[0],2)+pow(aclM[1],2)+pow(aclM[2],2));
  aclU[0] = aclM[0]/mag;
  aclU[1] = aclM[1]/mag;
  aclU[2] = aclM[2]/mag;
  pitch = (asin(aclU[0])/PI)*180.0;
  roll = (atan(aclU[1]/aclU[2])/PI)*180.0;
  *(uAs[0]+0) = aclU[0];
  *(uAs[0]+1) = aclU[1];
  *(uAs[0]+2) = aclU[2];
  *(pr[0]) = pitch;
  *(pr[1]) = roll;

  verboseFSM = true;
  accelTog = false;

  for(int i=0;i<12;++i){
    if(verboseFSM){
      Serial.println("\n\t\t\t--------"+sc[i]->adc.name+":"+String(i)+"--------");
    }
    ocalsM[i] = 0;
    omin[i] = ocals[0][i];
    omax[i] = ocals[0][i];
    for(int j=0;j<50;++j){
      ocalsM[i] = ocalsM[i]+ocals[j][i];
      if(ocals[j][i]<omin[i]){
        omin[i] = ocals[j][i];
      }
      if(ocals[j][i]>omax[i]){
        omax[i] = ocals[j][i];
      }
      if(verboseFSM){
        Serial.print(ocals[j][i]);
        ocalsmF[i]=0.0;
        for(int k=0;k<31;++k){
            ocalsmF[i] = ocalsmF[i]+((1&(ocals[j][i]>>k))*(pow(2.0,k-32)));
        }
        ocalsmF[i] = ocalsmF[i] - (1&(ocals[j][i]>>31))*0.5;
        Serial.print("\t(");
        Serial.print(ocalsmF[i],6);
        Serial.print(")");
        if(accelTog){
          mag = sqrt(pow(aclMeas[i][0],2)+pow(aclMeas[i][1],2)+pow(aclMeas[i][2],2));
          aclU[0] = aclMeas[j][0]/mag;
          aclU[1] = aclMeas[j][1]/mag;
          aclU[2] = aclMeas[j][2]/mag;
          pitch = (asin(aclU[0])/PI)*180.0;
          roll = (atan(aclU[1]/aclU[2])/PI)*180.0;
          Serial.print(" , ");
          Serial.print(aclMeas[j][0],4);
          Serial.print(" , ");
          Serial.print(aclMeas[j][1],4);
          Serial.print(" , ");
          Serial.print(aclMeas[j][2],4);
          Serial.print("\t->\t");
          Serial.print(aclU[0],6);
          Serial.print(" , ");
          Serial.print(aclU[1],6);
          Serial.print(" , ");
          Serial.print(aclU[2],6);
          Serial.print("\t->\t");
          Serial.print("Pitch: ");
          Serial.print(pitch,4);
          Serial.print(" , Roll: ");
          Serial.println(roll,4);
        }
        else{
          Serial.print("\n");
        }
      }
    }
    ocalsM[i] = ocalsM[i]/50.0;
    orng[i] = omax[i] - omin[i];
    if(verboseFSM){
      Serial.print("Means: ");
      Serial.print((uint32_t)ocalsM[i]);
    }
    ocalsmF[i]=0.0;
    for(int k=0;k<31;++k){
        ocalsmF[i] = ocalsmF[i]+((1&(ocalsM[i]>>k))*(pow(2.0,k-32)));
    }
    ocalsmF[i] = ocalsmF[i] - (1&(ocalsM[i]>>31))*0.5;
    if(verboseFSM){
      Serial.print(" (");
      Serial.print(ocalsmF[i],8);
      Serial.println(")");
      Serial.print(omin[i]);
      Serial.print(", ");
      Serial.print(omax[i]);
      Serial.print(", ");
      Serial.print(orng[i]);
      if(orng[i]==0){
        Serial.println("\tERROR!! REDO");
      }
      else{
        Serial.print("\n");
      }
    }
    sc[i]->adc.uploadOffset(Offset1,(uint32_t)ocalsM[i],0);
    *cfgCals[i] = (uint32_t)ocalsM[i];
    //verboseRR = true;
    sc[i]->adc.readReg(Offset1);
    *cfgCals[i] = sc[i]->adc.offsetReg1.out;
    //verboseRR = false;
    if(verboseFSM){
      Serial.print("\nOffset1 Reg: ");
      Serial.println(sc[i]->adc.offsetReg1.out);
    }
  }
  updateFlash();

  if(verboseFSM){
    delay(200);
    lcd.clear();
    lcd.command(lcd.row1[0]);
    lcd.print("     Testing...");
    syncADCs = true;
    sc0.adc.contMeas(Setup1,1);
    sdoF = false;
    for(int i=0;i<100;++i){
      for(int j=0;j<12;++j){
        uint32_t m0 = sc[j]->adc.scanMeas();
        m0 = 0xFFFFFF&(m0>>8);
        int m2 = (int)m0;
        m2 = m2 - (int)((1&(m0>>23))*pow(2,24));
        Serial.print(m2);
        Serial.print(", ");
      }
      sdoF = false;
      Serial.print("\n");
    }
    Serial.println("\n---Summary---");
    for(int i=0;i<12;++i){
      sc[i]->adc.stopMeas(0);
    }
    for(int i=0;i<12;++i){
      Serial.print(sc[i]->name+": ");
      Serial.print(*cfgCals[i]);
      Serial.print("\t(");
      Serial.print(ocalsmF[i],8);
      Serial.println(")");
    }
    delay(1500);
  }
  
  lcd.clear();
  lcd.command(lcd.row0[0]);
  lcd.print("Calibration Complete");
  lcd.command(lcd.row1[0]);
  lcd.print("P: "+String(pitch,2));
  lcd.writeData(lcd.degree);
  lcd.print(",R: "+String(roll,2));
  lcd.writeData(lcd.degree);
  lcd.command(lcd.row2[0]);
  lcd.print(">Store Results");
  lcd.command(lcd.row3[1]);
  lcd.print("Start Over");
  scroll = false;
  btnF = false;
}

pages psetFullScale(){
  caption = "Full-Scale Cal";
  lcd.makeExtraChars();
  
  verboseFSM = true;
  
  if(verboseFSM){
    Serial.println(caption);
  }
  lcd.command(lcd.row0[3]);
  lcd.print(caption);
  
  String axes[4] = {
    "X",
    "Y",
    "Z",
    "Done"
  };
  SGSC* sc[12] = {
    &sc0,
    &sc1,
    &sc2,
    &sc3,
    &sc4,
    &sc5,
    &sc6,
    &sc7,
    &sc8,
    &sc9,
    &sc10,
    &sc11
  };
  uint32_t* cfgCals[12] = {
    &configuration.x1Gcal,
    &configuration.x2Gcal,
    &configuration.x3Gcal,
    &configuration.x4Gcal,
    &configuration.y1Gcal,
    &configuration.y2Gcal,
    &configuration.y3Gcal,
    &configuration.y4Gcal,
    &configuration.z1Gcal,
    &configuration.z2Gcal,
    &configuration.z3Gcal,
    &configuration.z4Gcal
  };
  for(int i=0;i<12;++i){
    sc[i]->setPGIA(sc[i]->adc.setup3.pgiaG);
  }
  scroll = false;
  btnF = false;
  syncADCs = true;
  byte cur = 0;
  int ax = 0;
  int opt = 0;
  int row1 = 0;
  int t1 = 0;
  int m1 = 1;
  int b1 = 2;
  syncBridges(r2sg);
  syncBridges(r3sg);
  lcd.command(lcd.row2[7]);
  lcd.print("Ready?");
  lcd.command(lcd.row3[0]);
  lcd.print(">Start          Back");
  
  while(1){
    if(scroll){
      if(!cur){
        cur = 1;
        lcd.command(lcd.row3[0]);
        lcd.print(" Start         >Back");
      }
      else if(cur==1){
        cur = 0;
        lcd.command(lcd.row3[0]);
        lcd.print(">Start          Back");
      }
      else if(cur==2){
        ax = ax + dir;
        row1 = row1 + dir;
        if(ax<0){
          ax = 0;
        }
        if(ax>3){
          ax=3;
        }
        if(row1<0){
          row1 = 0;
        }
        if(row1>2){
          row1=2;
        }
        t1 = ax-row1;
        m1 = t1 + 1;
        b1 = t1 + 2;
        lcd.clear();
        lcd.command(lcd.row0[0]);
        lcd.print("    Select Axis");
        lcd.command(lcd.row1[1]);
        lcd.print(axes[t1]);
        lcd.command(lcd.row2[1]);
        lcd.print(axes[m1]);
        lcd.command(lcd.row3[1]);
        lcd.print(axes[b1]);
        switch(row1){
          case 0:
            lcd.command(lcd.row1[0]);
            break;
          case 1:
            lcd.command(lcd.row2[0]);
            break;
          case 2:
            lcd.command(lcd.row3[0]);
            break;
        }
        lcd.print(">");
      }
      else{
        if(opt){
          opt = 0;
          lcd.command(lcd.row2[0]);
          lcd.print(">");
          lcd.command(lcd.row3[0]);
          lcd.print(" ");
        }
        else{
          opt = 1;
          lcd.command(lcd.row2[0]);
          lcd.print(" ");
          lcd.command(lcd.row3[0]);
          lcd.print(">");
        }
      }
      scroll = false;
    }
    if(btnF){
      if(!cur){
        cur = 2;
        
        lcd.clear();
        lcd.command(lcd.row0[0]);
        lcd.print("Please remove tread-");
        lcd.command(lcd.row1[0]);
        lcd.print("mill. Press knob");
        lcd.command(lcd.row2[0]);
        lcd.print("when complete.");
        lcd.command(lcd.row3[10]);
        lcd.print("Continue ");
        lcd.writeData(lcd.arrowR);
        btnF = false;
        while(!btnF){}
        delay(300);
        
        lcd.clear();
        lcd.command(lcd.row0[0]);
        lcd.print("Carefully place mass");
        lcd.command(lcd.row1[0]);
        lcd.print("insert on arms and");
        lcd.command(lcd.row2[0]);
        lcd.print("add 500g mass.");
        lcd.command(lcd.row3[10]);
        lcd.print("Continue ");
        lcd.writeData(lcd.arrowR);
        btnF = false;
        while(!btnF){}
        delay(300);
        btnF = false;
        
        lcd.clear();
        lcd.command(lcd.row0[0]);
        lcd.print("    Select Axis");
        lcd.command(lcd.row1[1]);
        lcd.print(axes[t1]);
        lcd.command(lcd.row2[1]);
        lcd.print(axes[m1]);
        lcd.command(lcd.row3[1]);
        lcd.print(axes[b1]);
        switch(row1){
          case 0:
            lcd.command(lcd.row1[0]);
            break;
          case 1:
            lcd.command(lcd.row2[0]);
            break;
          case 2:
            lcd.command(lcd.row3[0]);
            break;
        }
        lcd.print(">");
      }
      else if(cur==1){
        ipage = calibrate;
        btnF = false;
        scroll = false;
        return ipage;
      }
      else if(cur==2){
        if(ax<3){
          setVFS(ax);
          cur = 3;
          btnF = false;
          scroll = false;
          opt = 0;
        }
        else{
          lcd.clear();
          lcd.command(lcd.row1[0]);
          lcd.print("  Uploading Saved");
          lcd.command(lcd.row2[0]);
          lcd.print("  Gains to ADCs...");
          for(int i=0;i<12;++i){
            sc[i]->adc.uploadGain(Gain1,*cfgCals[i],0);
          }
          delay(1500);

          if(verboseFSM){
            delay(200);
            lcd.clear();
            lcd.command(lcd.row1[0]);
            lcd.print("     Testing...");
            syncADCs = true;
            sc0.adc.contMeas(Setup1,1);
            sdoF = false;
            for(int i=0;i<100;++i){
              for(int j=0;j<12;++j){
                uint32_t m0 = sc[j]->adc.scanMeas();
                m0 = 0xFFFFFF&(m0>>8);
                int m2 = (int)m0;
                m2 = m2 - (int)((1&(m0>>23))*pow(2,24));
                Serial.print(m2);
                Serial.print(", ");
              }
              sdoF = false;
              Serial.print("\n");
            }
            Serial.println("\n---Summary---");
            for(int i=0;i<12;++i){
              sc[i]->adc.stopMeas(0);
            }
            for(int i=0;i<12;++i){
              Serial.print(sc[i]->name+": ");
              Serial.print(*cfgCals[i]);
              float gcalF=0.0;
              for(int k=0;k<30;++k){
                  gcalF = gcalF+((1&(*cfgCals[i]>>k))*(pow(2.0,k-24)));
              }
              Serial.print("\t(");
              Serial.print(gcalF,8);
              Serial.println(")");
            }
          }
          delay(1500);
          
          lcd.clear();
          lcd.command(lcd.row0[3]);
          lcd.print(caption);
          lcd.command(lcd.row2[7]);
          lcd.print("Ready?");
          lcd.command(lcd.row3[0]);
          lcd.print(">Start          Back");
          cur = 0;
          btnF = false;
          scroll = false;
        }
      }
      else{
        if(opt){
          cur = 2;
          btnF = true;
          scroll = false;
        }
        else{
          cur = 2;
          btnF = false;
          scroll = true;
        }
      }
    }
  }
}

bool setVFS(int ax){
  int opt = 0;
  uint32_t gcals[50][12];
  uint64_t gcalsM[12];
  uint32_t gmin[12];
  uint32_t gmax[12];
  uint32_t grng[12];
  float gcalsmF[12];
  float aclMeas[50][3];
  float aclM[3] = {0,0,0};
  float aclU[3] = {0,0,0};
  float pitch = 0.0;
  float roll = 0.0;
  float mag = 0.0;
  float* uAs[4] = {
    configuration.uO,
    configuration.uGx,
    configuration.uGy,
    configuration.uGz
  };
  float* pr[2] = {
    configuration.pitchCals,
    configuration.rollCals
  };
  SGSC* sc[12] = {
    &sc0,
    &sc1,
    &sc2,
    &sc3,
    &sc4,
    &sc5,
    &sc6,
    &sc7,
    &sc8,
    &sc9,
    &sc10,
    &sc11
  };
  uint32_t* cfgCals[12] = {
    &configuration.x1Gcal,
    &configuration.x2Gcal,
    &configuration.x3Gcal,
    &configuration.x4Gcal,
    &configuration.y1Gcal,
    &configuration.y2Gcal,
    &configuration.y3Gcal,
    &configuration.y4Gcal,
    &configuration.z1Gcal,
    &configuration.z2Gcal,
    &configuration.z3Gcal,
    &configuration.z4Gcal
  };
  int angs[6] = {5, 0, 0, 5, 0, 0};
  for(int i=0;i<12;++i){
    sc[i]->setPGIA(sc[i]->adc.setup3.pgiaG);
    Serial.println(sc[i]->adc.offsetReg1.out);
  }
  
  lcd.clear();
  lcd.command(lcd.row0[0]);
  lcd.print("Set pitch to "+String(angs[2*ax]));
  lcd.writeData(lcd.degree);
  lcd.command(lcd.row1[0]);
  lcd.print(" and roll to "+String(angs[(2*ax)+1]));
  lcd.writeData(lcd.degree);
  lcd.command(lcd.row2[0]);
  lcd.print("P:       , R:       ");
  lcd.command(lcd.row3[10]);
  lcd.print("Continue ");
  lcd.writeData(lcd.arrowR);
  btnF = false;
  while(!btnF){
    imu.read();
    sensors_event_t a, m, g, temp;
    imu.getEvent(&a, &m, &g, &temp);
    aclM[0] = a.acceleration.x;
    aclM[1] = a.acceleration.y;
    aclM[2] = a.acceleration.z;
    mag = sqrt(pow(aclM[0],2)+pow(aclM[1],2)+pow(aclM[2],2));
    aclU[0] = aclM[0]/mag;
    aclU[1] = aclM[1]/mag;
    aclU[2] = aclM[2]/mag;
    pitch = asin(aclU[0])*(180.0/PI);
    roll = atan(aclU[1]/aclU[2])*(180.0/PI);

    lcd.command(lcd.row2[2]);
    if(pitch>=0){
      lcd.print(" ");
    }
    lcd.print(String(pitch,2));   //P:-12.34o, R:-56.78o
    lcd.writeData(lcd.degree);
    lcd.writeData(' ');
    lcd.command(lcd.row2[12]);
    if(roll>=0){
      lcd.print(" ");
    }
    lcd.print(String(roll,2));
    lcd.writeData(lcd.degree);
    delay(50);
  }
  aclM[0] = 0;
  aclM[1] = 0;
  aclM[2] = 0;
  delay(300);
  lcd.clear();
  btnF = false;

  syncADCs = true;
  sdoF = false;
  verboseRR = false;
  verboseFSM = true;
  lcd.command(lcd.row1[0]);
  lcd.print("   Calibrating...");
  lcd.command(lcd.row2[0]);
  lcd.print("    Do not touch");
  delay(1500);

  for(int i=0;i<50;++i){
    imu.read();
    sensors_event_t a, m, g, temp;
    imu.getEvent(&a, &m, &g, &temp);
    aclMeas[i][0] = a.acceleration.x;
    aclMeas[i][1] = a.acceleration.y;
    aclMeas[i][2] = a.acceleration.z;
    aclM[0] = aclM[0] + aclMeas[i][0];
    aclM[1] = aclM[1] + aclMeas[i][1];
    aclM[2] = aclM[2] + aclMeas[i][2];
    
    sdoF = false;
    sc0.adc.sysGainCal(Setup3,1);
    for(int j=0;j<12;++j){
      sc[j]->adc.readReg(Gain1);
      gcals[i][j] = sc[j]->adc.gainReg1.out;
    }
  }

  aclM[0] = aclM[0]/50.0;
  aclM[1] = aclM[1]/50.0;
  aclM[2] = aclM[2]/50.0;
  mag = sqrt(pow(aclM[0],2)+pow(aclM[1],2)+pow(aclM[2],2));
  aclU[0] = aclM[0]/mag;
  aclU[1] = aclM[1]/mag;
  aclU[2] = aclM[2]/mag;
  pitch = (asin(aclU[0])/PI)*180.0;
  roll = (atan(aclU[1]/aclU[2])/PI)*180.0;
  *(uAs[ax+1]+0) = aclU[0];
  *(uAs[ax+1]+1) = aclU[1];
  *(uAs[ax+1]+2) = aclU[2];
  *(pr[0]+ax+1) = pitch;
  *(pr[1]+ax+1) = roll;

  verboseFSM = true;
  accelTog = false;

  for(int i=ax*4;i<(ax+1)*4;++i){
    if(verboseFSM){
      Serial.println("\n\t\t\t--------"+sc[i]->adc.name+":"+String(i)+"--------");
    }
    gcalsM[i] = 0;
    gmin[i] = gcals[0][i];
    gmax[i] = gcals[0][i];
    for(int j=0;j<50;++j){
      gcalsM[i] = gcalsM[i]+gcals[j][i];
      if(gcals[j][i]<gmin[i]){
        gmin[i] = gcals[j][i];
      }
      if(gcals[j][i]>gmax[i]){
        gmax[i] = gcals[j][i];
      }
      if(verboseFSM){
        Serial.print(gcals[j][i]);
        gcalsmF[i]=0.0;
        for(int k=0;k<30;++k){
            gcalsmF[i] = gcalsmF[i]+((1&(gcals[j][i]>>k))*(pow(2.0,k-24)));
        }
        Serial.print("\t(");
        Serial.print(gcalsmF[i],6);
        Serial.print(")");
        if(accelTog){
          mag = sqrt(pow(aclMeas[i][0],2)+pow(aclMeas[i][1],2)+pow(aclMeas[i][2],2));
          aclU[0] = aclMeas[j][0]/mag;
          aclU[1] = aclMeas[j][1]/mag;
          aclU[2] = aclMeas[j][2]/mag;
          pitch = (asin(aclU[0])/PI)*180.0;
          roll = (atan(aclU[1]/aclU[2])/PI)*180.0;
          Serial.print(" , ");
          Serial.print(aclMeas[j][0],4);
          Serial.print(" , ");
          Serial.print(aclMeas[j][1],4);
          Serial.print(" , ");
          Serial.print(aclMeas[j][2],4);
          Serial.print("\t->\t");
          Serial.print(aclU[0],6);
          Serial.print(" , ");
          Serial.print(aclU[1],6);
          Serial.print(" , ");
          Serial.print(aclU[2],6);
          Serial.print("\t->\t");
          Serial.print("Pitch: ");
          Serial.print(pitch,4);
          Serial.print(" , Roll: ");
          Serial.println(roll,4);
        }
        else{
          Serial.print("\n");
        }
      }
    }
    gcalsM[i] = gcalsM[i]/50.0;
    grng[i] = gmax[i] - gmin[i];
    if(verboseFSM){
      Serial.print("Means: ");
      Serial.print((uint32_t)gcalsM[i]);
    }
    gcalsmF[i]=0.0;
    for(int k=0;k<30;++k){
        gcalsmF[i] = gcalsmF[i]+((1&(gcalsM[i]>>k))*(pow(2.0,k-24)));
    }
    if(verboseFSM){
      Serial.print(" (");
      Serial.print(gcalsmF[i],8);
      Serial.println(")");
      Serial.print(gmin[i]);
      Serial.print(", ");
      Serial.print(gmax[i]);
      Serial.print(", ");
      Serial.print(grng[i]);
      if(grng[i]==0){
        Serial.println("\tERROR!! REDO");
      }
      else{
        Serial.print("\n");
      }
    }
    
    sc[i]->adc.uploadGain(Gain1,(uint32_t)gcalsM[i],0);
    *cfgCals[i] = (uint32_t)gcalsM[i];
    //verboseRR = true;
    sc[i]->adc.readReg(Gain1);
    *cfgCals[i] = sc[i]->adc.gainReg1.out;
    //verboseRR = false;
    if(verboseFSM){
      Serial.print("\nGain1 Reg: ");
      Serial.println(sc[i]->adc.gainReg1.out);
    }
  }
  updateFlash();

  if(verboseFSM){
    delay(200);
    lcd.clear();
    lcd.command(lcd.row1[0]);
    lcd.print("     Testing...");
    delay(1500);
    syncADCs = true;
    sc0.adc.contMeas(Setup1,1);
    sdoF = false;
    for(int i=0;i<100;++i){
      for(int j=0;j<12;++j){
        uint32_t m0 = sc[j]->adc.scanMeas();
        m0 = 0xFFFFFF&(m0>>8);
        int m2 = (int)m0;
        m2 = m2 - (int)((1&(m0>>23))*pow(2,24));
        Serial.print(m2);
        Serial.print(", ");
      }
      sdoF = false;
      Serial.print("\n");
    }
    Serial.println("\n---Summary---");
    for(int i=0;i<12;++i){
      sc[i]->adc.stopMeas(0);
    }
    for(int i=0;i<12;++i){
      Serial.print(sc[i]->name+": ");
      Serial.print(*cfgCals[i]);
      Serial.print("\t(");
      Serial.print(gcalsmF[i],8);
      Serial.println(")");
    }
    delay(1500);
  }
  
  lcd.clear();
  lcd.command(lcd.row0[0]);
  lcd.print("Calibration Complete");
  lcd.command(lcd.row1[0]);
  lcd.print("P: "+String(pitch,2));
  lcd.writeData(lcd.degree);
  lcd.print(",R: "+String(roll,2));
  lcd.writeData(lcd.degree);
  lcd.command(lcd.row2[0]);
  lcd.print(">Store Results");
  lcd.command(lcd.row3[1]);
  lcd.print("Start Over");
  scroll = false;
  btnF = false;
}

pages ptuneConversion(){
  
}

pages pfreqCal(){
  
}

pages psettings(){
  // Menu
  caption = "Settings";
  if(verboseFSM){
    Serial.println(caption);
  }
  num_blurbs = 6;
  String blurbs[] = {
    "Back",
    "Sampling Rate",
    "Communications",
    "LCD Brightness",
    "LCD Contrast",
    "Buzzer"
  };
  pages states[] = {
    main,
    setFs,
    setComms,
    setBrightness,
    setContrast,
    setBuzzer
  };
  ipage = menu(num_blurbs, blurbs, states);
  return ipage;
}

pages psetFs(){
  caption = " Set Sampling Rate";
  String FsVals[10] = {
    " 7.5",
    "  15",
    "  30",
    "  60",
    " 120",
    " 240",
    " 480",
    " 960",
    "1920",
    "3840"
  };
  if(verboseFSM){
    Serial.println(caption);
  }
  lcd.writeRow(caption,0);
  int Fs = configuration.Fs_i;
  lcd.command(lcd.row1[6]);
  lcd.print(FsVals[Fs]+" Sps");
  if(Fs>0){
    lcd.command(lcd.row1[0]);
    lcd.print("<<");
  }
  if(Fs<9){
    lcd.command(lcd.row1[18]);
    lcd.print(">>");
  }
  scroll = false;
  btnF = false;
  lcd.command(lcd.row3[0]);
  lcd.print(">Set            Back");
  byte cur = 0;
  while(1){
    if(scroll){
      if(cur==0){
        cur = 2;
        lcd.command(lcd.row3[0]);
        lcd.print(" ");
        lcd.command(lcd.row3[15]);
        lcd.print(">");
      }
      else if(cur==1){
        Fs = Fs + dir;
        if(Fs<0){
          Fs = 0;
        }
        else if(Fs>9){
          Fs = 9;
        }
        lcd.command(lcd.row1[6]);
        lcd.print(FsVals[Fs]+" Sps");
        if(Fs>0){
          lcd.command(lcd.row1[0]);
          lcd.print("<<");
        }
        else{
          lcd.command(lcd.row1[0]);
          lcd.print("  ");
        }
        if(Fs<9){
          lcd.command(lcd.row1[18]);
          lcd.print(">>");
        }
        else{
          lcd.command(lcd.row1[18]);
          lcd.print("  ");
        }
      }
      else{
        cur = 0;
        lcd.command(lcd.row3[0]);
        lcd.print(">");
        lcd.command(lcd.row3[15]);
        lcd.print(" ");
      }
      scroll = false;
    }
    if(btnF){
      if(cur==0){
        cur = 1;
        lcd.command(lcd.row3[0]);
        lcd.print(" ");
      }
      else if(cur==1){
        cur = 0;
        lcd.command(lcd.row3[0]);
        lcd.print(">");
      }
      else{
        ipage = settings;
        configuration.Fs_i = Fs;
        updateFlash();
        return ipage;
      }
      btnF = false;
    }
  }
}

pages psetComms(){
  
}

pages psetBrightness(){
  caption = "Set LCD Brightness";
  if(verboseFSM){
    Serial.println(caption);
  }
  lcd.writeRow(caption,0);
  lcd.makeEmojis();
  lcd.command(lcd.row3[0]);
  lcd.print(">Set            Back");
  lcd.command(lcd.row1[6]);
  int lightPrev = light;
  byte cur = 0;
  for(int i=0;i<light;++i){
    lcd.writeChar(0x06,1,6+i);
  }
  for(int i=light;i<7;++i){
    lcd.writeChar(0x07,1,6+i);
  }
  lcd.command(lcd.row2[9]);
  lcd.print(String(light));
  scroll = false;
  btnF = false;
  while(1){
    if(scroll){
      if(cur==0){
        cur = 2;
        lcd.command(lcd.row3[0]);
        lcd.print(" ");
        lcd.command(lcd.row3[15]);
        lcd.print(">");
      }
      else if(cur==1){
        light = light + dir;
        if(light<3){
          light = 3;
        }
        else if(light>7){
          light = 7;
        }
        for(int i=0;i<light;++i){
          lcd.writeChar(0x06,1,6+i);
        }
        for(int i=light;i<7;++i){
          lcd.writeChar(0x07,1,6+i);
        }
        lcd.command(lcd.row2[9]);
        lcd.print(String(light));
        lcd.command(lcd.row2[9]);
        Serial.println(light);
        if(light==lightPrev){
          lcd.setCursor(0,0);
        }
        else{
          lcd.setCursor(0,1);
        }
      }
      else{
        cur = 0;
        lcd.command(lcd.row3[0]);
        lcd.print(">");
        lcd.command(lcd.row3[15]);
        lcd.print(" ");
      }
      scroll = false;
    }
    if(btnF){
      if(cur==0){
        cur = 1;
        lcd.command(lcd.row3[0]);
        lcd.print(" ");
      }
      else if(cur==1){
        lcd.setCursor(0,0);
        if(light==lightPrev){
          cur = 0;
          lcd.command(lcd.row3[0]);
          lcd.print(">");
        }
        else{
          lcd.backlight(light);
          lightPrev = light;
        }
      }
      else{
        ipage = settings;
        lcd.makeExtraChars();
        configuration.lcdB = light;
        updateFlash();
        return ipage;
      }
      btnF = false;
    }
  }
}

pages psetContrast(){
  
}

pages psetBuzzer(){
  
}

pages plock(){
  caption = "ERROR!! PLEASE RESET";
  Serial.println(caption);
  lcd.writeRow(caption,0);
  return ipage;
}

pages menu(int num_blurbs, String blurbs[], pages states[]){
  len = caption.length();
  if(len>20){
    scrollText = true;
    rendersA = len - 20;
    renderA = 0;
    lcd.scroll(caption,renderA,20,0,0);
    t1A = millis();
  }
  else if(len<20){
    bias = int(9 - ceil(len/2.0));
    if(bias<0){
      bias = 0;
    }
  }
  else{}
  if(page==main){
    lcd.writeChar(lcd.arrowR,1,19);
  }else{
    lcd.writeChar(lcd.backArrow,1,19);
  }
  lcd.writeChar(lcd.arrowR,2,19);
  lcd.writeChar(lcd.arrowR,3,19);
  
  while(1){
    if(scroll){
      sel = knob(num_blurbs, blurbs);
      if(scrollText){
        lcd.scroll(caption,renderA,20,0,0);
      }else{
        if(len==20){
          lcd.home();
          lcd.print(caption);
        }else{
          lcd.writeChar(' ',0,bias);
          lcd.print(caption);
        }
      }
      t1B = millis();
      renderB = 0;
      rendersB = blurbs[sel].length() - 17;
      if(frame==0){
        if(page==main){
          lcd.writeChar(lcd.arrowR,1,19);
        }else{
          lcd.writeChar(lcd.backArrow,1,19);
        }
      }
      else{
        lcd.writeChar(lcd.arrowR,1,19);
      }
      if(screens>0){
        if(frame<screens){
          lcd.writeChar(lcd.menuDown,3,18);
        }
        if(frame>0){
          lcd.writeChar(lcd.menuUp,1,18);
        }
      }
      lcd.writeChar(lcd.arrowR,2,19);
      lcd.writeChar(lcd.arrowR,3,19);
    }
    if(scrollText){
      t2A = millis();
      dtA = t2A - t1A;
      if(renderA==0){
        if(dtA>3*tScroll){
          ++renderA;
          lcd.scroll(caption,renderA,20,0,0);
          t1A = millis();
        }
      }
      else if(renderA == rendersA){
        if(dtA>3*tScroll){
          renderA = 0;
          lcd.scroll(caption,renderA,20,0,0);
          t1A = millis();
        }
      }
      else{
        if(dtA>tScroll){
          ++renderA;
          lcd.scroll(caption,renderA,20,0,0);
          t1A = millis();
        }
      }
    }
    else if(len==20){
      lcd.home();
      lcd.print(caption);
    }
    else{
      lcd.writeChar(' ',0,bias);
      lcd.print(caption);
    }
    if(blurbs[sel].length()>17){
      t2B = millis();
      dtB = t2B - t1B;
      if(renderB==0){
        if(dtB>3*tScroll){
          ++renderB;
          lcd.scroll(blurbs[sel],renderB,17,row+1,1);
          t1B = millis();
        }
      }
      else if(renderB==rendersB){
        if(dtB>3*tScroll){
          renderB = 0;
          lcd.scroll(blurbs[sel],renderB,17,row+1,1);
          t1B = millis();
        }
      }else{
        if(dtB>tScroll){
          ++renderB;
          lcd.scroll(blurbs[sel],renderB,17,row+1,1);
          t1B = millis();
        }
      }
    }
    ipage = states[sel];
    if(btnF){
      delay(500);
      return ipage;
    }
  }
}

int knob(int options, String phrases[]){
  screens = options - rows;
  if(row==0){
    if(dir==1){
      row = 1;
    }
    else if(dir==-1){
      if(frame>0){
        frame = frame - 1;
      }
    }
  }
  else if(row==1){
    if(dir==-1){
      row = 0;
    }
    else if(dir==1){
      row = 2;
    }
  }
  else{
    if(dir==-1){
      row = 1;
    }
    else if(dir==1){
      if(frame<16){
        frame = frame + 1;
      }
    }
  }
  if(frame>screens){
    frame = screens;
  }
  if(row==0){
    ct = '>';
    cm = ' ';
    cb = ' ';
  }
  else if(row==1){
    ct = ' ';
    cm = '>';
    cb = ' ';
  }
  else{
    ct = ' ';
    cm = ' ';
    cb = '>';
  }
  ind = frame + row;
  topline = phrases[frame];
  midline = phrases[frame+1];
  botline = phrases[frame+2];
  if(frame_prev!=frame){
    lcd.clear();
  }
  frame_prev = frame;
  
  if(topline.length()>17){
    for(int i=0;i<16;++i){
      buf[i] = topline[i];
    }
    lcd.writeRow(buf,1);
  }else{
    lcd.writeRow(topline,1);
  }
  if(midline.length()>17){
    for(int i=0;i<16;++i){
      buf[i] = midline[i];
    }
    lcd.writeRow(buf,2);
  }else{
    lcd.writeRow(midline,2);
  }
  if(botline.length()>17){
    for(int i=0;i<16;++i){
      buf[i] = botline[i];
    }
    lcd.writeRow(buf,3);
  }else{
    lcd.writeRow(botline,3);
  }

  lcd.writeChar(ct,1,0);
  lcd.writeChar(cm,2,0);
  lcd.writeChar(cb,3,0);
  scroll = false;
  return ind;
}

int stop_go(){
  if(dir==1){
    ct = '>';
    cb = ' ';
    ind = 1;
  }
  else{
    ct = ' ';
    cb = '>';
    ind = 0;
  }
  scroll = false;
  lcd.writeChar(cb,3,0);
  lcd.writeChar(ct,3,11);

  return ind;
}

void btn(){
  if(!btnF){
    btnF = true;
  }
}

void encA(){
  enc_f = 'A';
  a_state = digitalRead(gui_a);
  b_state = digitalRead(gui_b);
  //if(!scroll){
    //a_state = !a_state;
  if(a_state == b_state){
    dir = -1;
  }
  else{
    dir = 1;
  }
  scroll = true;
  //}
}

void encB(){
  enc_f = 'B';
  a_state = digitalRead(gui_a);
  b_state = digitalRead(gui_b);
  //if(!scroll){
    //b_state = !b_state;
  if(a_state == b_state){
    dir = 1;
  }
  else{
    dir = -1;
  }
  scroll = true;
  //}
}

void bldcPulse(){
  if(bldc.enV){
    bldc.tc = bldc.tf;
    bldc.tf = micros();
    bldc.dtk = bldc.tf - bldc.tc;
    if((bldc.dtk<990)||(bldc.dtk>1005)){
      bldc.pk = bldc.pk + 1;
    }
    bldc.dtk = bldc.tf - bldc.ti;
    if(bldc.dtk>bldc.ts){
      //bldc.bip = true;
      bldc.p = bldc.pk;
      bldc.dt = bldc.dtk;
      bldc.ti = micros();
      bldc.bip = true;
    }
  }
}

void sdoF_isr(){
  if(!sdoF){
    if(syncADCs){
      sdoF = true;
    }
    else if(!syncADCs&&(adcMic==0)){
      //Serial.println("-- SDOF 0 Flipped --");
      sdoF = true;
    }
  }
}

void sdo1F_isr(){
  if((!syncADCs&&!sdoF)&&(adcMic==1)){
    //Serial.println("-- SDOF 1 Flipped --");
    sdoF = true;
  }
}

void sdo2F_isr(){
  if((!syncADCs&&!sdoF)&&(adcMic==2)){
    //Serial.println("-- SDOF 2 Flipped --");
    sdoF = true;
  }
}

void sdo3F_isr(){
  if((!syncADCs&&!sdoF)&&(adcMic==3)){
    //Serial.println("-- SDOF 3 Flipped --");
    sdoF = true;
  }
}

void sdo4F_isr(){
  if((!syncADCs&&!sdoF)&&(adcMic==4)){
    //Serial.println("-- SDOF 4 Flipped --");
    sdoF = true;
  }
}

void sdo5F_isr(){
  if((!syncADCs&&!sdoF)&&(adcMic==5)){
    //Serial.println("-- SDOF 5 Flipped --");
    sdoF = true;
  }
}

void sdo6F_isr(){
  if((!syncADCs&&!sdoF)&&(adcMic==6)){
    //Serial.println("-- SDOF 6 Flipped --");
    sdoF = true;
  }
}

void sdo7F_isr(){
  if((!syncADCs&&!sdoF)&&(adcMic==7)){
    //Serial.println("-- SDOF 7 Flipped --");
    sdoF = true;
  }
}

void sdo8F_isr(){
  if((!syncADCs&&!sdoF)&&(adcMic==8)){
    //Serial.println("-- SDOF 8 Flipped --");
    sdoF = true;
  }
}

void sdo9F_isr(){
  if((!syncADCs&&!sdoF)&&(adcMic==9)){
    //Serial.println("-- SDOF 9 Flipped --");
    sdoF = true;
  }
}

void sdo10F_isr(){
  if((!syncADCs&&!sdoF)&&(adcMic==10)){
    //Serial.println("-- SDOF 10 Flipped --");
    sdoF = true;
  }
}

void sdo11F_isr(){
  if((!syncADCs&&!sdoF)&&(adcMic==11)){
    //Serial.println("-- SDOF 11 Flipped --");
    sdoF = true;
  }
}

void initFlash(){
  uint8_t firstRun = dueFlashStorage.read(0);
  if(firstRun){
    if(verboseFSM){
      Serial.println("Allocating Flash Storage for Configuration");
    }
    configuration.x1Gcal = 16777216;
    configuration.x2Gcal = 16777216;
    configuration.x3Gcal = 16777216;
    configuration.x4Gcal = 16777216;
    configuration.y1Gcal = 16777216;
    configuration.y2Gcal = 16777216;
    configuration.y3Gcal = 16777216;
    configuration.y4Gcal = 16777216;
    configuration.z1Gcal = 16777216;
    configuration.z2Gcal = 16777216;
    configuration.z3Gcal = 16777216;
    configuration.z4Gcal = 16777216;
    configuration.x1Ocal = 0;
    configuration.x2Ocal = 0;
    configuration.x3Ocal = 0;
    configuration.x4Ocal = 0;
    configuration.y1Ocal = 0;
    configuration.y2Ocal = 0;
    configuration.y3Ocal = 0;
    configuration.y4Ocal = 0;
    configuration.z1Ocal = 0;
    configuration.z2Ocal = 0;
    configuration.z3Ocal = 0;
    configuration.z4Ocal = 0;
    configuration.x1_Gia = 0;
    configuration.x2_Gia = 0;
    configuration.x3_Gia = 0;
    configuration.x4_Gia = 0;
    configuration.y1_Gia = 0;
    configuration.y2_Gia = 0;
    configuration.y3_Gia = 0;
    configuration.y4_Gia = 0;
    configuration.z1_Gia = 0;
    configuration.z2_Gia = 0;
    configuration.z3_Gia = 0;
    configuration.z4_Gia = 0;
    configuration.x1_Gadc = 2;
    configuration.x2_Gadc = 2;
    configuration.x3_Gadc = 2;
    configuration.x4_Gadc = 2;
    configuration.y1_Gadc = 2;
    configuration.y2_Gadc = 2;
    configuration.y3_Gadc = 2;
    configuration.y4_Gadc = 2;
    configuration.z1_Gadc = 2;
    configuration.z2_Gadc = 2;
    configuration.z3_Gadc = 2;
    configuration.z4_Gadc = 2;
    configuration.Fs_i = 0;
    configuration.lcdB = 6;
    configuration.lcdC = 25;
    for(int i=0;i<3;++i){
      configuration.uO[i] = 0;
      configuration.uGx[i] = 0;
      configuration.uGy[i] = 0;
      configuration.uGz[i] = 0;
    }
    for(int i=0;i<4;++i){
      configuration.pitchCals[i] = 0;
      configuration.rollCals[i] = 0;
    }
    configuration.outSet.mSpeed = 0.0;
    configuration.outSet.mDir = true;
    configuration.outSet.mUnits = 0;
    configuration.outSet.convM = 1.0;
    configuration.outSet.bodLen = 3.5;
    
    configuration.outSet.togAccel = true;
    configuration.outSet.togBLDC = true;
    configuration.outSet.togTime = true;
    configuration.outSet.adcUnits = 2;
    configuration.outSet.convA = 1.0;
    for(int i=0;i<14;++i){
      configuration.outSet.togADC[i] = true;
    }
    updateFlash();
  }
  else{
    loadFlash();
    firstRodeo = false;
    light = configuration.lcdB;
    cont = configuration.lcdC;
  }
}

void updateFlash(){
  if(verboseFSM){
    Serial.println("Uploading Configuration to Flash");
  }
  byte b2[sizeof(Configuration)];
  memcpy(b2, &configuration, sizeof(Configuration));
  dueFlashStorage.write(4, b2, sizeof(Configuration));
  dueFlashStorage.write(0,0);
}

void loadFlash(){
  if(verboseFSM){
    Serial.println("Downloading Configuration from Flash");
  }
  byte* b2 = dueFlashStorage.readAddress(4);
  memcpy(&configuration, b2, sizeof(Configuration));
}

void setupADCs(){
  if(verboseFSM){
    Serial.println("\n\t----- Setup ADCs -----\n");
  }
  syncADCs = true;
  sdoF = false;
  sc0.init(0);
  sc1.init(0);
  sc2.init(0);
  sc3.init(0);
  sc4.init(0);
  sc5.init(0);
  sc6.init(0);
  sc7.init(0);
  sc8.init(0);
  sc9.init(0);
  sc10.init(0);
  sc11.init(0);
  
  //verboseFSM = true;
  sc0.adc.SPIS(1);
  sc0.adc.SRS(1);
  
  sc1.adc.readReg(Config);
  sc2.adc.readReg(Config);
  sc3.adc.readReg(Config);
  sc4.adc.readReg(Config);
  sc5.adc.readReg(Config);
  sc6.adc.readReg(Config);
  sc7.adc.readReg(Config);
  sc8.adc.readReg(Config);
  sc9.adc.readReg(Config);
  sc10.adc.readReg(Config);
  sc11.adc.readReg(Config);
  sc0.adc.readReg(Config);
  sc1.adc.readReg(Config);
  sc2.adc.readReg(Config);
  sc3.adc.readReg(Config);
  sc4.adc.readReg(Config);
  sc5.adc.readReg(Config);
  sc6.adc.readReg(Config);
  sc7.adc.readReg(Config);
  sc8.adc.readReg(Config);
  sc9.adc.readReg(Config);
  sc10.adc.readReg(Config);
  sc11.adc.readReg(Config);
  
  sc0.adc.VRSinit(0);
  sc1.adc.VRSinit(0);
  sc2.adc.VRSinit(0);
  sc3.adc.VRSinit(0);
  sc4.adc.VRSinit(0);
  sc5.adc.VRSinit(0);
  sc6.adc.VRSinit(0);
  sc7.adc.VRSinit(0);
  sc8.adc.VRSinit(0);
  sc9.adc.VRSinit(0);
  sc10.adc.VRSinit(0);
  sc11.adc.VRSinit(0);

  loadOCals();
  loadGCals();
  resetCSRs();
}

void resetCSRs(){
  if(firstRodeo){
    sc0.adc.CSRinit(0);
    sc1.adc.CSRinit(0);
    sc2.adc.CSRinit(0);
    sc3.adc.CSRinit(0);
    sc4.adc.CSRinit(0);
    sc5.adc.CSRinit(0);
    sc6.adc.CSRinit(0);
    sc7.adc.CSRinit(0);
    sc8.adc.CSRinit(0);
    sc9.adc.CSRinit(0);
    sc10.adc.CSRinit(0);
    sc11.adc.CSRinit(0);
    
    sc1.adc.follow();
    verboseRR = false;
    sc2.adc.follow();
    sc3.adc.follow();
    sc4.adc.follow();
    sc5.adc.follow();
    sc6.adc.follow();
    sc7.adc.follow();
    sc8.adc.follow();
    sc9.adc.follow();
    sc10.adc.follow();
    sc11.adc.follow();
    //verboseRR = true;
    SGSC* sc[12] = {&sc0,
                    &sc1,
                    &sc2,
                    &sc3,
                    &sc4,
                    &sc5,
                    &sc6,
                    &sc7,
                    &sc8,
                    &sc9,
                    &sc10,
                    &sc11};
    Setups* stps[12] = {&configuration.x1_stps,
                       &configuration.x2_stps,
                       &configuration.x3_stps,
                       &configuration.x4_stps,
                       &configuration.y1_stps,
                       &configuration.y2_stps,
                       &configuration.y3_stps,
                       &configuration.y4_stps,
                       &configuration.z1_stps,
                       &configuration.z2_stps,
                       &configuration.z3_stps,
                       &configuration.z4_stps};
    Setup* stp[12] = {
      &(sc[0]->adc.setup1),
      &(sc[1]->adc.setup1),
      &(sc[2]->adc.setup1),
      &(sc[3]->adc.setup1),
      &(sc[4]->adc.setup1),
      &(sc[5]->adc.setup1),
      &(sc[6]->adc.setup1),
      &(sc[7]->adc.setup1),
      &(sc[8]->adc.setup1),
      &(sc[9]->adc.setup1),
      &(sc[10]->adc.setup1),
      &(sc[11]->adc.setup1)
    };
    for(int i=0;i<8;++i){
      for(int j=0;j<12;++j){
        switch(i){
          case 0:
            stp[j] = &(sc[j]->adc.setup1);
            break;
          case 1:
            stp[j] = &(sc[j]->adc.setup2);
            break;
          case 2:
            stp[j] = &(sc[j]->adc.setup3);
            break;
          case 3:
            stp[j] = &(sc[j]->adc.setup4);
            break;
          case 4:
            stp[j] = &(sc[j]->adc.setup5);
            break;
          case 5:
            stp[j] = &(sc[j]->adc.setup6);
            break;
          case 6:
            stp[j] = &(sc[j]->adc.setup7);
            break;
          case 7:
            stp[j] = &(sc[j]->adc.setup8);
            break;
        }
        stps[j]->name[i] = stp[j]->name;
        stps[j]->out[i] = stp[j]->out;
        stps[j]->ga[i] = stp[j]->stpVals[2];
        stps[j]->fs[i] = stp[j]->stpVals[3];
      }
    }
    updateFlash();
  }
  else{
    SGSC* sc[12] = {&sc0,
                    &sc1,
                    &sc2,
                    &sc3,
                    &sc4,
                    &sc5,
                    &sc6,
                    &sc7,
                    &sc8,
                    &sc9,
                    &sc10,
                    &sc11};
    Setups* stps[12] = {&configuration.x1_stps,
                       &configuration.x2_stps,
                       &configuration.x3_stps,
                       &configuration.x4_stps,
                       &configuration.y1_stps,
                       &configuration.y2_stps,
                       &configuration.y3_stps,
                       &configuration.y4_stps,
                       &configuration.z1_stps,
                       &configuration.z2_stps,
                       &configuration.z3_stps,
                       &configuration.z4_stps};
    Setup* stp[12] = {
      &(sc[0]->adc.setup1),
      &(sc[1]->adc.setup1),
      &(sc[2]->adc.setup1),
      &(sc[3]->adc.setup1),
      &(sc[4]->adc.setup1),
      &(sc[5]->adc.setup1),
      &(sc[6]->adc.setup1),
      &(sc[7]->adc.setup1),
      &(sc[8]->adc.setup1),
      &(sc[9]->adc.setup1),
      &(sc[10]->adc.setup1),
      &(sc[11]->adc.setup1)
    };
    reg regs[8] = {
      Setup1,
      Setup2,
      Setup3,
      Setup4,
      Setup5,
      Setup6,
      Setup7,
      Setup8
    };
    for(int j=0;j<12;++j){
      for(int i=0;i<8;++i){
        switch(i){
          case 0:
            stp[j] = &(sc[j]->adc.setup1);
            break;
          case 1:
            stp[j] = &(sc[j]->adc.setup2);
            break;
          case 2:
            stp[j] = &(sc[j]->adc.setup3);
            break;
          case 3:
            stp[j] = &(sc[j]->adc.setup4);
            break;
          case 4:
            stp[j] = &(sc[j]->adc.setup5);
            break;
          case 5:
            stp[j] = &(sc[j]->adc.setup6);
            break;
          case 6:
            stp[j] = &(sc[j]->adc.setup7);
            break;
          case 7:
            stp[j] = &(sc[j]->adc.setup8);
            break;
        }
        stp[j]->name = stps[j]->name[i];
        stp[j]->out = stps[j]->out[i];
        stp[j]->pgiaG = stps[j]->gi[i];
        stp[j]->parse();
        sc[j]->adc.writeReg(regs[i],0);
      }
      sc[j]->adc.CSRset = true;
    }
  }
}

void loadOCals(){
  SGSC* sc[12] = {
    &sc0,
    &sc1,
    &sc2,
    &sc3,
    &sc4,
    &sc5,
    &sc6,
    &sc7,
    &sc8,
    &sc9,
    &sc10,
    &sc11
  };
  uint32_t* oCals[12] = {
    &configuration.x1Ocal,
    &configuration.x2Ocal,
    &configuration.x3Ocal,
    &configuration.x4Ocal,
    &configuration.y1Ocal,
    &configuration.y2Ocal,
    &configuration.y3Ocal,
    &configuration.y4Ocal,
    &configuration.z1Ocal,
    &configuration.z2Ocal,
    &configuration.z3Ocal,
    &configuration.z4Ocal
  };
  for(int i=0;i<12;++i){
    sc[i]->adc.uploadOffset(Offset1,*oCals[i],0);
  }
}

void loadGCals(){
  SGSC* sc[12] = {
    &sc0,
    &sc1,
    &sc2,
    &sc3,
    &sc4,
    &sc5,
    &sc6,
    &sc7,
    &sc8,
    &sc9,
    &sc10,
    &sc11
  };
  uint32_t* gCals[12] = {
    &configuration.x1Gcal,
    &configuration.x2Gcal,
    &configuration.x3Gcal,
    &configuration.x4Gcal,
    &configuration.y1Gcal,
    &configuration.y2Gcal,
    &configuration.y3Gcal,
    &configuration.y4Gcal,
    &configuration.z1Gcal,
    &configuration.z2Gcal,
    &configuration.z3Gcal,
    &configuration.z4Gcal
  };
  for(int i=0;i<12;++i){
    sc[i]->adc.uploadGain(Gain1,*gCals[i],0);
  }
}

void checkPGIAs(){
  sdoF = false;
  syncADCs = false;
  lcd.writeChar(0,2,9);
  byte gia0 = sc0.checkPGIA();
  lcd.writeChar(1,2,9);
  byte gia1 = sc1.checkPGIA();
  lcd.writeChar(2,2,9);
  byte gia2 = sc2.checkPGIA();
  lcd.writeChar(3,2,9);
  byte gia3 = sc3.checkPGIA();
  lcd.writeChar(0,2,9);
  byte gia4 = sc4.checkPGIA();
  lcd.writeChar(1,2,9);
  byte gia5 = sc5.checkPGIA();
  lcd.writeChar(2,2,9);
  byte gia6 = sc6.checkPGIA();
  lcd.writeChar(3,2,9);
  byte gia7 = sc7.checkPGIA();
  lcd.writeChar(0,2,9);
  byte gia8 = sc8.checkPGIA();
  lcd.writeChar(1,2,9);
  byte gia9 = sc9.checkPGIA();
  lcd.writeChar(2,2,9);
  byte gia10 = sc10.checkPGIA();
  lcd.writeChar(3,2,9);
  byte gia11 = sc11.checkPGIA();
  lcd.writeChar(0,2,9);

  if(gia0>0){
    for(byte i=gia0;i<4;++i){
      sc0.jk();
    }
  }
  if(gia1>0){
    for(byte i=gia1;i<4;++i){
      sc1.jk();
    }
  }
  if(gia2>0){
    for(byte i=gia2;i<4;++i){
      sc2.jk();
    }
  }
  if(gia3>0){
    for(byte i=gia3;i<4;++i){
      sc3.jk();
    }
  }
  if(gia4>0){
    for(byte i=gia4;i<4;++i){
      sc4.jk();
    }
  }
  if(gia5>0){
    for(byte i=gia5;i<4;++i){
      sc5.jk();
    }
  }
  if(gia6>0){
    for(byte i=gia6;i<4;++i){
      sc6.jk();
    }
  }
  if(gia7>0){
    for(byte i=gia7;i<4;++i){
      sc7.jk();
    }
  }
  if(gia8>0){
    for(byte i=gia8;i<4;++i){
      sc8.jk();
    }
  }
  if(gia9>0){
    for(byte i=gia9;i<4;++i){
      sc9.jk();
    }
  }
  if(gia10>0){
    for(byte i=gia10;i<4;++i){
      sc10.jk();
    }
  }
  if(gia11>0){
    for(byte i=gia11;i<4;++i){
      sc11.jk();
    }
  }
  loadOCals();
  loadGCals();
}

void loadPGIAs(){
  SGSC* sc[12] = {&sc0,
                  &sc1,
                  &sc2,
                  &sc3,
                  &sc4,
                  &sc5,
                  &sc6,
                  &sc7,
                  &sc8,
                  &sc9,
                  &sc10,
                  &sc11};
  byte* Gb[12] = {&configuration.x1_Gia,
                  &configuration.x2_Gia,
                  &configuration.x3_Gia,
                  &configuration.x4_Gia,
                  &configuration.y1_Gia,
                  &configuration.y2_Gia,
                  &configuration.y3_Gia,
                  &configuration.y4_Gia,
                  &configuration.z1_Gia,
                  &configuration.z2_Gia,
                  &configuration.z3_Gia,
                  &configuration.z4_Gia};
  for(int i=0;i<12;++i){
    for(int j=0;j<*Gb[i];++j){
      sc[i]->jk();
    }
    sc[i]->pgiaG = *Gb[i];
  }
}

void syncBridges(byte _r2){
  sc0.setBridge(_r2);
  sc1.setBridge(_r2);
  sc2.setBridge(_r2);
  sc3.setBridge(_r2);
  sc4.setBridge(_r2);
  sc5.setBridge(_r2);
  sc6.setBridge(_r2);
  sc7.setBridge(_r2);
  sc8.setBridge(_r2);
  sc9.setBridge(_r2);
  sc10.setBridge(_r2);
  sc11.setBridge(_r2);
  delay(10);
}
