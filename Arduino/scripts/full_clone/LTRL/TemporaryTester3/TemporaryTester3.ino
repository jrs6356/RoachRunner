#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_10DOF.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <Servo.h>
#include <SPI.h>
#include <RH_RF95.h>
#include <string.h>

Adafruit_10DOF                dof   = Adafruit_10DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_BMP085_Unified       bmp   = Adafruit_BMP085_Unified(18001);

TinyGPS gps;
SoftwareSerial ss(20, 21);
Servo solarServo;
Servo releaseServo;

#define RFM95_CS 47
#define RFM95_INT 2
#define LED 22
#define RF95_FREQ 915.0

RH_RF95 rf95(RFM95_CS, RFM95_INT);

int commPort = 4;
int groundState = 0;
int deployState = 0;
int vertOr;
long distance;
int counter = 1;
int pos = 0;

#define trigPin 30
#define echoPin 31

int MOTORA_IN1 = 8;
int MOTORA_IN2 = 9;
int MOTORB_IN1 = 10;
int MOTORB_IN2 = 11;

float ti1 = 0.0;
float ti2 = 0.0;
float ti3 = 0.0;
float tOld1 = 0.0;
float tOld2 = 0.0;
float tnew = 0.0;

float xi1 = 0.0;       // u(K)
float xi2 = 0.0;       // u(K-1)
float xi3 = 0.0;       // u(K-2)
float xOld1 = 0.0;    // y(K-1)
float xOld2 = 0.0;    // y(K-2)
float xnew = 0.0;     // y(K)

float yi1 = 0.0;       // u(K)
float yi2 = 0.0;       // u(K-1)
float yi3 = 0.0;       // u(K-2)
float yOld1 = 0.0;    // y(K-1)
float yOld2 = 0.0;    // y(K-2)
float ynew = 0.0;     // y(K)

float zi1 = 0.0;       // u(K)
float zi2 = 0.0;       // u(K-1)
float zi3 = 0.0;       // u(K-2)
float zOld1 = 0.0;    // y(K-1)
float zOld2 = 0.0;    // y(K-2)
float znew = 10;     // y(K)

float a1 = 1.0;     // filter coefficient
float a2 = -1.764;  // filter coefficient
float a3 = 0.789;   // filter coefficient
float b1 = 0.0063;  // filter coefficient
float b2 = 0.0125;  // filter coefficient
float b3 = 0.0063;  // filter coefficient

float tscaled;      // time scaled up to seconds
float accelMag;

float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;

static void smartdelay(unsigned long ms);
static void print_float(float val, float invalid, int len, int prec);
static void print_int(unsigned long val, unsigned long invalid, int len);
static void print_date(TinyGPS &gps);
static void print_str(const char *str, int len);

int gpsCheck = 0;
int i = 0;
int k = 0;
float xCoord;float yCoord;
float xInit; float yInit;
float magTimeStart;
int16_t packetnum = 0;  // packet counter, we increment per xmission

