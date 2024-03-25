#include <SPI.h>
#include "RoachRunner_LCD.h"
#include "Digipot.h"

#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>

extern char _end;
extern "C" char *sbrk(int i);
char *ramstart=(char *)0x20070000;
char *ramend=(char *)0x20088000;

//Pin Assignments
const byte inc = 2;
const byte cs_pot = 3;
const byte ud = 4;
const byte up = 5;
const byte down = 6;
const byte ledUp = 7;
const byte ledDown = 8;
const byte sw = 9;
const byte a2 = 10;
const byte b2 = 11;
const byte cs_lcd = 38;
const byte rs_lcd = 39;

LCD lcd(cs_lcd, cs_pot, rs_lcd, inc, ud);

int num_blurbs = 0;
String topline = "";
String midline = "";
String botline = "";
String caption = "";
char buf[18] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', lcd.ellipsis, ' '};
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
volatile int dir = 0;
volatile bool bounce = false;
volatile bool scroll = false;
volatile int a_state = false;
volatile int b_state = false;
unsigned long t1 = 0;
unsigned long t2 = 0;
unsigned long dt = 0;
unsigned long t1A = 0;
unsigned long t2A = 0;
unsigned long dtA = 0;
unsigned long t1B = 0;
unsigned long t2B = 0;
unsigned long dtB = 0;
unsigned long trialT = 0;
unsigned long runProg = 2000;
unsigned long tScroll = 500;

enum pages{
  logo,
  mainMenu,
  trial,
  trialSpeed,
  trialDuration,
  trialStatus,
  trialComp,
  calibrate,
  calSpeed,
  calOrientation,
  calVibration,
  calForce,
  speedStep,
  speedRamp,
  speedStability,
  stepProg,
  stepComp,
  rampProg,
  rampComp,
  stabilityProg,
  stabilityComp,
  orientationProg,
  orientationComp,
  vibStatic,
  vibStaticAccel,
  vibStaticSG,
  vibStaticBoth,
  vibNatFreq,
  vibStaticAccelProg,
  vibStaticAccelComp,
  vibStaticSGProg,
  vibStaticSGComp,
  vibStaticBothProg,
  vibStaticBothComp,
  vibNatFreqBL,
  vibNatFreqBLProg,
  vibNatFreqBR,
  vibNatFreqBRProg,
  vibNatFreqFL,
  vibNatFreqFLProg,
  vibNatFreqFR,
  vibNatFreqFRProg,
  vibNatFreqHammer,
  vibNatFreqHammerProg,
  vibNatFreqComp,
  vibActive,
  vibActiveAccel,
  vibActiveSG,
  vibActiveBoth,
  vibActiveAccelProg,
  vibActiveAccelComp,
  vibActiveSGProg,
  vibActiveSGComp,
  vibActiveBothProg,
  vibActiveBothComp,
  forceWeight,
  forceSensitivity,
  forceOffset,
  forceDrift,
  forceTriangulation,
  weightUnloaded,
  weightUnloadedProg,
  weightLoaded,
  weightLoadedProg,
  weightComp,
  sensitivityActual,
  sensitivityActualProg,
  sensitivityActualComp,
  sensitivitySimulated,
  sensitivitySimulatedShunt,
  sensitivitySimulatedShuntProg,
  sensitivitySimulatedShuntComp,
  sensitivitySimulatedPot,
  sensitivitySimulatedPotProg,
  sensitivitySimulatedPotComp,
  offsetUnloaded,
  offsetUnloadedProg,
  offsetUnloadedComp,
  offsetLoaded,
  offsetLoadedProg,
  offsetLoadedComp,
  driftThermal,
  driftThermalProg,
  driftThermalComp,
  driftCreep,
  driftCreepProg,
  driftCreepComp,
  triangulationStep1,
  triangulationStep2,
  triangulationProg,
  triangulationComp,
  settings,
  settingsFs,
  settingsLCDBrightness,
  settingsMoveMotor,
  settingsDeBug,
  moveMotorDistance,
  moveMotorSpeed,
  deBugDance,
  deBugRun,
  deBugClimb,
  deBugPlay,
  deBugMan,
  cancelTrial,
  cancelStep,
  cancelRamp,
  cancelStability,
  cancelOrientation,
  cancelVibStaticAccel,
  cancelVibStaticSG,
  cancelVibStaticBoth,
  cancelVibNatFreq,
  cancelVibActiveAccel,
  cancelVibActiveSG,
  cancelVibActiveBoth,
  cancelWeight,
  cancelSensitivityActual,
  cancelSensitivitySimulatedShunt,
  cancelSensitivitySimulatedPot,
  cancelOffset,
  cancelDriftThermal,
  cancelDriftCreep,
  cancelTriangulation,
  lock
};
pages page = logo;
pages ipage = page;

void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.println("\n");
  SPI.begin();
  SPI.beginTransaction(SPISettings((210000),MSBFIRST,SPI_MODE3));
  lcd.init();
  pinMode(a2, INPUT_PULLUP);
  pinMode(b2, INPUT_PULLUP);
  pinMode(sw, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(a2), encA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(b2), encB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(sw), ensw, RISING);
  lcd.contrast(73);
}

