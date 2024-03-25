#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_10DOF.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>

Adafruit_10DOF                dof   = Adafruit_10DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_BMP085_Unified       bmp   = Adafruit_BMP085_Unified(18001);

TinyGPS gps;
SoftwareSerial ss(11, 3);

int commPort = 4;
int groundState = 0;
int deployState = 0;
int vertOr;
int counter = 0;

int trigPin = 8;
int echoPin = 7;

int MOTORA_IN1 = 9;
int MOTORA_IN2 = 10;
int MOTORB_IN1 = 5;
int MOTORB_IN2 = 6;

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
float xDest; float yDest;
float magTimeStart;

void setup() {
  pinMode(commPort,INPUT);
  pinMode(MOTORA_IN1, OUTPUT);
  pinMode(MOTORA_IN2, OUTPUT);
  pinMode(MOTORB_IN1, OUTPUT);
  pinMode(MOTORB_IN2, OUTPUT);
  #ifndef ESP8266
    while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
  #endif
  Serial.begin(115200);
  
  Serial.print("Testing TinyGPS library v. "); Serial.println(TinyGPS::library_version());
  Serial.println("by Mikal Hart");
  Serial.println();
  Serial.println("Sats HDOP Latitude  Longitude  Fix  Date       Time     Date Alt    Course Speed Card  Distance Course Card  Chars Sentences Checksum");
  Serial.println("          (deg)     (deg)      Age                      Age  (m)    --- from GPS ----  ---- to London  ----  RX    RX        Fail");
  Serial.println("-------------------------------------------------------------------------------------------------------------------------------------");

  ss.begin(57600);
  
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
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
    magOrient();
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
  groundState = digitalRead(commPort);
  Serial.println(groundState);
  if(1==groundState){
    deployState = 1;
  }
  else{
    deployState = 0;
  }
}
void deploy(){
  vertOr = checkOrientation();
  Serial.println(vertOr);
  if(0<vertOr){
    Serial.println("Forwards");
    if(0==counter){
      digitalWrite(MOTORA_IN1, LOW);
      digitalWrite(MOTORB_IN2, LOW);
      for (int i=0; i<255; i++) {
        analogWrite(MOTORA_IN2, i);
        analogWrite(MOTORB_IN1, i);
        delay(10);
      }
      counter = 1;
      analogWrite(MOTORA_IN2, 254);
      analogWrite(MOTORB_IN1, 254);
      delay(1000);
      for (int i=255; i>=0; i--) {
          analogWrite(MOTORA_IN2, i);
          analogWrite(MOTORB_IN1, i);
          delay(10);
    }
  }
  else{
    Serial.println("Backwards");
    if(0==counter){
      digitalWrite(MOTORA_IN2, LOW);
      digitalWrite(MOTORB_IN1, LOW);
      for (int i=0; i<255; i++) {
        analogWrite(MOTORA_IN1, i);
        analogWrite(MOTORB_IN2, i);
        delay(10);
      }
      counter = 1;
      analogWrite(MOTORA_IN1, 254);
      analogWrite(MOTORB_IN2, 254);
      delay(1000);
      for (int i=255; i>=0; i--) {
          analogWrite(MOTORA_IN1, i);
          analogWrite(MOTORB_IN2, i);
          delay(10);
      }
    }
  }
  //Serial.println("Boom!");
  }
  gpsCheck = 1;
  deployState = 0;
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
  xCoord = flon; yCoord = flat;
  if(0==k){
  Serial.print("Coordinates:     ");
  Serial.print(xCoord,6);Serial.print("   ");Serial.println(yCoord,6);
  yDest = yCoord + .00001;xDest = xCoord;
  Serial.println(yDest,6);
  k = k+1;
  }
}

