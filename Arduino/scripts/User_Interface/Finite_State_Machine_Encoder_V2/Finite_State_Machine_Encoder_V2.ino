#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>

extern char _end;
extern "C" char *sbrk(int i);
char *ramstart=(char *)0x20070000;
char *ramend=(char *)0x20088000;

const byte a1 = 5;
const byte b1 = 4;
const byte sw = 3;
const byte bt = 11;

int num_blurbs = 0;
String topline = "";
String botline = "";
String ct = "";
String cb = "";
int screens = 0;
int sel = 0;
int frame = 0;
int row = 0;
int ind = 0;
volatile int dir = 0;
volatile bool bounce = false;
volatile bool scroll = false;
volatile int a_state = false;
volatile int b_state = false;
unsigned long t1 = 0;
unsigned long t2 = 0;
unsigned long dt = 0;
unsigned long trialT = 0;
unsigned long runProg = 2000;

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
  pinMode(a1, INPUT_PULLUP);
  pinMode(b1, INPUT_PULLUP);
  pinMode(sw, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(a1), encA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(b1), encB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(sw), ensw, RISING);
  Serial.println("New Encoder");
  //Serial.println(cancelTriangulation);
  delay(500);
}

void loop() {
  delay(500);
  Serial.println("-------------------------------------------------------");
  //Serial.print("Page: ");Serial.println(page);
  bounce = false;
  scroll = true;
  dir = 0;
  frame = 0;
  row = 0;
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
  Serial.println("Logo");
  delay(500);
  return mainMenu;
}
pages pmainMenu(){
  // Menu
  Serial.println("Main Menu");
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
  Serial.println("Run Trial?");
  pages states[] = {
    mainMenu,
    trialSpeed,
    trial
  };
  ipage = instruction(states);
  return ipage;
}
pages ptrialSpeed(){
  Serial.println("Set Trial Speed");
  
}
pages ptrialDuration(){
  Serial.println("Set Trial Duration");
  
}
pages ptrialStatus(){
  // Progress
  Serial.println("Trial Time Remaining");
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
  Serial.println("Trial Complete!");
  complete();
  return mainMenu;
}
pages pcalibrate(){
  // Menu
  Serial.println("Calibrate");
  num_blurbs = 5;
  String blurbs[] = {
    "Speed",
    "Orientation",
    "Vibration",
    "Force",
    "Back"
  };
  pages states[] = {
    calSpeed,
    calOrientation,
    calVibration,
    calForce,
    mainMenu,
    lock,
    calibrate
  };
  ipage = menu(num_blurbs, blurbs, states);
  return ipage;
}
pages pcalSpeed(){
  // Menu
  Serial.println("Calibrate Speed");
  num_blurbs = 4;
  String blurbs[] = {
    "Step Input",
    "Ramp Input",
    "Speed Regulation",
    "Back"
  };
  pages states[] = {
    speedStep,
    speedRamp,
    speedStability,
    calibrate,
    lock,
    lock,
    calSpeed
  };
  ipage = menu(num_blurbs, blurbs, states);
  return ipage;
}
pages pcalOrientation(){
  // Instruction
  Serial.println("Calibrate Orientation");
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
  Serial.println("Calibrate Vibration");
  num_blurbs = 4;
  String blurbs[] = {
    "Ambient Vibration",
    "Motor-Induced Vibration",
    "Natural Frequency",
    "Back"
  };
  pages states[] = {
    vibStatic,
    vibActive,
    vibNatFreq,
    calibrate,
    lock,
    lock,
    calVibration
  };
  ipage = menu(num_blurbs, blurbs, states);
  return ipage;
}
pages pcalForce(){
  // Menu
  Serial.println("Calibrate Force Systems");
  num_blurbs = 6;
  String blurbs[] = {
    "Weigh Treadmill",
    "Tune Strain Equation Gains",
    "Measure Offset",
    "Measure Drift",
    "Triangulate Forces",
    "Back"
  };
  pages states[] = {
    forceWeight,
    forceSensitivity,
    forceOffset,
    forceDrift,
    forceTriangulation,
    calibrate,
    calForce
  };
  ipage = menu(num_blurbs, blurbs, states);
  return ipage;
}
pages pspeedStep(){
  // Instruction
  Serial.println("Step Input");
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
  Serial.println("Ramp Input");
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
  Serial.println("Speed Regulation");
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
  Serial.println("Collecting Data Points...");
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
  Serial.println("Step Test Complete!");
  complete();
  return calSpeed;
}
pages prampProg(){
  // Progress
  Serial.println("Collecting Data Points...");
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
  Serial.println("Ramp Test Complete!");
  complete();
  return calSpeed;
}
pages pstabilityProg(){
  // Progress
  Serial.println("Collecting Data Points...");
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
  Serial.println("Stability Test Complete!");
  complete();
  return calSpeed;
}
pages porientationProg(){
  // Progress
  Serial.println("Collecting Data Points...");
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
  Serial.println("Normal Vector Found!");
  complete();
  return calibrate;
}
pages pvibStatic(){
  // Menu
  Serial.println("Measure Ambient Vibrations");
  num_blurbs = 4;
  String blurbs[] = {
    "Chassis",
    "Sensor Arms",
    "Full Body",
    "Back"
  };
  pages states[] = {
    vibStaticAccel,
    vibStaticSG,
    vibStaticBoth,
    calVibration,
    lock,
    lock,
    vibStatic
  };
  ipage = menu(num_blurbs, blurbs, states);
  return ipage;
}
pages pvibStaticAccel(){
  // Instruction
  Serial.println("Measure Ambient Vibrations in Chassis");
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
  Serial.println("Measure Ambient Vibrations in Sensor Arms");
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
  Serial.println("Measure Ambient Vibrations in Chassis and Sensor Arms");
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
  Serial.println("Collecting Data Points...");
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
  Serial.println("Ambient Chassis Vibration Test Complete!");
  complete();
  return vibStatic;
}
pages pvibStaticSGProg(){
  // Progress
  Serial.println("Collecting Data Points...");
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
  Serial.println("Ambient Sensor Arm Vibration Test Complete!");
  complete();
  return vibStatic;
}
pages pvibStaticBothProg(){
  // Progress
  Serial.println("Collecting Data Points...");
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
  Serial.println("Ambient Full Body Vibration Test Complete!");
  complete();
  return vibStatic;
}
pages pvibNatFreq(){
  // Instruction
  Serial.println("Measure Natural Frequency of Sensor Arms\nRemove Treadmill");
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
  Serial.println("Pluck the Back Left Arm");
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
  Serial.println("Collecting Data Points...");
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
  Serial.println("Pluck the Back Right Arm");
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
  Serial.println("Collecting Data Points...");
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
  Serial.println("Pluck the Front Left Arm");
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
  Serial.println("Collecting Data Points...");
  t1 = millis();
  while(1){
    t2 = millis();
    dt = t2 - t1;
    if(dt>runProg){
      return vibNatFreqFR;
    }
    if(bounce==true){
      return cancelVibNatFreq;
    }
  }
}
pages pvibNatFreqFR(){
  // Instruction
  Serial.println("Pluck the Front Right Arm");
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
  Serial.println("Collecting Data Points...");
  t1 = millis();
  while(1){
    t2 = millis();
    dt = t2 - t1;
    if(dt>runProg){
      return vibNatFreqHammer;
    }
    if(bounce==true){
      return cancelVibNatFreq;
    }
  }
}
pages pvibNatFreqHammer(){
  // Instruction
  Serial.println("Replace Treadmill and Hit LIGHTLY with a Hammer at Center of Bed");
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
  Serial.println("Collecting Data Points...");
  t1 = millis();
  while(1){
    t2 = millis();
    dt = t2 - t1;
    if(dt>runProg){
      return vibNatFreqComp;
    }
    if(bounce==true){
      return cancelVibNatFreq;
    }
  }
}
pages pvibNatFreqComp(){
  // Complete
  Serial.println("Natural Frequency of Sensor Arms Calculated!");
  complete();
  return calVibration;
}
pages pvibActive(){
  // Menu
  Serial.println("Measure Motor-Induced Vibrations");
  num_blurbs = 4;
  String blurbs[] = {
    "Chassis",
    "Sensor Arms",
    "Full Body",
    "Back"
  };
  pages states[] = {
    vibActiveAccel,
    vibActiveSG,
    vibActiveBoth,
    calVibration,
    lock,
    lock,
    vibActive
  };
  ipage = menu(num_blurbs, blurbs, states);
  return ipage;
}
pages pvibActiveAccel(){
  // Instruction
  Serial.println("Measure Motor-Induced Vibrations in Chassis");
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
  Serial.println("Measure Motor-Induced Vibrations in Sensor Arms");
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
  Serial.println("Measure Motor-Induced Vibrations in Chassis and Sensor Arms");
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
  Serial.println("Collecting Data Points...");
  t1 = millis();
  while(1){
    t2 = millis();
    dt = t2 - t1;
    if(dt>runProg){
      return vibActiveAccelComp;
    }
    if(bounce==true){
      return cancelVibActiveAccel;
    }
  }
}
pages pvibActiveAccelComp(){
  // Complete
  Serial.println("Motor-Induced Chassis Vibration Test Complete!");
  complete();
  return vibActive;
}
pages pvibActiveSGProg(){
  // Progress
  Serial.println("Collecting Data Points...");
  t1 = millis();
  while(1){
    t2 = millis();
    dt = t2 - t1;
    if(dt>runProg){
      return vibActiveSGComp;
    }
    if(bounce==true){
      return cancelVibActiveSG;
    }
  }
}
pages pvibActiveSGComp(){
  // Complete
  Serial.println("Motor-Induced Sensor Arm Vibration Test Complete!");
  complete();
  return vibActive;
}
pages pvibActiveBothProg(){
  // Progress
  Serial.println("Collecting Data Points...");
  t1 = millis();
  while(1){
    t2 = millis();
    dt = t2 - t1;
    if(dt>runProg){
      return vibActiveBothComp;
    }
    if(bounce==true){
      return cancelVibActiveBoth;
    }
  }
}
pages pvibActiveBothComp(){
  // Complete
  Serial.println("Motor-Induced Full Body Vibration Test Complete!");
  complete();
  return vibActive;
}
pages pforceWeight(){
  // Instruction
  Serial.println("Measure Treadmill Weight");
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
  Serial.println("Calibrate Strain Equations");
  num_blurbs = 3;
  String blurbs[] = {
    "Actual Strain",
    "Simulated Strain",
    "Back"
  };
  pages states[] = {
    sensitivityActual,
    sensitivitySimulated,
    calForce,
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
  Serial.println("Measure Bridge Offset");
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
  Serial.println("Measure Bridge Drift");
  num_blurbs = 3;
  String blurbs[] = {
    "Thermal Drift",
    "Creep Drift",
    "Back"
  };
  pages states[] = {
    driftThermal,
    driftCreep,
    calForce,
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
  Serial.println("Calibrate Force Vector Triangulation");
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
  Serial.println("Please Remove Treadmill");
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
  Serial.println("Collecting Data Points...");
  t1 = millis();
  while(1){
    t2 = millis();
    dt = t2 - t1;
    if(dt>runProg){
      return weightLoaded;
    }
    if(bounce==true){
      return cancelWeight;
    }
  }
}
pages pweightLoaded(){
  // Instruction
  Serial.println("Please Replace Treadmill");
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
  Serial.println("Collecting Data Points...");
  t1 = millis();
  while(1){
    t2 = millis();
    dt = t2 - t1;
    if(dt>runProg){
      return weightComp;
    }
    if(bounce==true){
      return cancelWeight;
    }
  }
}
pages pweightComp(){
  // Complete
  Serial.println("Treadmill Weight Calculated!");
  complete();
  return calForce;
}
pages psensitivityActual(){
  // Instruction
  Serial.println("Calibrate Sensitivity via Known Masses");
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
  Serial.println("Collecting Data Points...");
  t1 = millis();
  while(1){
    t2 = millis();
    dt = t2 - t1;
    if(dt>runProg){
      return sensitivityActualComp;
    }
    if(bounce==true){
      return cancelSensitivityActual;
    }
  }
}
pages psensitivityActualComp(){
  // Complete
  Serial.println("Known Mass Sensitivity Calibration Complete!");
  complete();
  return forceSensitivity;
}
pages psensitivitySimulated(){
  // Menu
  Serial.println("Calibrate Sensitivity via Simulated Strain");
  num_blurbs = 3;
  String blurbs[] = {
    "Shunt Strain",
    "Potentiometer Strain",
    "Back"
  };
  pages states[] = {
    sensitivitySimulatedShunt,
    sensitivitySimulatedPot,
    forceSensitivity,
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
  Serial.println("Simulate Strain via Shunt Resistor");
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
  Serial.println("Collecting Data Points...");
  t1 = millis();
  while(1){
    t2 = millis();
    dt = t2 - t1;
    if(dt>runProg){
      return sensitivitySimulatedShuntComp;
    }
    if(bounce==true){
      return cancelSensitivitySimulatedShunt;
    }
  }
}
pages psensitivitySimulatedShuntComp(){
  // Complete
  Serial.println("Shunt Strain Sensitivity Calibration Complete!");
  complete();
  return sensitivitySimulated;
}
pages psensitivitySimulatedPot(){
  // Instruction
  Serial.println("Simulate Strain via Potentiometer");
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
  Serial.println("Collecting Data Points...");
  t1 = millis();
  while(1){
    t2 = millis();
    dt = t2 - t1;
    if(dt>runProg){
      return sensitivitySimulatedPotComp;
    }
    if(bounce==true){
      return cancelSensitivitySimulatedPot;
    }
  }
}
pages psensitivitySimulatedPotComp(){
  // Complete
  Serial.println("Potentiometer Strain Sensitivity Calibration Complete!");
  complete();
  return sensitivitySimulated;
}
pages poffsetUnloaded(){
  // Instruction
  Serial.println("Please Remove Treadmill");
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
  Serial.println("Collecting Data Points...");
  t1 = millis();
  while(1){
    t2 = millis();
    dt = t2 - t1;
    if(dt>runProg){
      return offsetUnloadedComp;
    }
    if(bounce==true){
      return cancelOffset;
    }
  }
}
pages poffsetUnloadedComp(){
  // Complete
  Serial.println("Unloaded Offset Measured");
  complete();
  return offsetLoaded;
}
pages poffsetLoaded(){
  // Instruction
  Serial.println("Please Replace Treadmill");
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
  Serial.println("Collecting Data Points...");
  t1 = millis();
  while(1){
    t2 = millis();
    dt = t2 - t1;
    if(dt>runProg){
      return offsetLoadedComp;
    }
    if(bounce==true){
      return cancelOffset;
    }
  }
}
pages poffsetLoadedComp(){
  // Complete
  Serial.println("Offset Measurement Complete!");
  complete();
  return calForce;
}
pages pdriftThermal(){
  // Instruction
  Serial.println("Measure Bridge Offset From Resistive Heating in Strain Gauges");
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
  Serial.println("Collecting Data Points...");
  t1 = millis();
  while(1){
    t2 = millis();
    dt = t2 - t1;
    if(dt>runProg){
      return driftThermalComp;
    }
    if(bounce==true){
      return cancelDriftThermal;
    }
  }
}
pages pdriftThermalComp(){
  // Complete
  Serial.println("Thermal Offset Calibration Complete!");
  complete();
  return forceDrift;
}
pages pdriftCreep(){
  // Instruction
  Serial.println("Measure Bridge Offset From Beam Creep and Residual Strain");
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
  Serial.println("Collecting Data Points...");
  t1 = millis();
  while(1){
    t2 = millis();
    dt = t2 - t1;
    if(dt>runProg){
      return driftCreepComp;
    }
    if(bounce==true){
      return cancelDriftCreep;
    }
  }
}
pages pdriftCreepComp(){
  // Complete
  Serial.println("Beam Creep Offset Calibration Complete!");
  complete();
  return forceDrift;
}
pages ptriangulationStep1(){
  // Instruction
  Serial.println("Please Place Specified Masses at Specified Locations When Instructed");
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
  Serial.println("Place Random Masses at Random Locations and Judge Systems Estimate");
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
  Serial.println("Collecting Data Points...");
  t1 = millis();
  while(1){
    t2 = millis();
    dt = t2 - t1;
    if(dt>runProg){
      return triangulationComp;
    }
    if(bounce==true){
      return cancelTriangulation;
    }
  }
}
pages ptriangulationComp(){
  // Complete
  Serial.println("Triangulation Calibration Complete!");
  complete();
  return calForce;
}
pages psettings(){
  // Menu
  Serial.println("Settings");
  num_blurbs = 5;
  String blurbs[] = {
    "Adjust Sampling Rate",
    "Adjust LCD Brightness",
    "Move Motor",
    "Debug",
    "Back"
  };
  pages states[] = {
    settingsFs,
    settingsLCDBrightness,
    settingsMoveMotor,
    settingsDeBug,
    mainMenu,
    lock,
    settings
  };
  ipage = menu(num_blurbs, blurbs, states);
  return ipage;
}
pages psettingsFs(){
  Serial.println("Set Sampling Frequency");
  
}
pages psettingsLCDBrightness(){
  Serial.println("Set LCD Brightness");
  
}
pages psettingsMoveMotor(){
  // Menu
  Serial.println("Move Motor");
  num_blurbs = 3;
  String blurbs[] = {
    "Scroll Distance",
    "Scroll Speed",
    "Back"
  };
  pages states[] = {
    moveMotorDistance,
    moveMotorSpeed,
    settings,
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
  Serial.println("Debug");
  num_blurbs = 6;
  String blurbs[] = {
    "De' Bug Run",
    "De' Bug Climb",
    "De' Bug Dance",
    "De' Bug Play",
    "De' Bug Man",
    "Back"
  };
  pages states[] = {
    deBugRun,
    deBugClimb,
    deBugDance,
    deBugPlay,
    deBugMan,
    settings,
    settingsDeBug
  };
  ipage = menu(num_blurbs, blurbs, states);
  return ipage;
}
pages pmoveMotorDistance(){
  Serial.println("Scroll Knob to Move Motor Incrementally");
  
}
pages pmoveMotorSpeed(){
  Serial.println("Scroll Knob to Adjust Speed of Motor");
  
}
pages pdeBugDance(){
  Serial.println("Go cockroach, go cockroach, go!");
  while(1){
    if(bounce==true){
      return settingsDeBug;
    }
  }
}
pages pdeBugRun(){
  Serial.println("Wanna see some real speed?");
  while(1){
    if(bounce==true){
      return settingsDeBug;
    }
  }
}
pages pdeBugClimb(){
  Serial.println("What can't he do???");
  while(1){
    if(bounce==true){
      return settingsDeBug;
    }
  }
}
pages pdeBugPlay(){
  Serial.println("Don't get squished!");
  while(1){
    if(bounce==true){
      return settingsDeBug;
    }
  }
}
pages pdeBugMan(){
  // Instruction
  Serial.println("Creator:");
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
    if(bounce==true){
      if(sel==5){
        return ipage;
      }
    }
  }
}
pages pcancelTrial(){
  // Cancel
  Serial.println("Trial Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return trial;
    }
  }
}
pages pcancelStep(){
  // Cancel
  Serial.println("Step Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return speedStep;
    }
  }
}
pages pcancelRamp(){
  // Cancel
  Serial.println("Ramp Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return speedRamp;
    }
  }
}
pages pcancelStability(){
  // Cancel
  Serial.println("Speed Stability Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return speedStability;
    }
  }
}
pages pcancelOrientation(){
  // Cancel
  Serial.println("Orientation Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return calOrientation;
    }
  }
}
pages pcancelVibStaticAccel(){
  // Cancel
  Serial.println("Chassis Ambient Vibration Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return vibStaticAccel;
    }
  }
}
pages pcancelVibStaticSG(){
  // Cancel
  Serial.println("Sensor Arm Ambient Vibration Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return vibStaticSG;
    }
  }
}
pages pcancelVibStaticBoth(){
  // Cancel
  Serial.println("Full Body Ambient Vibration Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return vibStaticBoth;
    }
  }
}
pages pcancelVibNatFreq(){
  // Cancel
  Serial.println("Natural Frequency Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return vibNatFreq;
    }
  }
}
pages pcancelVibActiveAccel(){
  // Cancel
  Serial.println("Chassis Motor Vibration Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return vibActiveAccel;
    }
  }
}
pages pcancelVibActiveSG(){
  // Cancel
  Serial.println("Sensor Arm Motor Vibration Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return vibActiveSG;
    }
  }
}
pages pcancelVibActiveBoth(){
  // Cancel
  Serial.println("Full Body Motor Vibration Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return vibActiveBoth;
    }
  }
}
pages pcancelWeight(){
  // Cancel
  Serial.println("Treadmill Weight Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return forceWeight;
    }
  }
}
pages pcancelSensitivityActual(){
  // Cancel
  Serial.println("Real Strain Sensitivity Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return sensitivityActual;
    }
  }
}
pages pcancelSensitivitySimulatedShunt(){
  // Cancel
  Serial.println("Shunt Strain Sensitivity Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return sensitivitySimulatedShunt;
    }
  }
}
pages pcancelSensitivitySimulatedPot(){
  // Cancel
  Serial.println("Potentiometer Strain Sensitivity Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return sensitivitySimulatedPot;
    }
  }
}
pages pcancelOffset(){
  // Cancel
  Serial.println("Initial Offset Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return forceOffset;
    }
  }
}
pages pcancelDriftThermal(){
  // Cancel
  Serial.println("Thermal Drift Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return driftThermal;
    }
  }
}
pages pcancelDriftCreep(){
  // Cancel
  Serial.println("Creep Drift Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return driftCreep;
    }
  }
}
pages pcancelTriangulation(){
  // Cancel
  Serial.println("Triangulation Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return forceTriangulation;
    }
  }
}

