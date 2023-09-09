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
  menu,
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
  cancelTriangulation
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
  switch(page){
    case logo:
      page = plogo();
      break;
    case menu:
      page = pmenu();
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
    default:
      page = pmenu();
      break;
  }
}

pages plogo(){
  Serial.println("Logo");
  delay(500);
  return menu;
}
pages pmenu(){
  Serial.println("Main Menu");
  num_blurbs = 3;
  String blurbs[] = {
    "Run Trial",
    "Calibrate",
    "Settings"
  };
  while(1){
    if(scroll==true){
      sel = knob(num_blurbs, blurbs);
    }
    switch(sel){
      case 0:
        ipage = trial;
        break;
      case 1:
        ipage = calibrate;
        break;
      case 2:
        ipage = settings;
        break;
      default:
        ipage = menu;
        break;
    }
    if(bounce==true){
      return ipage;
    }
  }
}
pages ptrial(){
  Serial.println("Run Trial?");
  while(1){
    if(scroll==true){
      sel = stop_go();
    }
    switch(sel){
      case 0:
        ipage = menu;
        break;
      case 1:
        ipage = trialSpeed;
        break;
      default:
        ipage = trial;
        break;
    }
    if(bounce==true){
      return ipage;
    }
  }
}
pages ptrialSpeed(){
  Serial.println("Set Trial Speed");
  
}
pages ptrialDuration(){
  Serial.println("Set Trial Duration");
  
}
pages ptrialStatus(){
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
  Serial.println("Trial Complete!");
  Serial.println("> OK");
  while(1){
    if(bounce==true){
      return menu;
    }
  }
}
pages pcalibrate(){
  Serial.println("Calibrate");
  num_blurbs = 5;
  String blurbs[] = {
    "Speed",
    "Orientation",
    "Vibration",
    "Force",
    "Back"
  };
  while(1){
    if(scroll==true){
      sel = knob(num_blurbs, blurbs);
      Serial.print("Sel:  ");Serial.println(sel);
      switch(sel){
        case 0:
          ipage = calSpeed;
          break;
        case 1:
          ipage = calOrientation;
          break;
        case 2:
          ipage = calVibration;
          break;
        case 3:
          ipage = calForce;
          break;
        case 4:
          ipage = menu;
          break;
        default:
          ipage = calibrate;
          break;
      }
    }
    if(bounce==true){
      return ipage;
    }
  }
}
pages pcalSpeed(){
  Serial.println("Calibrate Speed");
  num_blurbs = 4;
  String blurbs[] = {
    "Step Input",
    "Ramp Input",
    "Speed Regulation",
    "Back"
  };
  while(1){
    if(scroll==true){
      sel = knob(num_blurbs, blurbs);
    }
    switch(sel){
      case 0:
        page = speedStep;
        break;
      case 1:
        page = speedRamp;
        break;
      case 2:
        page = speedStability;
        break;
      case 3:
        page = calibrate;
        break;
      default:
        page = calSpeed;
        break;
    }
    if(bounce==true){
      return page;
    }
  }
}
pages pcalOrientation(){
  Serial.println("Calibrate Orientation");
  while(1){
    if(scroll==true){
      sel = stop_go();
    }
    switch(sel){
      case 0:
        ipage = calibrate;
        break;
      case 1:
        ipage = orientationProg;
        break;
      default:
        ipage = calOrientation;
        break;
    }
    if(bounce==true){
      return ipage;
    }
  }
}
pages pcalVibration(){
  Serial.println("Calibrate Vibration");
  num_blurbs = 4;
  String blurbs[] = {
    "Ambient Vibration",
    "Motor-Induced Vibration",
    "Natural Frequency",
    "Back"
  };
  while(1){
    if(scroll==true){
      sel = knob(num_blurbs, blurbs);
    }
    switch(sel){
      case 0:
        page = vibStatic;
        break;
      case 1:
        page = vibActive;
        break;
      case 2:
        page = vibNatFreq;
        break;
      case 3:
        page = calibrate;
        break;
      default:
        page = calVibration;
        break;
    }
    if(bounce==true){
      return page;
    }
  }
}
pages pcalForce(){
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
  while(1){
    if(scroll==true){
      sel = knob(num_blurbs, blurbs);
    }
    switch(sel){
      case 0:
        page = forceWeight;
        break;
      case 1:
        page = forceSensitivity;
        break;
      case 2:
        page = forceOffset;
        break;
      case 3:
        page = forceDrift;
        break;
      case 4:
        page = forceTriangulation;
        break;
      case 5:
        page = calibrate;
        break;
      default:
        page = calForce;
        break;
    }
    if(bounce==true){
      return page;
    }
  }
}
pages pspeedStep(){
  Serial.println("Step Input");
  
}
pages pspeedRamp(){
  Serial.println("Ramp Input");
  
}
pages pspeedStability(){
  Serial.println("Speed Regulation");
  
}
pages pstepProg(){
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
  Serial.println("Step Test Complete!");
  Serial.println("> OK");
  while(1){
    if(bounce==true){
      return calSpeed;
    }
  }
}
pages prampProg(){
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
  Serial.println("Ramp Test Complete!");
  Serial.println("> OK");
  while(1){
    if(bounce==true){
      return calSpeed;
    }
  }
}
pages pstabilityProg(){
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
  Serial.println("Stability Test Complete!");
  Serial.println("> OK");
  while(1){
    if(bounce==true){
      return calSpeed;
    }
  }
}
pages porientationProg(){
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
  Serial.println("Normal Vector Found!");
  Serial.println("> OK");
  while(1){
    if(bounce==true){
      return calibrate;
    }
  }
}
pages pvibStatic(){
  Serial.println("Measure Ambient Vibrations");
  num_blurbs = 4;
  String blurbs[] = {
    "Chassis",
    "Sensor Arms",
    "Full Body",
    "Back"
  };
  while(1){
    if(scroll==true){
      sel = knob(num_blurbs, blurbs);
    }
    switch(sel){
      case 0:
        ipage = vibStaticAccel;
        break;
      case 1:
        ipage = vibStaticSG;
        break;
      case 2:
        ipage = vibStaticBoth;
        break;
      case 3:
        ipage = calVibration;
        break;
      default:
        ipage = vibStatic;
        break;
    }
    if(bounce==true){
      return ipage;
    }
  }
}
pages pvibStaticAccel(){
  Serial.println("Measure Ambient Vibrations in Chassis");
  while(1){
    if(scroll==true){
      sel = stop_go();
    }
    switch(sel){
      case 0:
        ipage = vibStatic;
        break;
      case 1:
        ipage = vibStaticAccelProg;
        break;
      default:
        ipage = vibStaticAccel;
        break;
    }
    if(bounce==true){
      return ipage;
    }
  }
}
pages pvibStaticSG(){
  Serial.println("Measure Ambient Vibrations in Sensor Arms");
  while(1){
    if(scroll==true){
      sel = stop_go();
    }
    switch(sel){
      case 0:
        ipage = vibStatic;
        break;
      case 1:
        ipage = vibStaticSGProg;
        break;
      default:
        ipage = vibStaticSG;
        break;
    }
    if(bounce==true){
      return ipage;
    }
  }
}
pages pvibStaticBoth(){
  Serial.println("Measure Ambient Vibrations in Chassis and Sensor Arms");
  while(1){
    if(scroll==true){
      sel = stop_go();
    }
    switch(sel){
      case 0:
        ipage = vibStatic;
        break;
      case 1:
        ipage = vibStaticBothProg;
        break;
      default:
        ipage = vibStaticBoth;
        break;
    }
    if(bounce==true){
      return ipage;
    }
  }
}
pages pvibStaticAccelProg(){
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
  Serial.println("Ambient Chassis Vibration Test Complete!");
  Serial.println("> OK");
  while(1){
    if(bounce==true){
      return vibStatic;
    }
  }
}
pages pvibStaticSGProg(){
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
  Serial.println("Ambient Sensor Arm Vibration Test Complete!");
  Serial.println("> OK");
  while(1){
    if(bounce==true){
      return vibStatic;
    }
  }
}
pages pvibStaticBothProg(){
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
  Serial.println("Ambient Full Body Vibration Test Complete!");
  Serial.println("> OK");
  while(1){
    if(bounce==true){
      return vibStatic;
    }
  }
}
pages pvibNatFreq(){
  Serial.println("Measure Natural Frequency of Sensor Arms\nRemove Treadmill");
  while(1){
    if(scroll==true){
      sel = stop_go();
    }
    switch(sel){
      case 0:
        ipage = calVibration;
        break;
      case 1:
        ipage = vibNatFreqBL;
        break;
      default:
        ipage = vibNatFreq;
        break;
    }
    if(bounce==true){
      return ipage;
    }
  }
}
pages pvibNatFreqBL(){
  Serial.println("Pluck the Back Left Arm");
  
}
pages pvibNatFreqBLProg(){
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
  Serial.println("Pluck the Back Right Arm");
  
}
pages pvibNatFreqBRProg(){
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
  Serial.println("Pluck the Front Left Arm");
  
}
pages pvibNatFreqFLProg(){
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
  Serial.println("Pluck the Front Right Arm");
  
}
pages pvibNatFreqFRProg(){
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
  Serial.println("Replace Treadmill and Hit LIGHTLY with a Hammer at Center of Bed");
  
}
pages pvibNatFreqHammerProg(){
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
  Serial.println("Natural Frequency of Sensor Arms Calculated!");
  Serial.println("> OK");
  while(1){
    if(bounce==true){
      return calVibration;
    }
  }
}
pages pvibActive(){
  Serial.println("Measure Motor-Induced Vibrations");
  num_blurbs = 4;
  String blurbs[] = {
    "Chassis",
    "Sensor Arms",
    "Full Body",
    "Back"
  };
  while(1){
    if(scroll==true){
      sel = knob(num_blurbs, blurbs);
    }
    switch(sel){
      case 0:
        ipage = vibActiveAccel;
        break;
      case 1:
        ipage = vibActiveSG;
        break;
      case 2:
        ipage = vibActiveBoth;
        break;
      case 3:
        ipage = calVibration;
        break;
      default:
        ipage = vibActive;
        break;
    }
    if(bounce==true){
      return ipage;
    }
  }
}
pages pvibActiveAccel(){
  Serial.println("Measure Motor-Induced Vibrations in Chassis");
  while(1){
    if(scroll==true){
      sel = stop_go();
    }
    switch(sel){
      case 0:
        ipage = vibActive;
        break;
      case 1:
        ipage = vibActiveAccelProg;
        break;
      default:
        ipage = vibActiveAccel;
        break;
    }
    if(bounce==true){
      return ipage;
    }
  }
}
pages pvibActiveSG(){
  Serial.println("Measure Motor-Induced Vibrations in Sensor Arms");
  while(1){
    if(scroll==true){
      sel = stop_go();
    }
    switch(sel){
      case 0:
        ipage = vibActive;
        break;
      case 1:
        ipage = vibActiveSGProg;
        break;
      default:
        ipage = vibActiveSG;
        break;
    }
    if(bounce==true){
      return ipage;
    }
  }
}
pages pvibActiveBoth(){
  Serial.println("Measure Motor-Induced Vibrations in Chassis and Sensor Arms");
  while(1){
    if(scroll==true){
      sel = stop_go();
    }
    switch(sel){
      case 0:
        ipage = vibActive;
        break;
      case 1:
        ipage = vibActiveBothProg;
        break;
      default:
        ipage = vibActiveBoth;
        break;
    }
    if(bounce==true){
      return ipage;
    }
  }
}
pages pvibActiveAccelProg(){
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
  Serial.println("Motor-Induced Chassis Vibration Test Complete!");
  Serial.println("> OK");
  while(1){
    if(bounce==true){
      return vibActive;
    }
  }
}
pages pvibActiveSGProg(){
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
  Serial.println("Motor-Induced Sensor Arm Vibration Test Complete!");
  Serial.println("> OK");
  while(1){
    if(bounce==true){
      return vibActive;
    }
  }
}
pages pvibActiveBothProg(){
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
  Serial.println("Motor-Induced Full Body Vibration Test Complete!");
  Serial.println("> OK");
  while(1){
    if(bounce==true){
      return vibActive;
    }
  }
}
pages pforceWeight(){
  Serial.println("Measure Treadmill Weight");
  while(1){
    if(scroll==true){
      sel = stop_go();
    }
    switch(sel){
      case 0:
        ipage = calForce;
        break;
      case 1:
        ipage = weightUnloaded;
        break;
      default:
        ipage = forceWeight;
        break;
    }
    if(bounce==true){
      return ipage;
    }
  }
}
pages pforceSensitivity(){
  Serial.println("Calibrate Strain Equations");
  num_blurbs = 3;
  String blurbs[] = {
    "Actual Strain",
    "Simulated Strain",
    "Back"
  };
  while(1){
    if(scroll==true){
      sel = knob(num_blurbs, blurbs);
    }
    switch(sel){
      case 0:
        ipage = sensitivityActual;
        break;
      case 1:
        ipage = sensitivitySimulated;
        break;
      case 2:
        ipage = calForce;
        break;
      default:
        ipage = forceSensitivity;
        break;
    }
    if(bounce==true){
      return ipage;
    }
  }
}
pages pforceOffset(){
  Serial.println("Measure Bridge Offset");
  while(1){
    if(scroll==true){
      sel = stop_go();
    }
    switch(sel){
      case 0:
        ipage = calForce;
        break;
      case 1:
        ipage = offsetUnloaded;
        break;
      default:
        ipage = forceOffset;
        break;
    }
    if(bounce==true){
      return ipage;
    }
  }
}
pages pforceDrift(){
  Serial.println("Measure Bridge Drift");
  num_blurbs = 3;
  String blurbs[] = {
    "Thermal Drift",
    "Creep Drift",
    "Back"
  };
  while(1){
    if(scroll==true){
      sel = knob(num_blurbs, blurbs);
    }
    switch(sel){
      case 0:
        ipage = driftThermal;
        break;
      case 1:
        ipage = driftCreep;
        break;
      case 2:
        ipage = calForce;
        break;
      default:
        ipage = forceDrift;
        break;
    }
    if(bounce==true){
      return ipage;
    }
  }
}
pages pforceTriangulation(){
  Serial.println("Calibrate Force Vector Triangulation");
  while(1){
    if(scroll==true){
      sel = stop_go();
    }
    switch(sel){
      case 0:
        ipage = calForce;
        break;
      case 1:
        ipage = triangulationStep1;
        break;
      default:
        ipage = forceTriangulation;
        break;
    }
    if(bounce==true){
      return ipage;
    }
  }
}
pages pweightUnloaded(){
  Serial.println("Please Remove Treadmill");
  while(1){
    if(scroll==true){
      sel = stop_go();
    }
    switch(sel){
      case 0:
        ipage = forceWeight;
        break;
      case 1:
        ipage = weightUnloadedProg;
        break;
      default:
        ipage = weightUnloaded;
        break;
    }
    if(bounce==true){
      return ipage;
    }
  }
}
pages pweightUnloadedProg(){
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
  Serial.println("Please Replace Treadmill");
  while(1){
    if(scroll==true){
      sel = stop_go();
    }
    switch(sel){
      case 0:
        ipage = forceWeight;
        break;
      case 1:
        ipage = weightLoadedProg;
        break;
      default:
        ipage = weightLoaded;
        break;
    }
    if(bounce==true){
      return ipage;
    }
  }
}
pages pweightLoadedProg(){
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
  Serial.println("Treadmill Weight Calculated!");
  Serial.println("> OK");
  while(1){
    if(bounce==true){
      return calForce;
    }
  }
}
pages psensitivityActual(){
  Serial.println("Calibrate Sensitivity via Known Masses");
  while(1){
    if(scroll==true){
      sel = stop_go();
    }
    switch(sel){
      case 0:
        ipage = forceSensitivity;
        break;
      case 1:
        ipage = sensitivityActualProg;
        break;
      default:
        ipage = sensitivityActual;
        break;
    }
    if(bounce==true){
      return ipage;
    }
  }
}
pages psensitivityActualProg(){
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
  Serial.println("Known Mass Sensitivity Calibration Complete!");
  Serial.println("> OK");
  while(1){
    if(bounce==true){
      return forceSensitivity;
    }
  }
}
pages psensitivitySimulated(){
  Serial.println("Calibrate Sensitivity via Simulated Strain");
  num_blurbs = 3;
  String blurbs[] = {
    "Shunt Strain",
    "Potentiometer Strain",
    "Back"
  };
  while(1){
    if(scroll==true){
      sel = knob(num_blurbs, blurbs);
    }
    switch(sel){
      case 0:
        ipage = sensitivitySimulatedShunt;
        break;
      case 1:
        ipage = sensitivitySimulatedPot;
        break;
      case 2:
        ipage = forceSensitivity;
        break;
      default:
        ipage = sensitivitySimulated;
        break;
    }
    if(bounce==true){
      return ipage;
    }
  }
}
pages psensitivitySimulatedShunt(){
  Serial.println("Simulate Strain via Shunt Resistor");
  while(1){
    if(scroll==true){
      sel = stop_go();
    }
    switch(sel){
      case 0:
        ipage = sensitivitySimulated;
        break;
      case 1:
        ipage = sensitivitySimulatedShuntProg;
        break;
      default:
        ipage = sensitivitySimulatedShunt;
        break;
    }
    if(bounce==true){
      return ipage;
    }
  }
}
pages psensitivitySimulatedShuntProg(){
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
  Serial.println("Shunt Strain Sensitivity Calibration Complete!");
  Serial.println("> OK");
  while(1){
    if(bounce==true){
      return sensitivitySimulated;
    }
  }
}
pages psensitivitySimulatedPot(){
  Serial.println("Simulate Strain via Potentiometer");
  while(1){
    if(scroll==true){
      sel = stop_go();
    }
    switch(sel){
      case 0:
        ipage = sensitivitySimulated;
        break;
      case 1:
        ipage = sensitivitySimulatedPotProg;
        break;
      default:
        ipage = sensitivitySimulatedPot;
        break;
    }
    if(bounce==true){
      return ipage;
    }
  }
}
pages psensitivitySimulatedPotProg(){
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
  Serial.println("Potentiometer Strain Sensitivity Calibration Complete!");
  Serial.println("> OK");
  while(1){
    if(bounce==true){
      return sensitivitySimulated;
    }
  }
}
pages poffsetUnloaded(){
  Serial.println("Please Remove Treadmill");
  while(1){
    if(scroll==true){
      sel = stop_go();
    }
    switch(sel){
      case 0:
        ipage = forceOffset;
        break;
      case 1:
        ipage = offsetUnloadedProg;
        break;
      default:
        ipage = offsetUnloaded;
        break;
    }
    if(bounce==true){
      return ipage;
    }
  }
}
pages poffsetUnloadedProg(){
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
  Serial.println("Unloaded Offset Measured");
  Serial.println("> OK");
  while(1){
    if(bounce==true){
      return offsetLoaded;
    }
  }
}
pages poffsetLoaded(){
  Serial.println("Please Replace Treadmill");
  while(1){
    if(scroll==true){
      sel = stop_go();
    }
    switch(sel){
      case 0:
        ipage = forceOffset;
        break;
      case 1:
        ipage = offsetLoadedProg;
        break;
      default:
        ipage = offsetLoaded;
        break;
    }
    if(bounce==true){
      return ipage;
    }
  }
}
pages poffsetLoadedProg(){
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
  Serial.println("Offset Measurement Complete!");
  Serial.println("> OK");
  while(1){
    if(bounce==true){
      return calForce;
    }
  }
}
pages pdriftThermal(){
  Serial.println("Measure Bridge Offset From Resistive Heating in Strain Gauges");
  while(1){
    if(scroll==true){
      sel = stop_go();
    }
    switch(sel){
      case 0:
        ipage = forceDrift;
        break;
      case 1:
        ipage = driftThermalProg;
        break;
      default:
        ipage = driftThermal;
        break;
    }
    if(bounce==true){
      return ipage;
    }
  }
}
pages pdriftThermalProg(){
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
  Serial.println("Thermal Offset Calibration Complete!");
  Serial.println("> OK");
  while(1){
    if(bounce==true){
      return forceDrift;
    }
  }
}
pages pdriftCreep(){
  Serial.println("Measure Bridge Offset From Beam Creep and Residual Strain");
  while(1){
    if(scroll==true){
      sel = stop_go();
    }
    switch(sel){
      case 0:
        ipage = forceDrift;
        break;
      case 1:
        ipage = driftCreepProg;
        break;
      default:
        ipage = driftCreep;
        break;
    }
    if(bounce==true){
      return ipage;
    }
  }
}
pages pdriftCreepProg(){
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
  Serial.println("Beam Creep Offset Calibration Complete!");
  Serial.println("> OK");
  while(1){
    if(bounce==true){
      return forceDrift;
    }
  }
}
pages ptriangulationStep1(){
  Serial.println("Please Place Specified Masses at Specified Locations When Instructed");
  while(1){
    if(scroll==true){
      sel = stop_go();
    }
    switch(sel){
      case 0:
        ipage = forceTriangulation;
        break;
      case 1:
        ipage = triangulationStep2;
        break;
      default:
        ipage = triangulationStep1;
        break;
    }
    if(bounce==true){
      return ipage;
    }
  }
}
pages ptriangulationStep2(){
  Serial.println("Place Random Masses at Random Locations and Judge Systems Estimate");
  while(1){
    if(scroll==true){
      sel = stop_go();
    }
    switch(sel){
      case 0:
        ipage = forceTriangulation;
        break;
      case 1:
        ipage = triangulationProg;
        break;
      default:
        ipage = triangulationStep2;
        break;
    }
    if(bounce==true){
      return ipage;
    }
  }
}
pages ptriangulationProg(){
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
  Serial.println("Triangulation Calibration Complete!");
  Serial.println("> OK");
  while(1){
    if(bounce==true){
      return calForce;
    }
  }
}
pages psettings(){
  Serial.println("Settings");
  num_blurbs = 5;
  String blurbs[] = {
    "Adjust Sampling Rate",
    "Adjust LCD Brightness",
    "Move Motor",
    "Debug",
    "Back"
  };
  while(1){
    if(scroll==true){
      sel = knob(num_blurbs, blurbs);
    }
    switch(sel){
      case 0:
        ipage = settingsFs;
        break;
      case 1:
        ipage = settingsLCDBrightness;
        break;
      case 2:
        ipage = settingsMoveMotor;
        break;
      case 3:
        ipage = settingsDeBug;
        break;
      case 4:
        ipage = menu;
        break;
      default:
        ipage = settings;
        break;
    }
    if(bounce==true){
      return ipage;
    }
  }
}
pages psettingsFs(){
  Serial.println("Set Sampling Frequency");
  
}
pages psettingsLCDBrightness(){
  Serial.println("Set LCD Brightness");
  
}
pages psettingsMoveMotor(){
  Serial.println("Move Motor");
  num_blurbs = 3;
  String blurbs[] = {
    "Scroll Distance",
    "Scroll Speed",
    "Back"
  };
  while(1){
    if(scroll==true){
      sel = knob(num_blurbs, blurbs);
    }
    switch(sel){
      case 0:
        ipage = moveMotorDistance;
        break;
      case 1:
        ipage = moveMotorSpeed;
        break;
      case 2:
        ipage = settings;
        break;
      default:
        ipage = settingsMoveMotor;
        break;
    }
    if(bounce==true){
      return ipage;
    }
  }
}
pages psettingsDeBug(){
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
  while(1){
    if(scroll==true){
      sel = knob(num_blurbs, blurbs);
    }
    switch(sel){
      case 0:
        ipage = deBugRun;
        break;
      case 1:
        ipage = deBugClimb;
        break;
      case 2:
        ipage = deBugDance;
        break;
      case 3:
        ipage = deBugPlay;
        break;
      case 4:
        ipage = deBugMan;
        break;
      case 5:
        ipage = settings;
        break;
      default:
        ipage = settingsDeBug;
        break;
    }
    if(bounce==true){
      return ipage;
    }
  }
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
  Serial.println("Trial Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return trial;
    }
  }
}
pages pcancelStep(){
  Serial.println("Step Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return speedStep;
    }
  }
}
pages pcancelRamp(){
  Serial.println("Ramp Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return speedRamp;
    }
  }
}
pages pcancelStability(){
  Serial.println("Speed Stability Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return speedStability;
    }
  }
}
pages pcancelOrientation(){
  Serial.println("Orientation Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return calOrientation;
    }
  }
}
pages pcancelVibStaticAccel(){
  Serial.println("Chassis Ambient Vibration Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return vibStaticAccel;
    }
  }
}
pages pcancelVibStaticSG(){
  Serial.println("Sensor Arm Ambient Vibration Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return vibStaticSG;
    }
  }
}
pages pcancelVibStaticBoth(){
  Serial.println("Full Body Ambient Vibration Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return vibStaticBoth;
    }
  }
}
pages pcancelVibNatFreq(){
  Serial.println("Natural Frequency Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return vibNatFreq;
    }
  }
}
pages pcancelVibActiveAccel(){
  Serial.println("Chassis Motor Vibration Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return vibActiveAccel;
    }
  }
}
pages pcancelVibActiveSG(){
  Serial.println("Sensor Arm Motor Vibration Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return vibActiveSG;
    }
  }
}
pages pcancelVibActiveBoth(){
  Serial.println("Full Body Motor Vibration Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return vibActiveBoth;
    }
  }
}
pages pcancelWeight(){
  Serial.println("Treadmill Weight Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return forceWeight;
    }
  }
}
pages pcancelSensitivityActual(){
  Serial.println("Real Strain Sensitivity Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return sensitivityActual;
    }
  }
}
pages pcancelSensitivitySimulatedShunt(){
  Serial.println("Shunt Strain Sensitivity Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return sensitivitySimulatedShunt;
    }
  }
}
pages pcancelSensitivitySimulatedPot(){
  Serial.println("Potentiometer Strain Sensitivity Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return sensitivitySimulatedPot;
    }
  }
}
pages pcancelOffset(){
  Serial.println("Initial Offset Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return forceOffset;
    }
  }
}
pages pcancelDriftThermal(){
  Serial.println("Thermal Drift Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return driftThermal;
    }
  }
}
pages pcancelDriftCreep(){
  Serial.println("Creep Drift Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return driftCreep;
    }
  }
}
pages pcancelTriangulation(){
  Serial.println("Triangulation Test Cancelled!");
  Serial.println("> Return");
  while(1){
    if(bounce==true){
      return forceTriangulation;
    }
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