void magOrient(){
  grabCoords();
  float xVect = xDest - xCoord; float yVect = yDest - yCoord;
  float theta = 360.0*atan(xVect/yVect)/(2.0*3.14159265359);
  if(0>theta){
    theta = theta + 360;
  }
  if(1==k){
    Serial.println("Curr Long      Dest Long    Curr Lat     Dest Lat     Angle To    Heading");
    k=k+1;
  }
  //Serial.print(xCoord,6);Serial.print("    ");Serial.print(xDest,6);Serial.print("    ");
  //Serial.print(yCoord,6);Serial.print("    ");Serial.print(yDest,6);Serial.print("    ");
  //Serial.print(pathTheta,4);Serial.print("     ");

  sensors_event_t mag_event;
  sensors_vec_t   orientation;
  mag.getEvent(&mag_event);
  if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation)){
    float magTime = millis();
    if(2==k){
      magTimeStart = millis();
      k=k+1;
    }
    float magRead = (magTime - magTimeStart)/1000.0;
    ti3 = ti2;  // u(K-2) = u(K-1)
    ti2 = ti1;  // u(K-1) = u(K)
    ti1 = orientation.heading;
    tnew = (1/a1)*(b1*ti1 + b2*ti2 + b3*ti3 - a2*tOld1 - a3*tOld2);
    tOld2 = tOld1;  // y(K-1) = y(K)
    tOld1 = tnew;   // y(K) = current output
    //Serial.print(F("Heading: "));
    Serial.print(magRead);Serial.print("    ");
    Serial.print(orientation.heading);Serial.print("    ");
    Serial.println(tnew);
    //Serial.println(F("; "));
  }
  float pathTheta = tnew - theta;
  Serial.println("---------------------------------");
  Serial.print(theta);Serial.print("    ");Serial.print(tnew);Serial.print("    ");Serial.println(pathTheta);
  Serial.println("---------------------------------");
  if((5<abs(pathTheta))^(355>abs(pathTheta))){
    if((180>abs(pathTheta))){
      digitalWrite(MOTORA_IN1, LOW);
      for (int i=0; i<255; i++) {
        analogWrite(MOTORA_IN2, i);
        delay(10);
      }
      while((3<abs(pathTheta))^(358>abs(pathTheta))){
        analogWrite(MOTORA_IN2, 254);
        sensors_event_t mag_event;
        sensors_vec_t   orientation;
        mag.getEvent(&mag_event);
        ti3 = ti2;  // u(K-2) = u(K-1)
        ti2 = ti1;  // u(K-1) = u(K)
        ti1 = orientation.heading;
        tnew = (1/a1)*(b1*ti1 + b2*ti2 + b3*ti3 - a2*tOld1 - a3*tOld2);
        tOld2 = tOld1;  // y(K-1) = y(K)
        tOld1 = tnew;
        float pathTheta = tnew - theta;
        Serial.println(pathTheta);
      }
      for (int i=255; i>=0; i--) {
          analogWrite(MOTORA_IN2, i);
          delay(10);
      }
    }
    else{
      digitalWrite(MOTORA_IN2, LOW);
      for (int i=0; i<255; i++) {
        analogWrite(MOTORA_IN1, i);
        delay(10);
      }
      while((3<abs(pathTheta))^(358>abs(pathTheta))){
        analogWrite(MOTORA_IN1, 254);
        sensors_event_t mag_event;
        sensors_vec_t   orientation;
        mag.getEvent(&mag_event);
        ti3 = ti2;  // u(K-2) = u(K-1)
        ti2 = ti1;  // u(K-1) = u(K)
        ti1 = orientation.heading;
        tnew = (1/a1)*(b1*ti1 + b2*ti2 + b3*ti3 - a2*tOld1 - a3*tOld2);
        tOld2 = tOld1;  // y(K-1) = y(K)
        tOld1 = tnew;
        float pathTheta = tnew - theta;
        Serial.println(pathTheta);
      }
      for (int i=255; i>=0; i--) {
          analogWrite(MOTORA_IN1, i);
          delay(10);
      }
    }
  }
  else{
    drive();
  }
  delay(100);
}

void drive(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  long distance = (duration/2) / 29.1;
  Serial.println(distance);
  checkOrientation();
  if (distance < 4){
    for (int i=255; i>=0; i--) {
        analogWrite(MOTORA_IN2, i);
        delay(10);
    }
    digitalWrite(MOTORA_IN2, LOW);
    for (int i=0; i<255; i++) {
      analogWrite(MOTORA_IN1, i);
      delay(10);
    }
    while (distance < 4) {
      delay(1000);
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH);
      distance = (duration/2) / 29.1;
      Serial.print("While        ");
      Serial.println(distance);
    }
    for (int i=255; i>=0; i--) {
      analogWrite(MOTORA_IN1, i);
      delay(10);
    }
    digitalWrite(MOTORA_IN1, LOW);
    for (int i=0; i<255; i++) {
      analogWrite(MOTORA_IN2, i);
      delay(10);
    }
  }
    analogWrite(MOTORA_IN2, 254);
    delay(100);
}

static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
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

int checkOrientation(){
  sensors_event_t event;
  accel.getEvent(&event);
  Serial.println(event.acceleration.z);
  if(0<event.acceleration.z){
    vertOr = 1;
  }
  else{
    vertOr = -1;
  }
  return vertOr;
}


