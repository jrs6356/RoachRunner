#include <DueFlashStorage.h>
DueFlashStorage dueFlashStorage;

struct Configuration {
  char* greeting;
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
  int Fs;
};
Configuration configuration;

void setup() {
  Serial.begin(250000);
  while(!Serial){}
  initFlash();
  Serial.println(configuration.greeting);
  configuration.greeting = "Cheddar Toes!";
  updateFlash();
}

void loop() {
  // put your main code here, to run repeatedly:

}

void initFlash(){
  uint8_t firstRun = dueFlashStorage.read(0);
  if(firstRun){
    Serial.println("Allocating Flash Storage for Configuration");
    configuration.greeting = "Poop a fart";
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
    configuration.x1_Gia = 1;
    configuration.x2_Gia = 1;
    configuration.x3_Gia = 1;
    configuration.x4_Gia = 1;
    configuration.y1_Gia = 1;
    configuration.y2_Gia = 1;
    configuration.y3_Gia = 1;
    configuration.y4_Gia = 1;
    configuration.z1_Gia = 1;
    configuration.z2_Gia = 1;
    configuration.z3_Gia = 1;
    configuration.z4_Gia = 1;
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
    configuration.Fs = 0;
    updateFlash();
  }
  else{
    loadFlash();
  }
}

void updateFlash(){
  Serial.println("Uploading Configuration to Flash");
  byte b2[sizeof(Configuration)];
  memcpy(b2, &configuration, sizeof(Configuration));
  dueFlashStorage.write(4, b2, sizeof(Configuration));
  dueFlashStorage.write(0,0);
}

void loadFlash(){
  Serial.println("Downloading Configuration from Flash");
  byte* b2 = dueFlashStorage.readAddress(4);
  memcpy(&configuration, b2, sizeof(Configuration));
}