pages plock(){
  // Cancel
  Serial.println("Error: System Locked");
  Serial.println("Reset Required");
  while(1){
  }
}

void encA(){
  scroll = true;
  a_state = digitalRead(a1);
  b_state = digitalRead(b1);
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
  a_state = digitalRead(a1);
  b_state = digitalRead(b1);
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
  if(bounce == false){
    bounce = true;
  }
}

pages display_f(){
  
}

pages menu(int num_blurbs, String blurbs[], pages states[]){
  while(1){
    if(scroll){
      sel = knob(num_blurbs, blurbs);
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
      return ipage;
    }
  }
}

pages instruction(pages states[]){
  while(1){
    if(scroll==true){
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
    if(bounce==true){
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
  int screens = options-2;
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
  else{
    if(dir==-1){
      row = 0;
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
  if(row==1){
    ct = "  ";
    cb = "> ";
  }
  else{
    ct = "> ";
    cb = "  ";
  }
  switch(frame){
    case 0:
      topline = ct + phrases[0];
      botline = cb + phrases[1];
      break;
    case 1:
      topline = ct + phrases[1];
      botline = cb + phrases[2];
      break;
    case 2:
      topline = ct + phrases[2];
      botline = cb + phrases[3];
      break;
    case 3:
      topline = ct + phrases[3];
      botline = cb + phrases[4];
      break;
    case 4:
      topline = ct + phrases[4];
      botline = cb + phrases[5];
      break;
    default:
      topline = ct + phrases[0];
      botline = cb + phrases[1];
      break;
  }
  ind = frame + row;
  Serial.println("----------------");
  Serial.println(topline);
  Serial.println(botline);
  scroll = false;
  return ind;
}

int stop_go(){
  if(dir==1){
    ct = "> ";
    cb = "  ";
    ind = 1;
  }
  else{
    ct = "  ";
    cb = "> ";
    ind = 0;
  }
  topline = ct + "Continue";
  botline = cb + "Back";
  scroll = false;
  Serial.print(botline);Serial.print("\t\t");Serial.println(topline);
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