void loop() {
  Serial.println("-------------------------------------------------------");
  //Serial.print("Page: ");Serial.println(page);
  bounce = false;
  scroll = true;
  scrollText = false;
  dir = 0;
  frame = 0;
  frame_prev = 0;
  row = 0;
  lcd.clear();
  //freeRam();
  taskMaster();
}
void taskMaster(){
  switch(page){
    case logo:
      page = plogo();
      break;
    case mainMenu:
      page = pmainMenu();
      break;
    case trial:
      page = ptrial();
      break;
    case trialSpeed:
      page = ptrialSpeed();
      break;
    case trialDuration:
      page = ptrialDuration();
      break;
    case trialStatus:
      page = ptrialStatus();
      break;
    case trialComp:
      page = ptrialComp();
      break;
    case calibrate:
      page = pcalibrate();
      break;
    case calSpeed:
      page = pcalSpeed();
      break;
    case calOrientation:
      page = pcalOrientation();
      break;
    case calVibration:
      page = pcalVibration();
      break;
    case calForce:
      page = pcalForce();
      break;
    case speedStep:
      page = pspeedStep();
      break;
    case speedRamp:
      page = pspeedRamp();
      break;
    case speedStability:
      page = pspeedStability();
      break;
    case stepProg:
      page = pstepProg();
      break;
    case stepComp:
      page = pstepComp();
      break;
    case rampProg:
      page = prampProg();
      break;
    case rampComp:
      page = prampComp();
      break;
    case stabilityProg:
      page = pstabilityProg();
      break;
    case stabilityComp:
      page = pstabilityComp();
      break;
    case orientationProg:
      page = porientationProg();
      break;
    case orientationComp:
      page = porientationComp();
      break;
    case vibStatic:
      page = pvibStatic();
      break;
    case vibStaticAccel:
      page = pvibStaticAccel();
      break;
    case vibStaticSG:
      page = pvibStaticSG();
      break;
    case vibStaticBoth:
      page = pvibStaticBoth();
      break;
    case vibNatFreq:
      page = pvibNatFreq();
      break;
    case vibStaticAccelProg:
      page = pvibStaticAccelProg();
      break;
    case vibStaticAccelComp:
      page = pvibStaticAccelComp();
      break;
    case vibStaticSGProg:
      page = pvibStaticSGProg();
      break;
    case vibStaticSGComp:
      page = pvibStaticSGComp();
      break;
    case vibStaticBothProg:
      page = pvibStaticBothProg();
      break;
    case vibStaticBothComp:
      page = pvibStaticBothComp();
      break;
    case vibNatFreqBL:
      page = pvibNatFreqBL();
      break;
    case vibNatFreqBLProg:
      page = pvibNatFreqBLProg();
      break;
    case vibNatFreqBR:
      page = pvibNatFreqBR();
      break;
    case vibNatFreqBRProg:
      page = pvibNatFreqBRProg();
      break;
    case vibNatFreqFL:
      page = pvibNatFreqFL();
      break;
    case vibNatFreqFLProg:
      page = pvibNatFreqFLProg();
      break;
    case vibNatFreqFR:
      page = pvibNatFreqFR();
      break;
    case vibNatFreqFRProg:
      page = pvibNatFreqFRProg();
      break;
    case vibNatFreqHammer:
      page = pvibNatFreqHammer();
      break;
    case vibNatFreqHammerProg:
      page = pvibNatFreqHammerProg();
      break;
    case vibNatFreqComp:
      page = pvibNatFreqComp();
      break;
    case vibActive:
      page = pvibActive();
      break;
    case vibActiveAccel:
      page = pvibActiveAccel();
      break;
    case vibActiveSG:
      page = pvibActiveSG();
      break;
    case vibActiveBoth:
      page = pvibActiveBoth();
      break;
    case vibActiveAccelProg:
      page = pvibActiveAccelProg();
      break;
    case vibActiveAccelComp:
      page = pvibActiveAccelComp();
      break;
    case vibActiveSGProg:
      page = pvibActiveSGProg();
      break;
    case vibActiveSGComp:
      page = pvibActiveSGComp();
      break;
    case vibActiveBothProg:
      page = pvibActiveBothProg();
      break;
    case vibActiveBothComp:
      page = pvibActiveBothComp();
      break;
    case forceWeight:
      page = pforceWeight();
      break;
    case forceSensitivity:
      page = pforceSensitivity();
      break;
    case forceOffset:
      page = pforceOffset();
      break;
    case forceDrift:
      page = pforceDrift();
      break;
    case forceTriangulation:
      page = pforceTriangulation();
      break;
    case weightUnloaded:
      page = pweightUnloaded();
      break;
    case weightUnloadedProg:
      page = pweightUnloadedProg();
      break;
    case weightLoaded:
      page = pweightLoaded();
      break;
    case weightLoadedProg:
      page = pweightLoadedProg();
      break;
    case weightComp:
      page = pweightComp();
      break;
    case sensitivityActual:
      page = psensitivityActual();
      break;
    case sensitivityActualProg:
      page = psensitivityActualProg();
      break;
    case sensitivityActualComp:
      page = psensitivityActualComp();
      break;
    case sensitivitySimulated:
      page = psensitivitySimulated();
      break;
    case sensitivitySimulatedShunt:
      page = psensitivitySimulatedShunt();
      break;
    case sensitivitySimulatedShuntProg:
      page = psensitivitySimulatedShuntProg();
      break;
    case sensitivitySimulatedShuntComp:
      page = psensitivitySimulatedShuntComp();
      break;
    case sensitivitySimulatedPot:
      page = psensitivitySimulatedPot();
      break;
    case sensitivitySimulatedPotProg:
      page = psensitivitySimulatedPotProg();
      break;
    case sensitivitySimulatedPotComp:
      page = psensitivitySimulatedPotComp();
      break;
    case offsetUnloaded:
      page = poffsetUnloaded();
      break;
    case offsetUnloadedProg:
      page = poffsetUnloadedProg();
      break;
    case offsetUnloadedComp:
      page = poffsetUnloadedComp();
      break;
    case offsetLoaded:
      page = poffsetLoaded();
      break;
    case offsetLoadedProg:
      page = poffsetLoadedProg();
      break;
    case offsetLoadedComp:
      page = poffsetLoadedComp();
      break;
    case driftThermal:
      page = pdriftThermal();
      break;
    case driftThermalProg:
      page = pdriftThermalProg();
      break;
    case driftThermalComp:
      page = pdriftThermalComp();
      break;
    case driftCreep:
      page = pdriftCreep();
      break;
    case driftCreepProg:
      page = pdriftCreepProg();
      break;
    case driftCreepComp:
      page = pdriftCreepComp();
      break;
    case triangulationStep1:
      page = ptriangulationStep1();
      break;
    case triangulationStep2:
      page = ptriangulationStep2();
      break;
    case triangulationProg:
      page = ptriangulationProg();
      break;
    case triangulationComp:
      page = ptriangulationComp();
      break;
    case settings:
      page = psettings();
      break;
    case settingsFs:
      page = psettingsFs();
      break;
    case settingsLCDBrightness:
      page = psettingsLCDBrightness();
      break;
    case settingsMoveMotor:
      page = psettingsMoveMotor();
      break;
    case settingsDeBug:
      page = psettingsDeBug();
      break;
    case moveMotorDistance:
      page = pmoveMotorDistance();
      break;
    case moveMotorSpeed:
      page = pmoveMotorSpeed();
      break;
    case deBugDance:
      page = pdeBugDance();
      break;
    case deBugRun:
      page = pdeBugRun();
      break;
    case deBugClimb:
      page = pdeBugClimb();
      break;
    case deBugPlay:
      page = pdeBugPlay();
      break;
    case deBugMan:
      page = pdeBugMan();
      break;
    case cancelTrial:
      page = pcancelTrial();
      break;
    case cancelStep:
      page = pcancelStep();
      break;
    case cancelRamp:
      page = pcancelRamp();
      break;
    case cancelStability:
      page = pcancelStability();
      break;
    case cancelOrientation:
      page = pcancelOrientation();
      break;
    case cancelVibStaticAccel:
      page = pcancelVibStaticAccel();
      break;
    case cancelVibStaticSG:
      page = pcancelVibStaticSG();
      break;
    case cancelVibStaticBoth:
      page = pcancelVibStaticBoth();
      break;
    case cancelVibNatFreq:
      page = pcancelVibNatFreq();
      break;
    case cancelVibActiveAccel:
      page = pcancelVibActiveAccel();
      break;
    case cancelVibActiveSG:
      page = pcancelVibActiveSG();
      break;
    case cancelVibActiveBoth:
      page = pcancelVibActiveBoth();
      break;
    case cancelWeight:
      page = pcancelWeight();
      break;
    case cancelSensitivityActual:
      page = pcancelSensitivityActual();
      break;
    case cancelSensitivitySimulatedShunt:
      page = pcancelSensitivitySimulatedShunt();
      break;
    case cancelSensitivitySimulatedPot:
      page = pcancelSensitivitySimulatedPot();
      break;
    case cancelOffset:
      page = pcancelOffset();
      break;
    case cancelDriftThermal:
      page = pcancelDriftThermal();
      break;
    case cancelDriftCreep:
      page = pcancelDriftCreep();
      break;
    case cancelTriangulation:
      page = pcancelTriangulation();
      break;
    case lock:
      page = plock();
      break;
    default:
      page = pmainMenu();
      break;
  }
}
pages plogo(){
  // Display
  caption = "Logo";
  Serial.println(caption);
  delay(500);
  return mainMenu;
}
pages pmainMenu(){
  // Menu
  caption = "Main Menu";
  Serial.println(caption);
  num_blurbs = 3;
  String blurbs[] = {
    "Run Trial",
    "Calibrate",
    "Settings"
  };
  pages states[] = {
    trial,
    calibrate,
    settings,
    lock,
    lock,
    lock,
    mainMenu
  };
  ipage = menu(num_blurbs, blurbs, states);
  return ipage;
}
pages ptrial(){
  // Instruction
  caption = "Run Trial?";
  Serial.println(caption);
  pages states[] = {
    mainMenu,
    trialSpeed,
    trial
  };
  ipage = instruction(states);
  return ipage;
}
pages ptrialSpeed(){
  caption = "Set Trial Speed";
  Serial.println(caption);
  
}
pages ptrialDuration(){
  caption = "Set Trial Duration";
  Serial.println(caption);
  
}
pages ptrialStatus(){
  // Progress
  caption = "Trial Time Remaining";
  Serial.println(caption);
  t1 = millis();
  while(1){
    t2 = millis();
    dt = t2 - t1;
    if(dt>trialT){
      return trialComp;
    }
    if(bounce==true){
      return cancelTrial;
    }
  }
}
pages ptrialComp(){
  // Complete
  caption = "Trial Complete!";
  Serial.println(caption);
  complete();
  return mainMenu;
}
pages pcalibrate(){
  // Menu
  caption = "Calibrate";
  Serial.println(caption);
  num_blurbs = 5;
  String blurbs[] = {
    "Back",
    "Speed",
    "Orientation",
    "Vibration",
    "Force"
  };
  pages states[] = {
    mainMenu,
    calSpeed,
    calOrientation,
    calVibration,
    calForce,
    lock,
    calibrate
  };
  ipage = menu(num_blurbs, blurbs, states);
  return ipage;
}
pages pcalSpeed(){
  // Menu
  caption = "Calibrate Speed";
  Serial.println(caption);
  num_blurbs = 4;
  String blurbs[] = {
    "Back",
    "Step Input",
    "Ramp Input",
    "Speed Regulation"
  };
  pages states[] = {
    calibrate,
    speedStep,
    speedRamp,
    speedStability,
    lock,
    lock,
    calSpeed
  };
  ipage = menu(num_blurbs, blurbs, states);
  return ipage;
}
pages pcalOrientation(){
  // Instruction
  caption = "Calibrate Orientation?";
  Serial.println(caption);
  pages states[] = {
    calibrate,
    orientationProg,
    calOrientation
  };
  ipage = instruction(states);
  return ipage;
}
pages pcalVibration(){
  // Menu
  caption = "Calibrate Vibration";
  Serial.println(caption);
  num_blurbs = 4;
  String blurbs[] = {
    "Back",
    "Ambient Vibration",
    "Motor-Induced Vibration",
    "Natural Frequency"
  };
  pages states[] = {
    calibrate,
    vibStatic,
    vibActive,
    vibNatFreq,
    lock,
    lock,
    calVibration
  };
  ipage = menu(num_blurbs, blurbs, states);
  return ipage;
}
pages pcalForce(){
  // Menu
  caption = "Calibrate Force Systems";
  Serial.println(caption);
  num_blurbs = 6;
  String blurbs[] = {
    "Back",
    "Weigh Treadmill",
    "Tune Strain Equation Gains",
    "Measure Offset",
    "Measure Drift",
    "Triangulate Forces"
  };
  pages states[] = {
    calibrate,
    forceWeight,
    forceSensitivity,
    forceOffset,
    forceDrift,
    forceTriangulation,
    calForce
  };
  ipage = menu(num_blurbs, blurbs, states);
  return ipage;
}
pages pspeedStep(){
  // Instruction
  caption = "Run Step Input Test?";
  Serial.println(caption);
  pages states[] = {
    calSpeed,
    stepProg,
    speedStep
  };
  ipage = instruction(states);
  return ipage;
}
pages pspeedRamp(){
  // Instruction
  caption = "Run Ramp Input Test?";
  Serial.println(caption);
  pages states[] = {
    calSpeed,
    rampProg,
    speedRamp
  };
  ipage = instruction(states);
  return ipage;
}
pages pspeedStability(){
  // Instruction
  caption = "Run Speed Regulation Test?";
  Serial.println(caption);
  pages states[] = {
    calSpeed,
    stabilityProg,
    speedStability
  };
  ipage = instruction(states);
  return ipage;
}
pages pstepProg(){
  // Progress
  caption = "Collecting Data Points...";
  Serial.println(caption);
  t1 = millis();
  while(1){
    t2 = millis();
    dt = t2 - t1;
    if(dt>runProg){
      return stepComp;
    }
    if(bounce==true){
      return cancelStep;
    }
  }
}
pages pstepComp(){
  // Complete
  caption = "Step Test Complete!";
  Serial.println(caption);
  complete();
  return calSpeed;
}
pages prampProg(){
  // Progress
  caption = "Collecting Data Points...";
  Serial.println(caption);
  t1 = millis();
  while(1){
    t2 = millis();
    dt = t2 - t1;
    if(dt>runProg){
      return rampComp;
    }
    if(bounce==true){
      return cancelRamp;
    }
  }
}
pages prampComp(){
  // Complete
  caption = "Ramp Test Complete!";
  Serial.println(caption);
  complete();
  return calSpeed;
}
pages pstabilityProg(){
  // Progress
  caption = "Collecting Data Points...";
  Serial.println(caption);
  t1 = millis();
  while(1){
    t2 = millis();
    dt = t2 - t1;
    if(dt>runProg){
      return stabilityComp;
    }
    if(bounce==true){
      return cancelStability;
    }
  }
}
pages pstabilityComp(){
  // Complete
  caption = "Stability Test Complete!";
  Serial.println(caption);
  complete();
  return calSpeed;
}
pages porientationProg(){
  // Progress
  caption = "Collecting Data Points...";
  Serial.println(caption);
  t1 = millis();
  while(1){
    t2 = millis();
    dt = t2 - t1;
    if(dt>runProg){
      return orientationComp;
    }
    if(bounce==true){
      return cancelOrientation;
    }
  }
}
pages porientationComp(){
  // Complete
  caption = "Normal Vector Found!";
  Serial.println(caption);
  complete();
  return calibrate;
}
pages pvibStatic(){
  // Menu
  caption = "Measure Ambient Vibrations";
  Serial.println(caption);
  num_blurbs = 4;
  String blurbs[] = {
    "Back",
    "Chassis",
    "Sensor Arms",
    "Full Body"
  };
  pages states[] = {
    calVibration,
    vibStaticAccel,
    vibStaticSG,
    vibStaticBoth,
    lock,
    lock,
    vibStatic
  };
  ipage = menu(num_blurbs, blurbs, states);
  return ipage;
}
pages pvibStaticAccel(){
  // Instruction
  caption = "Measure Ambient Vibrations in Chassis?";
  Serial.println(caption);
  pages states[] = {
    vibStatic,
    vibStaticAccelProg,
    vibStaticAccel
  };
  ipage = instruction(states);
  return ipage;
}
pages pvibStaticSG(){
  // Instruction
  caption = "Measure Ambient Vibrations in Sensor Arms?";
  Serial.println(caption);
  pages states[] = {
    vibStatic,
    vibStaticSGProg,
    vibStaticSG
  };
  ipage = instruction(states);
  return ipage;
}
pages pvibStaticBoth(){
  // Instruction
  caption = "Measure Ambient Vibrations in Full Body?";
  Serial.println(caption);
  pages states[] = {
    vibStatic,
    vibStaticBothProg,
    vibStaticBoth
  };
  ipage = instruction(states);
  return ipage;
}
pages pvibStaticAccelProg(){
  // Progress
  caption = "Collecting Data Points...";
  Serial.println(caption);
  t1 = millis();
  while(1){
    t2 = millis();
    dt = t2 - t1;
    if(dt>runProg){
      return vibStaticAccelComp;
    }
    if(bounce==true){
      return cancelVibStaticAccel;
    }
  }
}
pages pvibStaticAccelComp(){
  // Complete
  caption = "Ambient Chassis Vibration Test Complete!";
  Serial.println(caption);
  complete();
  return vibStatic;
}
pages pvibStaticSGProg(){
  // Progress
  caption = "Collecting Data Points...";
  Serial.println(caption);
  t1 = millis();
  while(1){
    t2 = millis();
    dt = t2 - t1;
    if(dt>runProg){
      return vibStaticSGComp;
    }
    if(bounce==true){
      return cancelVibStaticSG;
    }
  }
}
pages pvibStaticSGComp(){
  // Complete
  caption = "Ambient Sensor Arm Vibration Test Complete!";
  Serial.println(caption);
  complete();
  return vibStatic;
}
pages pvibStaticBothProg(){
  // Progress
  caption = "Collecting Data Points...";
  Serial.println(caption);
  t1 = millis();
  while(1){
    t2 = millis();
    dt = t2 - t1;
    if(dt>runProg){
      return vibStaticBothComp;
    }
    if(bounce==true){
      return cancelVibStaticBoth;
    }
  }
}
pages pvibStaticBothComp(){
  // Complete
  caption = "Ambient Full Body Vibration Test Complete!";
  Serial.println(caption);
  complete();
  return vibStatic;
}
pages pvibNatFreq(){
  // Instruction
  caption = "Measure Natural Frequency of Sensor Arms?";
  Serial.println(caption);
  pages states[] = {
    calVibration,
    vibNatFreqBL,
    vibNatFreq
  };
  ipage = instruction(states);
  return ipage;
}
pages pvibNatFreqBL(){
  // Instruction
  caption = "Pluck the Back Left Arm";
  Serial.println(caption);
  pages states[] = {
    cancelVibNatFreq,
    vibNatFreqBLProg,
    vibNatFreqBL
  };
  ipage = instruction(states);
  return ipage;
}
pages pvibNatFreqBLProg(){
  // Progress
  caption = "Collecting Data Points...";
  Serial.println(caption);
  t1 = millis();
  while(1){
    t2 = millis();
    dt = t2 - t1;
    if(dt>runProg){
      return vibNatFreqBR;
    }
    if(bounce==true){
      return cancelVibNatFreq;
    }
  }
}
pages pvibNatFreqBR(){
  // Instruction
  caption = "Pluck the Back Right Arm";
  Serial.println(caption);
  pages states[] = {
    cancelVibNatFreq,
    vibNatFreqBRProg,
    vibNatFreqBR
  };
  ipage = instruction(states);
  return ipage;
}
pages pvibNatFreqBRProg(){
  // Progress
  caption = "Collecting Data Points...";
  Serial.println(caption);
  t1 = millis();
  while(1){
    t2 = millis();
    dt = t2 - t1;
    if(dt>runProg){
      return vibNatFreqFL;
    }
    if(bounce==true){
      return cancelVibNatFreq;
    }
  }
}
pages pvibNatFreqFL(){
  // Instruction
  caption = "Pluck the Front Left Arm";
  Serial.println(caption);
  pages states[] = {
    cancelVibNatFreq,
    vibNatFreqFLProg,
    vibNatFreqFL
  };
  ipage = instruction(states);
  return ipage;
}
pages pvibNatFreqFLProg(){
  // Progress
  caption = "Collecting Data Points...";
  Serial.println(caption);
  t1A = millis();
  while(1){
    t2A = millis();
    dtA = t2A - t1A;
    if(dtA>runProg){
      return vibNatFreqFR;
    }
    if(bounce==true){
      return cancelVibNatFreq;
    }
  }
}
pages pvibNatFreqFR(){
  // Instruction
  caption = "Pluck the Front Right Arm";
  Serial.println(caption);
  pages states[] = {
    cancelVibNatFreq,
    vibNatFreqFRProg,
    vibNatFreqFR
  };
  ipage = instruction(states);
  return ipage;
}
pages pvibNatFreqFRProg(){
  // Progress
  caption = "Collecting Data Points...";
  Serial.println(caption);
  t1A = millis();
  while(1){
    t2A = millis();
    dtA = t2A - t1A;
    if(dtA>runProg){
      return vibNatFreqHammer;
    }
    if(bounce==true){
      return cancelVibNatFreq;
    }
  }
}
pages pvibNatFreqHammer(){
  // Instruction
  caption = "Replace Treadmill and Hit LIGHTLY with a Hammer at Center of Bed";
  Serial.println(caption);
  pages states[] = {
    cancelVibNatFreq,
    vibNatFreqHammerProg,
    vibNatFreqHammer
  };
  ipage = instruction(states);
  return ipage;
}
pages pvibNatFreqHammerProg(){
  // Progress
  caption = "Collecting Data Points...";
  Serial.println(caption);
  t1A = millis();
  while(1){
    t2A = millis();
    dtA = t2A - t1A;
    if(dtA>runProg){
      return vibNatFreqComp;
    }
    if(bounce==true){
      return cancelVibNatFreq;
    }
  }
}
pages pvibNatFreqComp(){
  // Complete
  caption = "Natural Frequency of Sensor Arms Calculated!";
  Serial.println(caption);
  complete();
  return calVibration;
}
pages pvibActive(){
  // Menu
  caption = "Measure Motor-Induced Vibrations";
  Serial.println(caption);
  num_blurbs = 4;
  String blurbs[] = {
    "Back",
    "Chassis",
    "Sensor Arms",
    "Full Body"
  };
  pages states[] = {
    calVibration,
    vibActiveAccel,
    vibActiveSG,
    vibActiveBoth,
    lock,
    lock,
    vibActive
  };
  ipage = menu(num_blurbs, blurbs, states);
  return ipage;
}
pages pvibActiveAccel(){
  // Instruction
  caption = "Measure Motor-Induced Vibrations in Chassis?";
  Serial.println(caption);
  pages states[] = {
    vibActive,
    vibActiveAccelProg,
    vibActiveAccel
  };
  ipage = instruction(states);
  return ipage;
}
pages pvibActiveSG(){
  // Instruction
  caption = "Measure Motor-Induced Vibrations in Sensor Arms?";
  Serial.println(caption);
  pages states[] = {
    vibActive,
    vibActiveSGProg,
    vibActiveSG
  };
  ipage = instruction(states);
  return ipage;
}
pages pvibActiveBoth(){
  // Instruction
  caption = "Measure Motor-Induced Vibrations in Full Body?";
  Serial.println(caption);
  pages states[] = {
    vibActive,
    vibActiveBothProg,
    vibActiveBoth
  };
  ipage = instruction(states);
  return ipage;
}
pages pvibActiveAccelProg(){
  // Progress
  caption = "Collecting Data Points...";
  Serial.println(caption);
  t1A = millis();
  while(1){
    t2A = millis();
    dtA = t2A - t1A;
    if(dtA>runProg){
      return vibActiveAccelComp;
    }
    if(bounce==true){
      return cancelVibActiveAccel;
    }
  }
}
pages pvibActiveAccelComp(){
  // Complete
  caption = "Motor-Induced Chassis Vibration Test Complete!";
  Serial.println(caption);
  complete();
  return vibActive;
}
pages pvibActiveSGProg(){
  // Progress
  caption = "Collecting Data Points...";
  Serial.println(caption);
  t1A = millis();
  while(1){
    t2A = millis();
    dtA = t2A - t1A;
    if(dtA>runProg){
      return vibActiveSGComp;
    }
    if(bounce==true){
      return cancelVibActiveSG;
    }
  }
}
pages pvibActiveSGComp(){
  // Complete
  caption = "Motor-Induced Sensor Arm Vibration Test Complete!";
  Serial.println(caption);
  complete();
  return vibActive;
}
pages pvibActiveBothProg(){
  // Progress
  caption = "Collecting Data Points...";
  Serial.println(caption);
  t1A = millis();
  while(1){
    t2A = millis();
    dtA = t2A - t1A;
    if(dtA>runProg){
      return vibActiveBothComp;
    }
    if(bounce==true){
      return cancelVibActiveBoth;
    }
  }
}
pages pvibActiveBothComp(){
  // Complete
  caption = "Motor-Induced Full Body Vibration Test Complete!";
  Serial.println(caption);
  complete();
  return vibActive;
}
pages pforceWeight(){
  // Instruction
  caption = "Measure Treadmill Weight?";
  Serial.println(caption);
  pages states[] = {
    calForce,
    weightUnloaded,
    forceWeight
  };
  ipage = instruction(states);
  return ipage;
}
pages pforceSensitivity(){
  // Menu
  caption = "Calibrate Strain Equations";
  Serial.println(caption);
  num_blurbs = 3;
  String blurbs[] = {
    "Back",
    "Actual Strain",
    "Simulated Strain"
  };
  pages states[] = {
    calForce,
    sensitivityActual,
    sensitivitySimulated,
    lock,
    lock,
    lock,
    forceSensitivity
  };
  ipage = menu(num_blurbs, blurbs, states);
  return ipage;
}
pages pforceOffset(){
  // Instruction
  caption = "Measure Bridge Offset?";
  Serial.println(caption);
  pages states[] = {
    calForce,
    offsetUnloaded,
    forceOffset
  };
  ipage = instruction(states);
  return ipage;
}
pages pforceDrift(){
  // Menu
  caption = "Measure Bridge Drift";
  Serial.println(caption);
  num_blurbs = 3;
  String blurbs[] = {
    "Back",
    "Thermal Drift",
    "Creep Drift"
  };
  pages states[] = {
    calForce,
    driftThermal,
    driftCreep,
    lock,
    lock,
    lock,
    forceDrift
  };
  ipage = menu(num_blurbs, blurbs, states);
  return ipage;
}
pages pforceTriangulation(){
  // Instruction
  caption = "Calibrate Force Vector Triangulation?";
  Serial.println(caption);
  pages states[] = {
    calForce,
    triangulationStep1,
    forceTriangulation
  };
  ipage = instruction(states);
  return ipage;
}
pages pweightUnloaded(){
  // Instruction
  caption = "Please Remove Treadmill";
  Serial.println(caption);
  pages states[] = {
    cancelWeight,
    weightUnloadedProg,
    weightUnloaded
  };
  ipage = instruction(states);
  return ipage;
}
pages pweightUnloadedProg(){
  // Progress
  caption = "Collecting Data Points...";
  Serial.println(caption);
  t1A = millis();
  while(1){
    t2A = millis();
    dtA = t2A - t1A;
    if(dtA>runProg){
      return weightLoaded;
    }
    if(bounce==true){
      return cancelWeight;
    }
  }
}
pages pweightLoaded(){
  // Instruction
  caption = "Please Replace Treadmill";
  Serial.println(caption);
  pages states[] = {
    cancelWeight,
    weightLoadedProg,
    weightLoaded
  };
  ipage = instruction(states);
  return ipage;
}
pages pweightLoadedProg(){
  // Progress
  caption = "Collecting Data Points...";
  Serial.println(caption);
  t1A = millis();
  while(1){
    t2A = millis();
    dtA = t2A - t1A;
    if(dtA>runProg){
      return weightComp;
    }
    if(bounce==true){
      return cancelWeight;
    }
  }
}
pages pweightComp(){
  // Complete
  caption = "Treadmill Weight Calculated!";
  Serial.println(caption);
  complete();
  return calForce;
}
pages psensitivityActual(){
  // Instruction
  caption = "Calibrate Sensitivity via Known Masses?";
  Serial.println(caption);
  pages states[] = {
    forceSensitivity,
    sensitivityActualProg,
    sensitivityActual
  };
  ipage = instruction(states);
  return ipage;
}
pages psensitivityActualProg(){
  // Progress
  caption = "Collecting Data Points...";
  Serial.println(caption);
  t1A = millis();
  while(1){
    t2A = millis();
    dtA = t2A - t1A;
    if(dtA>runProg){
      return sensitivityActualComp;
    }
    if(bounce==true){
      return cancelSensitivityActual;
    }
  }
}
pages psensitivityActualComp(){
  // Complete
  caption = "Known Mass Sensitivity Calibration Complete!";
  Serial.println(caption);
  complete();
  return forceSensitivity;
}
pages psensitivitySimulated(){
  // Menu
  caption = "Calibrate Sensitivity via Simulated Strain";
  Serial.println(caption);
  num_blurbs = 3;
  String blurbs[] = {
    "Back",
    "Shunt Strain",
    "Potentiometer Strain"
  };
  pages states[] = {
    forceSensitivity,
    sensitivitySimulatedShunt,
    sensitivitySimulatedPot,
    lock,
    lock,
    lock,
    sensitivitySimulated
  };
  ipage = menu(num_blurbs, blurbs, states);
  return ipage;
}
pages psensitivitySimulatedShunt(){
  // Instruction
  caption = "Simulate Strain via Shunt Resistor?";
  Serial.println(caption);
  pages states[] = {
    sensitivitySimulated,
    sensitivitySimulatedShuntProg,
    sensitivitySimulatedShunt
  };
  ipage = instruction(states);
  return ipage;
}
pages psensitivitySimulatedShuntProg(){
  // Progress
  caption = "Collecting Data Points...";
  Serial.println(caption);
  t1A = millis();
  while(1){
    t2A = millis();
    dtA = t2A - t1A;
    if(dtA>runProg){
      return sensitivitySimulatedShuntComp;
    }
    if(bounce==true){
      return cancelSensitivitySimulatedShunt;
    }
  }
}
pages psensitivitySimulatedShuntComp(){
  // Complete
  caption = "Shunt Strain Sensitivity Calibration Complete!";
  Serial.println(caption);
  complete();
  return sensitivitySimulated;
}
pages psensitivitySimulatedPot(){
  // Instruction
  caption = "Simulate Strain via Potentiometer?";
  Serial.println(caption);
  pages states[] = {
    sensitivitySimulated,
    sensitivitySimulatedPotProg,
    sensitivitySimulatedPot
  };
  ipage = instruction(states);
  return ipage;
}
pages psensitivitySimulatedPotProg(){
  // Progress
  caption = "Collecting Data Points...";
  Serial.println(caption);
  t1A = millis();
  while(1){
    t2A = millis();
    dtA = t2A - t1A;
    if(dtA>runProg){
      return sensitivitySimulatedPotComp;
    }
    if(bounce==true){
      return cancelSensitivitySimulatedPot;
    }
  }
}
pages psensitivitySimulatedPotComp(){
  // Complete
  caption = "Potentiometer Strain Sensitivity Calibration Complete!";
  Serial.println(caption);
  complete();
  return sensitivitySimulated;
}
pages poffsetUnloaded(){
  // Instruction
  caption = "Please Remove Treadmill";
  Serial.println(caption);
  pages states[] = {
    cancelOffset,
    offsetUnloadedProg,
    offsetUnloaded
  };
  ipage = instruction(states);
  return ipage;
}
pages poffsetUnloadedProg(){
  // Progress
  caption = "Collecting Data Points...";
  Serial.println(caption);
  t1A = millis();
  while(1){
    t2A = millis();
    dtA = t2A - t1A;
    if(dtA>runProg){
      return offsetUnloadedComp;
    }
    if(bounce==true){
      return cancelOffset;
    }
  }
}
pages poffsetUnloadedComp(){
  // Complete
  caption = "Unloaded Offset Measured";
  Serial.println(caption);
  complete();
  return offsetLoaded;
}
pages poffsetLoaded(){
  // Instruction
  caption = "Please Replace Treadmill";
  Serial.println(caption);
  pages states[] = {
    cancelOffset,
    offsetLoadedProg,
    offsetLoaded
  };
  ipage = instruction(states);
  return ipage;
}
pages poffsetLoadedProg(){
  // Progress
  caption = "Collecting Data Points...";
  Serial.println(caption);
  t1A = millis();
  while(1){
    t2A = millis();
    dtA = t2A - t1A;
    if(dtA>runProg){
      return offsetLoadedComp;
    }
    if(bounce==true){
      return cancelOffset;
    }
  }
}
pages poffsetLoadedComp(){
  // Complete
  caption = "Offset Measurement Complete!";
  Serial.println(caption);
  complete();
  return calForce;
}
pages pdriftThermal(){
  // Instruction
  caption = "Measure Bridge Drift From Resistive Heating?";
  Serial.println(caption);
  pages states[] = {
    forceDrift,
    driftThermalProg,
    driftThermal
  };
  ipage = instruction(states);
  return ipage;
}
pages pdriftThermalProg(){
  // Progress
  caption = "Collecting Data Points...";
  Serial.println(caption);
  t1A = millis();
  while(1){
    t2A = millis();
    dtA = t2A - t1A;
    if(dtA>runProg){
      return driftThermalComp;
    }
    if(bounce==true){
      return cancelDriftThermal;
    }
  }
}
pages pdriftThermalComp(){
  // Complete
  caption = "Thermal Offset Calibration Complete!";
  Serial.println(caption);
  complete();
  return forceDrift;
}
pages pdriftCreep(){
  // Instruction
  caption = "Measure Bridge Drift From Beam Creep and Residual Strain?";
  Serial.println(caption);
  pages states[] = {
    forceDrift,
    driftCreepProg,
    driftCreep
  };
  ipage = instruction(states);
  return ipage;
}
pages pdriftCreepProg(){
  // Progress
  caption = "Collecting Data Points...";
  Serial.println(caption);
  t1A = millis();
  while(1){
    t2A = millis();
    dtA = t2A - t1A;
    if(dtA>runProg){
      return driftCreepComp;
    }
    if(bounce==true){
      return cancelDriftCreep;
    }
  }
}
pages pdriftCreepComp(){
  // Complete
  caption = "Beam Creep Offset Calibration Complete!";
  Serial.println(caption);
  complete();
  return forceDrift;
}
pages ptriangulationStep1(){
  // Instruction
  caption = "Please Place Specified Masses at Specified Locations When Instructed";
  Serial.println(caption);
  pages states[] = {
    cancelTriangulation,
    triangulationStep2,
    triangulationStep1
  };
  ipage = instruction(states);
  return ipage;
}
pages ptriangulationStep2(){
  // Instruction
  caption = "Place Random Masses at Random Locations and Judge Systems Estimate";
  Serial.println(caption);
  pages states[] = {
    cancelTriangulation,
    triangulationProg,
    triangulationStep2
  };
  ipage = instruction(states);
  return ipage;
}
pages ptriangulationProg(){
  // Progress
  caption = "Collecting Data Points...";
  Serial.println(caption);
  t1A = millis();
  while(1){
    t2A = millis();
    dtA = t2A - t1A;
    if(dtA>runProg){
      return triangulationComp;
    }
    if(bounce==true){
      return cancelTriangulation;
    }
  }
}
pages ptriangulationComp(){
  // Complete
  caption = "Triangulation Calibration Complete!";
  Serial.println(caption);
  complete();
  return calForce;
}
pages psettings(){
  // Menu
  caption = "Settings";
  Serial.println(caption);
  num_blurbs = 5;
  String blurbs[] = {
    "Back",
    "Adjust Sampling Rate",
    "Adjust LCD Brightness",
    "Move Motor",
    "Debug"
  };
  pages states[] = {
    mainMenu,
    settingsFs,
    settingsLCDBrightness,
    settingsMoveMotor,
    settingsDeBug,
    lock,
    settings
  };
  ipage = menu(num_blurbs, blurbs, states);
  return ipage;
}
pages psettingsFs(){
  caption = "Set Sampling Frequency";
  Serial.println(caption);
  
}
pages psettingsLCDBrightness(){
  caption = "Set LCD Brightness";
  Serial.println(caption);
  lcd.writeRow(caption,0);
  lcd.loadingBar();
  for(int i=0;i<20;++i){
    lcd.writeChar(lcd.x0,3,i);
  }
  lcd.writeChar(lcd.x1,3,0);
  lcd.writeChar(lcd.x0r,3,19);
  lcd.command(lcd.row2[0]);
  lcd.print("Brightness =    /100");
  int fullCell = 0;
  int partial = 0;
  while(1){
    if(scroll){
      lcd.contrast(lcd.brightness + dir);
      lcd.command(lcd.row2[13]);
      if(lcd.brightness<100){
        lcd.print(" ");
      }
      lcd.print(String(lcd.brightness));
      fullCell = floor(lcd.brightness/5);
      partial = lcd.brightness%5;
      Serial.print("Brightness:\t");Serial.println(lcd.brightness);
      Serial.print("Full Cells:\t");Serial.println(fullCell);
      Serial.print("Partial:\t");Serial.println(partial);
      for(int i=0;i<fullCell;++i){
        lcd.writeChar(lcd.x5,3,i);
      }
      if(fullCell==19){
        switch(partial){
          case 1:
            lcd.writeData(lcd.x1r);
            break;
          case 2:
            lcd.writeData(lcd.x2r);
            break;
          case 3:
            lcd.writeData(lcd.x3r);
            break;
          case 4:
            lcd.writeData(lcd.x5);
            break;
          default:
            lcd.writeData(lcd.x0r);
            break;
        }
      }else{
        switch(partial){
          case 1:
            lcd.writeData(lcd.x1);
            break;
          case 2:
            lcd.writeData(lcd.x2);
            break;
          case 3:
            lcd.writeData(lcd.x3);
            break;
          case 4:
            lcd.writeData(lcd.x4);
            break;
          default:
            if(fullCell<20){
              lcd.writeData(lcd.x0);
            }
            break;
        }
      }
      scroll = false;
    }
    if(bounce){
      lcd.clear();
      lcd.makeExtraChars();
      return settings;
    }
  }
}
pages psettingsMoveMotor(){
  // Menu
  caption = "Move Motor";
  Serial.println(caption);
  num_blurbs = 3;
  String blurbs[] = {
    "Back",
    "Scroll Distance",
    "Scroll Speed"
  };
  pages states[] = {
    settings,
    moveMotorDistance,
    moveMotorSpeed,
    lock,
    lock,
    lock,
    settingsMoveMotor
  };
  ipage = menu(num_blurbs, blurbs, states);
  return ipage;
}
pages psettingsDeBug(){
  // Menu
  caption = "Debug";
  Serial.println(caption);
  num_blurbs = 6;
  String blurbs[] = {
    "Back",
    "De' Bug Run",
    "De' Bug Climb",
    "De' Bug Dance",
    "De' Bug Play",
    "De' Bug Man"
  };
  pages states[] = {
    settings,
    deBugRun,
    deBugClimb,
    deBugDance,
    deBugPlay,
    deBugMan,
    settingsDeBug
  };
  ipage = menu(num_blurbs, blurbs, states);
  return ipage;
}
pages pmoveMotorDistance(){
  caption = "Scroll Knob to Move Motor Incrementally";
  Serial.println(caption);
  
}
pages pmoveMotorSpeed(){
  caption = "Scroll Knob to Adjust Speed of Motor";
  Serial.println(caption);
  
}
pages pdeBugDance(){
  caption = "Go cockroach, go cockroach, go!";
  Serial.println(caption);
  while(1){
    if(bounce==true){
      return settingsDeBug;
    }
  }
}
pages pdeBugRun(){
  caption = "Wanna see some real speed?";
  Serial.println(caption);
  while(1){
    if(bounce==true){
      return settingsDeBug;
    }
  }
}
pages pdeBugClimb(){
  caption = "What can't he do???";
  Serial.println(caption);
  while(1){
    if(bounce==true){
      return settingsDeBug;
    }
  }
}
pages pdeBugPlay(){
  caption = "Don't get squished!";
  Serial.println(caption);
  while(1){
    if(bounce==true){
      return settingsDeBug;
    }
  }
}
pages pdeBugMan(){
  // Instruction
  caption = "Creator:";
  Serial.println(caption);
  num_blurbs = 6;
  String blurbs[] = {
    "Jackson R. Sizer",
    "Email: jacksonsizer@gmail.com",
    "M.S.M.E. Penn State University, 2022",
    "Advisor: Dr. Jean-Michel Mongeau",
    "Lab: Bio-Motion Systems Laboratory",
    "Back"
  };
  while(1){
    if(scroll==true){
      sel = knob(num_blurbs, blurbs);
    }
    switch(sel){
      case 0:
        ipage = settingsDeBug;
        break;
      case 1:
        ipage = settingsDeBug;
        break;
      case 2:
        ipage = settingsDeBug;
        break;
      case 3:
        ipage = settingsDeBug;
        break;
      case 4:
        ipage = settingsDeBug;
        break;
      case 5:
        ipage = settingsDeBug;
        break;
      default:
        ipage = settingsDeBug;
        break;
    }
    if(bounce){
      if(sel==5){
        return ipage;
      }
      bounce = false;
    }
  }
}
pages pcancelTrial(){
  // Cancel
  caption = "Trial Cancelled!";
  Serial.println(caption);
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return trial;
    }
  }
}
pages pcancelStep(){
  // Cancel
  caption = "Step Test Cancelled!";
  Serial.println(caption);
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return speedStep;
    }
  }
}
pages pcancelRamp(){
  // Cancel
  caption = "Ramp Test Cancelled!";
  Serial.println(caption);
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return speedRamp;
    }
  }
}
pages pcancelStability(){
  // Cancel
  caption = "Speed Stability Test Cancelled!";
  Serial.println(caption);
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return speedStability;
    }
  }
}
pages pcancelOrientation(){
  // Cancel
  caption = "Orientation Test Cancelled!";
  Serial.println(caption);
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return calOrientation;
    }
  }
}
pages pcancelVibStaticAccel(){
  // Cancel
  caption = "Chassis Ambient Vibration Test Cancelled!";
  Serial.println(caption);
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return vibStaticAccel;
    }
  }
}
pages pcancelVibStaticSG(){
  // Cancel
  caption = "Sensor Arm Ambient Vibration Test Cancelled!";
  Serial.println(caption);
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return vibStaticSG;
    }
  }
}
pages pcancelVibStaticBoth(){
  // Cancel
  caption = "Full Body Ambient Vibration Test Cancelled!";
  Serial.println(caption);
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return vibStaticBoth;
    }
  }
}
pages pcancelVibNatFreq(){
  // Cancel
  caption = "Natural Frequency Test Cancelled!";
  Serial.println(caption);
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return vibNatFreq;
    }
  }
}
pages pcancelVibActiveAccel(){
  // Cancel
  caption = "Chassis Motor Vibration Test Cancelled!";
  Serial.println(caption);
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return vibActiveAccel;
    }
  }
}
pages pcancelVibActiveSG(){
  // Cancel
  caption = "Sensor Arm Motor Vibration Test Cancelled!";
  Serial.println(caption);
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return vibActiveSG;
    }
  }
}
pages pcancelVibActiveBoth(){
  // Cancel
  caption = "Full Body Motor Vibration Test Cancelled!";
  Serial.println(caption);
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return vibActiveBoth;
    }
  }
}
pages pcancelWeight(){
  // Cancel
  caption = "Treadmill Weight Test Cancelled!";
  Serial.println(caption);
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return forceWeight;
    }
  }
}
pages pcancelSensitivityActual(){
  // Cancel
  caption = "Real Strain Sensitivity Test Cancelled!";
  Serial.println(caption);
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return sensitivityActual;
    }
  }
}
pages pcancelSensitivitySimulatedShunt(){
  // Cancel
  caption = "Shunt Strain Sensitivity Test Cancelled!";
  Serial.println(caption);
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return sensitivitySimulatedShunt;
    }
  }
}
pages pcancelSensitivitySimulatedPot(){
  // Cancel
  caption = "Potentiometer Strain Sensitivity Test Cancelled!";
  Serial.println(caption);
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return sensitivitySimulatedPot;
    }
  }
}
pages pcancelOffset(){
  // Cancel
  caption = "Initial Offset Test Cancelled!";
  Serial.println(caption);
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return forceOffset;
    }
  }
}
pages pcancelDriftThermal(){
  // Cancel
  caption = "Thermal Drift Test Cancelled!";
  Serial.println(caption);
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return driftThermal;
    }
  }
}
pages pcancelDriftCreep(){
  // Cancel
  caption = "Creep Drift Test Cancelled!";
  Serial.println(caption);
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return driftCreep;
    }
  }
}
pages pcancelTriangulation(){
  // Cancel
  caption = "Triangulation Test Cancelled!";
  Serial.println(caption);
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return forceTriangulation;
    }
  }
}