void setup() {
  //solarServo.attach(2);
  //releaseServo.attach(13);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(commPort,INPUT);
  pinMode(MOTORA_IN1, OUTPUT);
  pinMode(MOTORA_IN2, OUTPUT);
  pinMode(MOTORB_IN1, OUTPUT);
  pinMode(MOTORB_IN2, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  #ifndef ESP8266
    while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
  #endif
  Serial.begin(115200);
  
  Serial3.begin(57600);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  
  if(!accel.begin())
  {
     //There was a problem detecting the ADXL345 ... check your connections 
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  if(!mag.begin())
  {
     //There was a problem detecting the LSM303 ... check your connections
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  rf95.setTxPower(23, false);
  
}
void loop() {
  //Serial.println("Made it this far!");
  if(0==groundState){
    groundSig();
  }
  if(1==deployState){
    deploy();
  }
  if(1==gpsCheck)
    drive();
}
void groundSig(){
  sensors_event_t event;
  accel.getEvent(&event);
  
  xi3 = xi2;  // u(K-2) = u(K-1)
  xi2 = xi1;  // u(K-1) = u(K)
  yi3 = yi2;  // u(K-2) = u(K-1)
  yi2 = yi1;  // u(K-1) = u(K)
  zi3 = zi2;  // u(K-2) = u(K-1)
  zi2 = zi1;  // u(K-1) = u(K)
  
  xi1 = event.acceleration.x; // u(K) = current voltage
  yi1 = event.acceleration.y; // u(K) = current voltage
  zi1 = event.acceleration.z; // u(K) = current voltage
  
  xnew = (1/a1)*(b1*xi1 + b2*xi2 + b3*xi3 - a2*xOld1 - a3*xOld2);    // rearranged transfer function to calculate current output
  ynew = (1/a1)*(b1*yi1 + b2*yi2 + b3*yi3 - a2*yOld1 - a3*yOld2);    // rearranged transfer function to calculate current output
  znew = (1/a1)*(b1*zi1 + b2*zi2 + b3*zi3 - a2*zOld1 - a3*zOld2);    // rearranged transfer function to calculate current output
  
  xOld2 = xOld1;  // y(K-1) = y(K)
  xOld1 = xnew;   // y(K) = current output
  yOld2 = yOld1;  // y(K-1) = y(K)
  yOld1 = ynew;   // y(K) = current output
  zOld2 = zOld1;  // y(K-1) = y(K)
  zOld1 = znew;   // y(K) = current output

  accelMag = sqrt(pow(xnew,2) + pow(ynew,2) + pow(znew,2));
  Serial.print("Acceleration:  ");Serial.print(xnew);Serial.print("    ");Serial.print(ynew);Serial.print("    ");Serial.print(znew);
  Serial.print("    ");Serial.println(accelMag);
  delay(50);
  if((9.5<accelMag)&&(10.9>accelMag)){
    Serial.println("Checking ground.......");
    checkGround();
  }
  else{
  }
}
void checkGround(){
  comm();
  //groundState = digitalRead(commPort);
  Serial.println(' ');
  Serial.println(groundState);
  Serial.println(' ');
  if(1==groundState){
    deployState = 1;
  }
  else{
    deployState = 0;
  }
}
void deploy(){
  detonate();
  delay(1000);
  checkOrientation();
  Serial.println(vertOr);
  if(1==counter){
    rampUpForward();
    forward();
    delay(1000);
    rampDownForward();
    counter=0;
  }
  gpsCheck = 1;
  deployState = 0;
}
void detonate(){
  digitalWrite(LED, HIGH);
  delay(2000);
  digitalWrite(LED, LOW);
}
void grabCoords(){
  float flat, flon;
  while(i<5){
    gps.f_get_position(&flat, &flon);
    print_float(flat, TinyGPS::GPS_INVALID_F_ANGLE, 10, 6);
    print_float(flon, TinyGPS::GPS_INVALID_F_ANGLE, 11, 6);
    Serial.println(i);
    smartdelay(1000);
    i = i+1;
  }
  gps.f_get_position(&flat, &flon);
  print_float(flat, TinyGPS::GPS_INVALID_F_ANGLE, 10, 6);
  print_float(flon, TinyGPS::GPS_INVALID_F_ANGLE, 11, 6);
  smartdelay(1000);
  xCoord = flon; yCoord = flat;
  if(0==k){
    Serial.print("Coordinates:     ");
    Serial.print(xCoord,6);Serial.print("   ");Serial.println(yCoord,6);
    yInit = yCoord;xInit = xCoord;
    k = k+1;
  }
}

void comm(){
  Serial.println("Sending to rf95_server");
  // Send a message to rf95_server
  
  char radiopacket[20] = "Hello World #      ";
  itoa(packetnum++, radiopacket+13, 10);
  Serial.print("Sending "); Serial.println(radiopacket);
  radiopacket[19] = 0;
  
  Serial.println("Sending..."); delay(10);
  rf95.send((uint8_t *)radiopacket, 20);

  Serial.println("Waiting for packet to complete..."); delay(10);
  rf95.waitPacketSent();
  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  Serial.println("Waiting for reply..."); delay(10);
  if (rf95.waitAvailableTimeout(1000))
  { 
    // Should be a reply message for us now   
    if (rf95.recv(buf, &len))
   {
      Serial.print("Got reply: ");
      Serial.println((char*)buf);
      if (strcmp(buf, "detonate") == 0){   
        //digitalWrite(LED, HIGH);
        //delay(2000);
        //digitalWrite(LED, LOW);
        groundState = 1;
        digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(250);                       // wait for a second
        digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
        delay(250);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(250);
        digitalWrite(LED_BUILTIN, LOW);
      }
      else {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(500);
        digitalWrite(LED_BUILTIN, LOW);    
      }
    }
    else
    {
      Serial.println("Receive failed");
    }
  }
  else
  {
    Serial.println("No reply, is there a listener around?");
  }
  delay(1000);
}

void drive(){
  grabCoords();
  float xVect = xInit - xCoord; float yVect = yInit - yCoord;
  float theta = 360.0*atan(xVect/yVect)/(2.0*3.14159265359);
  float dist = pow(10,6)*sqrt(pow(xVect,2)+pow(yVect,2))*.36;
  if(0>theta){
    theta = theta + 360;
  }
  if(1==k){
    Serial.println("Curr Long      Init Long    Curr Lat     Init Lat     Angle WRT N    Distance");
    k=k+1;
    forward();
    turn();
  }
  Serial.print(xCoord,6);Serial.print("    ");Serial.print(xInit,6);Serial.print("    ");
  Serial.print(yCoord,6);Serial.print("    ");Serial.print(yInit,6);Serial.print("    ");
  Serial.print(theta,4);Serial.print("     ");Serial.println(dist,4);
  /*
  if(20>dist){
    USS();
    //Serial.println(distance);
    checkOrientation();
    if(4>distance){
      while (4>distance){
        rampDownForward();
        rampUpBackward();
        backward();
        rampDownBackward();
        turn();
        USS();
        Serial.print("Object Detected!     ");Serial.println(distance);
      }
    rampUpForward();
    forward();
    }
    else{
      forward();
    }
  }
  else{
    solarDep();
  }
  */
  forward();
}
void USS(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  Serial.println(distance);
}
void solarDep(){
  solarServo.attach(2);
  for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      solarServo.write(pos);
      while(1){
      }
  }
}
void rampUpForward(){
  checkOrientation();
  if(1==vertOr){
    digitalWrite(MOTORA_IN1, LOW);
    digitalWrite(MOTORB_IN2, LOW);
    for (int i=0; i<255; i++) {
      analogWrite(MOTORA_IN2, i);
      analogWrite(MOTORB_IN1, i);
      delay(10);
    }
  }
  else{
    digitalWrite(MOTORA_IN2, LOW);
    digitalWrite(MOTORB_IN1, LOW);
    for (int i=0; i<255; i++) {
      analogWrite(MOTORA_IN1, i);
      analogWrite(MOTORB_IN2, i);
      delay(10);
    }
  }
}
void forward(){
  checkOrientation();
  if(1==vertOr){
    analogWrite(MOTORA_IN2, 254);
    analogWrite(MOTORB_IN1, 254);
    delay(500);
  }
  else{
    analogWrite(MOTORA_IN1, 254);
    analogWrite(MOTORB_IN2, 254);
    delay(500);
  }
}
void rampDownForward(){
  checkOrientation();
  if(1==vertOr){
    digitalWrite(MOTORA_IN1, LOW);
    digitalWrite(MOTORB_IN2, LOW);
    for (int i=255; i>=0; i--) {
      analogWrite(MOTORA_IN2, i);
      analogWrite(MOTORB_IN1, i);
      delay(10);
    }
  }
  else{
    digitalWrite(MOTORA_IN2, LOW);
    digitalWrite(MOTORB_IN1, LOW);
    for (int i=255; i>=0; i--) {
      analogWrite(MOTORA_IN1, i);
      analogWrite(MOTORB_IN2, i);
      delay(10);
    }
  }
}
void rampUpBackward(){
  checkOrientation();
  if(1==vertOr){
    digitalWrite(MOTORA_IN2, LOW);
    digitalWrite(MOTORB_IN1, LOW);
    for (int i=0; i<255; i++) {
      analogWrite(MOTORA_IN1, i);
      analogWrite(MOTORB_IN2, i);
      delay(10);
    }
  }
  else{
    digitalWrite(MOTORA_IN1, LOW);
    digitalWrite(MOTORB_IN2, LOW);
    for (int i=0; i<255; i++) {
      analogWrite(MOTORA_IN2, i);
      analogWrite(MOTORB_IN1, i);
      delay(10);
    }
  }
}
void backward(){
  checkOrientation();
  if(1==vertOr){
    analogWrite(MOTORA_IN1, 254);
    analogWrite(MOTORB_IN2, 254);
    delay(500);
  }
  else{
    analogWrite(MOTORA_IN2, 254);
    analogWrite(MOTORB_IN1, 254);
    delay(500);
  }
}
void rampDownBackward(){
  checkOrientation();
  if(1==vertOr){
    digitalWrite(MOTORA_IN2, LOW);
    digitalWrite(MOTORB_IN1, LOW);
    for (int i=255; i>=0; i--) {
      analogWrite(MOTORA_IN1, i);
      analogWrite(MOTORB_IN2, i);
      delay(10);
    }
  }
  else{
    digitalWrite(MOTORA_IN1, LOW);
    digitalWrite(MOTORB_IN2, LOW);
    for (int i=255; i>=0; i--) {
      analogWrite(MOTORA_IN2, i);
      analogWrite(MOTORB_IN1, i);
      delay(10);
    }
  }
}
void turn(){
  digitalWrite(MOTORA_IN1, LOW);
  digitalWrite(MOTORB_IN1, LOW);
  for (int i=0; i<120; i++) {
      analogWrite(MOTORA_IN2, i);
      analogWrite(MOTORB_IN2, i);
  }
  for (int i=120; i>=0; i--) {
    analogWrite(MOTORA_IN2, i);
    analogWrite(MOTORB_IN2, i);
  }
  forward();
  digitalWrite(MOTORA_IN2, LOW);
  digitalWrite(MOTORB_IN2, LOW);
  for (int i=0; i<120; i++) {
      analogWrite(MOTORA_IN1, i);
      analogWrite(MOTORB_IN1, i);
  }
  for (int i=120; i>=0; i--) {
    analogWrite(MOTORA_IN1, i);
    analogWrite(MOTORB_IN1, i);
  }
}
static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (Serial3.available())
      gps.encode(Serial3.read());
  } while (millis() - start < ms);
}

static void print_float(float val, float invalid, int len, int prec)
{
  if (val == invalid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartdelay(0);
}

void checkOrientation(){
  sensors_event_t event;
  accel.getEvent(&event);
  //Serial.println(event.acceleration.z);
  if(0<event.acceleration.z){
    vertOr = 1;
  }
  else{
    vertOr = -1;
  }
  //return vertOr;
}