pages plock(){
  // Cancel
  caption = "Error: System Locked";
  Serial.println(caption);
  Serial.println("Reset Required");
  while(1){}
}

void encA(){
  scroll = true;
  a_state = digitalRead(a2);
  b_state = digitalRead(b2);
  if(a_state==HIGH){
    if(b_state==HIGH){
      dir = -1;
    }
    else{
      dir = 1;
    }
  }
  else{
    if(b_state==HIGH){
      dir = 1;
    }
    else{
      dir = -1;
    }
  }
}

void encB(){
  scroll = true;
  a_state = digitalRead(a2);
  b_state = digitalRead(b2);
  if(b_state==HIGH){
    if(a_state==HIGH){
      dir = 1;
    }
    else{
      dir = -1;
    }
  }
  else{
    if(a_state==HIGH){
      dir = -1;
    }
    else{
      dir = 1;
    }
  }
}

void ensw(){
  if(!bounce){
    bounce = true;
  }
}

pages display_f(){
  
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
  if(page==mainMenu){
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
        if(page==mainMenu){
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
    switch(sel){
      case 0:
        ipage = states[0];
        break;
      case 1:
        ipage = states[1];
        break;
      case 2:
        ipage = states[2];
        break;
      case 3:
        ipage = states[3];
        break;
      case 4:
        ipage = states[4];
        break;
      case 5:
        ipage = states[5];
        break;
      default:
        ipage = states[6];
        break;
    }
    if(bounce){
      delay(500);
      return ipage;
    }
  }
}

pages instruction(pages states[]){
  topline = "                    ";
  midline = "                    ";
  botline = "                    ";
  String bufA = topline+topline;
  len = caption.length();
  if(len>20){
    int i = 20;
    while(1){
      if((caption[i]==' ')||(caption[i]=='-')){
        break;
      }else{
        --i;
      }
    }
    for(int x=0;x<i;++x){
      topline[x] = caption[x];
    }
    len = len - (i + 1);
    if(len>20){
      for(int x=0;x<len;++x){
        bufA[x] = caption[x + i + 1];
      }
      i = 19;
      while(1){
        if((bufA[i]==' ')||(bufA[i]=='-')){
          break;
        }else{
          --i;
        }
      }
      for(int x=0;x<i;++x){
        midline[x] = bufA[x];
      }
      len = len - (i + 1);
      for(int x=0;x<len;++x){
        botline[x] = bufA[x + i + 1];
      }
    }
    else{
      for(int x=0;x<len;++x){
        midline[x] = caption[x+i+1];
      }
    }
  }
  else{
    for(int i=0;i<len;++i){
      topline[i] = caption[i];
    }
  }
  lcd.command(lcd.row0[0]);
  lcd.print(topline);
  lcd.command(lcd.row1[0]);
  lcd.print(midline);
  lcd.command(lcd.row2[0]);
  lcd.print(botline);
  lcd.command(lcd.row3[1]);
  lcd.print("Back       Continue");
  while(1){
    if(scroll){
      sel = stop_go();

    }
    switch(sel){
      case 0:
        ipage = states[0];
        break;
      case 1:
        ipage = states[1];
        break;
      default:
        ipage = states[2];
        break;
    }
    if(bounce){
      return ipage;
    }
  }
}

pages user_int(){
  
}

pages progress(){
  
}

void complete(){
  Serial.println("> OK");
  while(1){
    if(bounce){
      break;
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
      switch(frame){
        case 0:
          frame = 0;
          break;
        case 1:
          frame = 0;
          break;
        case 2:
          frame = 1;
          break;
        case 3:
          frame = 2;
          break;
        case 4:
          frame = 3;
          break;
        default:
          frame = 0;
          break;
      }
    }else{}
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
      switch(frame){
        case 0:
          frame = 1;
          break;
        case 1:
          frame = 2;
          break;
        case 2:
          frame = 3;
          break;
        case 3:
          frame = 4;
          break;
        case 4:
          frame = 4;
          break;
        default:
          frame = 0;
          break;
      }
    }
  }
  if(frame>screens){
    frame = screens;
  }else{}
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
  switch(frame){
    case 0:
      topline = phrases[0];
      midline = phrases[1];
      botline = phrases[2];
      break;
    case 1:
      topline = phrases[1];
      midline = phrases[2];
      botline = phrases[3];
      break;
    case 2:
      topline = phrases[2];
      midline = phrases[3];
      botline = phrases[4];
      break;
    case 3:
      topline = phrases[3];
      midline = phrases[4];
      botline = phrases[5];
      break;
    case 4:
      topline = phrases[4];
      midline = phrases[5];
      botline = phrases[6];
      break;
    default:
      topline = phrases[0];
      midline = phrases[1];
      botline = phrases[2];
      break;
  }
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

void freeRam(){
  char *heapend=sbrk(0);
  register char * stack_ptr asm ("sp");
  struct mallinfo mi=mallinfo();
  Serial.print("\nDynamic ram used: ");
  Serial.println(mi.uordblks);
  Serial.print("Program static ram used ");
  Serial.println(&_end - ramstart); 
  Serial.print("Stack ram used ");
  Serial.println(ramend - stack_ptr); 
  Serial.print("My guess at free mem: ");
  Serial.println(stack_ptr - heapend + mi.fordblks);
} 
