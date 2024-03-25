//ToC
//setup
//loop
//counting: communication tracking
//killSequence: for chute deployed in air
//landed: after kiwi is done flying (ensure chute deployed, turn on buzzer, send final gps loc)
//manuvering: NOT DONE
//preprocessing: NOT DONE

//GPS: LS23060 version 10
//GSsig values: 911-kill, 2-no para, 3-recieved final location
#include <TinyGPS.h>
#include <SoftwareSerial.h>
#include <XBee.h>
#include <Servo.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_10DOF.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_LSM303_U.h>
#include <Wire.h>
#include <math.h>

#define para 7
#define buzzer 13
#define MAX_DEFLECT 20
#define pi 3.14159
#define SEA_LEVEL 1013.25
#define propPin 8
#define light_Hi 350    // value in full light <------
#define light_Lo 675
#define photoRes_Pin A1

class Vector {
public:
  long x;
  long y;
};

Adafruit_LSM303_Accel_Unified accel;
Adafruit_BMP085_Unified baro;
Adafruit_LSM303_Mag_Unified mag;
sensors_event_t sensorEvent;
Vector pos1;
Vector pos2;
Vector currentPos;
Vector dest;
int count; //Keeping track of time w/o signal before killing Kiwi
double weight;
double error;
double oldError;
int mappedError;
int altitude;
Servo rudder;
bool newData;
long GSsig;
long fix_age;
int propRun;
int light;

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
// create reusable response objects for responses we expect to handle
ZBRxResponse rx = ZBRxResponse();
ZBRxResponse rx2 = ZBRxResponse();
//ModemStatusResponse msr = ModemStatusResponse();

XBeeAddress64 address;
XBeeAddress64 senderAddr = XBeeAddress64(0x0013A200, 0x416390E8);


// Define NewSoftSerial TX/RX pins
// Connect Arduino pin 10 to TX of usb-serial device
uint8_t ssRX = 10;
// Connect Arduino pin 11 to RX of usb-serial device
uint8_t ssTX = 11;
// Remember to connect all devices to a common Ground: XBee, Arduino and USB-Serial device
SoftwareSerial nss(ssRX, ssTX);

long lon;
long lat;

TinyGPS gps;


void setup() {
  Serial.begin(57600);
  Serial.println("TEST");
  Serial.println("Serial begun");
  //accel.begin();
  //Serial.println("Accel begun");
  //baro.begin();
  //Serial.println("Baro begun");
  //mag.begin();
  //Serial.println("Mag begun");
  
  //pin setup
  pinMode(para, OUTPUT);
  pinMode(buzzer, OUTPUT);

  digitalWrite(para, HIGH);
  digitalWrite(buzzer, LOW); 
  rudder.attach(3);
  count = 1;
  //manuvering intial values
  oldError = 180;
  weight = 1;
  propRun = 0;
  light = 0;
  xbee.setSerial(nss);
  nss.begin(9600);
  lon = 0;
  lat = 0;

  Serial.println("Setup");
  newData = false;
  digitalWrite(propPin, HIGH);
  getCoord(); //receive GPS coordinates of destination from ground station
  //sunSpot();
  dest.x = 0; //hardcoded coordinates for testing
  dest.y = 0;
  Serial.println(dest.x);
  Serial.println(dest.y);
  GSsig = 0;
  Serial.println("Setup done");
}

void loop() {
  //Serial.println("Loop");
  getGSsig();
  Serial.println(GSsig);
  if (GSsig == 911) {
    killSequence();
    Serial.println("die");
    exit(0);
  }
  else if (GSsig != 100) {
    counting(false);
  }
  else if (GSsig == 100) {
    counting(false);
  }
  /*if (Serial.available()) { //if Kiwi's xbee is working
    int GSsig = Serial.read();
    if (GSsig != 911 && GSsig !=2 && GSsig !=3) { //not recieving from GS
      counting(0);
    } else {
      counting(true);
      if (GSsig == 911) { //GS sends kill signal
        killSequence();
      } else if (GSsig == 2) { //GS sends land w/o chute signal
        digitalWrite(para, LOW);
      }
    }
  } else { //Kiwi's xbee is not working
    counting(false);
  }*/

  //call IMU/GPS preprocessing, find altitude
  /*getPos(&pos1);
  Serial.print(pos1.x);
  Serial.print(", ");
  Serial.println(pos1.y);
  unsigned long start = millis();
  while (millis() - start < 500)
    ;
  getPos(&pos2);
  Serial.print(pos2.x);
  Serial.print(", ");
  Serial.println(pos2.y);
  updateError(pos1, pos2, &error);
  mappedError = map(error, -180, 180, 45, 135);
  rudder.write(mappedError);

  //CURRENT POS, TWO OTHER POS, ALTITUDE
  Serial.println("Before getEvent");
  //accel.getEvent(&sensorEvent);
  //baro.getEvent(&sensorEvent);
  Serial.println("After getEvent");
  altitude = int(baro.pressureToAltitude(SEA_LEVEL, sensorEvent.pressure));
  if (altitude <= 500 && GSsig != 2) { //check if altitude is low enough and must land with parachute
    killSequence();
  }

  //send GPS loc, velocity
  GSsig = -1;*/
}//end loop

void counting(bool connection) {
  // n represents the number of times the loop runs w/o signal before issuing the kill command
  int n = 1000;
  //boolean "i" for radio connection 1 = yes, 0 = no
  if (!connection) {
    count++;
    if (count = n) {
      killSequence();
    }
  }
  else if (connection) {
    count = 0;
  }
}//end counting
//GPS SHIT
void getPos(Vector* currentPos) {
  for (unsigned long start = millis(); millis() - start < 10;)
  {
    while (Serial.available())
    {
      char c = Serial.read();
      //Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }
  gps.get_position(&lat, &lon, &fix_age);
  currentPos->x = lon;
  currentPos->y = lat;
  newData = false;
}
//END GPS
//MANUVERING SHENANIGANS
Vector makeVector(double tx, double ty) {
  Vector v;
  v.x = tx;
  v.y = ty;
  return v;
}
void calculateVector(Vector start, Vector finish, Vector* result) { //Calculates vector between two given points (aka direction of motion)
  result->x = finish.x - start.x;
  result->y = finish.y - start.y;
}

double calculateError(Vector* velVector, Vector* posVector) { //Calculate degree difference between current velocity vector and position vector to destination
  double velDegree = atan2(double(velVector->y), double(velVector->x)) * 57.3;
  double posDegree = atan2(double(posVector->y), double(posVector->x)) * 57.3;
  return posDegree - velDegree;
}

void updateError(Vector pos1, Vector pos2, double* error) {
  Vector velocityVector;
  Vector positionVector;
  calculateVector(pos1, pos2, &velocityVector);
  calculateVector(pos2, dest, &positionVector);
  *error = calculateError(&velocityVector, &positionVector);
  if (abs(int(*error)) >= abs(int(oldError))) {
    weight = weight * 1.1;
  }
  else {
    weight = 1;
  }
  *error = *error * weight;
  oldError = *error;
}
//END MANEUVERING
//COMMUNICATIONS SHIT
void getCoord() {
  while (lon == 0 || lat == 0) {
    xbee.readPacket();
    
    if (xbee.getResponse().isAvailable()) {
      // got something
      Serial.println("Packet available");     
      if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
        // got a zb rx packet
        
        // now fill our zb rx class
        xbee.getResponse().getZBRxResponse(rx);
      
        Serial.println("Got an rx packet!");
        address = rx.getRemoteAddress64() ;
        if (address == senderAddr) {
          Serial.println("Got address");
            
          if (rx.getOption() == ZB_PACKET_ACKNOWLEDGED) {
              // the sender got an ACK
              Serial.println("packet acknowledged");
          } else {
            Serial.println("packet not acknowledged");
          }
          
          Serial.println("Got it");     
  
          for (int i = 0; i < rx.getDataLength(); i++) {
            if (i < 8) {
              lon = lon * 10 + (rx.getData()[i]);
            }
            else {
              lat = lat * 10 + (rx.getData()[i]);
            }
          }
          //Serial.println(lon);
          //Serial.println(lat);  
        }
      }
    } else if (xbee.getResponse().isError()) {
      nss.print("error code:");
      nss.println(xbee.getResponse().getErrorCode());
    }
  }
  dest.x = lon;
  Serial.println(dest.x);
  dest.y = lat;
  Serial.println(dest.y);
}

void getGSsig() {
  xbee.readPacket();
    
  if (xbee.getResponse().isAvailable()) {
    // got something
    Serial.println("Packet available");     
    if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
      // got a zb rx packet
      
      // now fill our zb rx class
      xbee.getResponse().getZBRxResponse(rx);
    
      Serial.println("Got an rx packet!");
      address = rx.getRemoteAddress64() ;
      if (address == senderAddr) {
        Serial.println("Got address");
          
        if (rx.getOption() == ZB_PACKET_ACKNOWLEDGED) {
            // the sender got an ACK
            Serial.println("packet acknowledged");
        } else {
          Serial.println("packet not acknowledged");
        }
        
        Serial.println("Got it");     
        GSsig = 0;
        for (int i = 0; i < rx.getDataLength(); i++) {
          GSsig = GSsig * 10 + (rx.getData()[i]);
        }
      }
    }
  } else if (xbee.getResponse().isError()) {
    nss.print("error code:");
    nss.println(xbee.getResponse().getErrorCode());
  }
}

//END COMMUNICATIONS

//KILL
void killSequence() {
  rudder.write(0);
  rudder.detach();
  digitalWrite(para, HIGH);
  delay(1000);
  digitalWrite(para, LOW);
  //xbee.write("Kill Code Recieved");
  delay(2000);
  digitalWrite(buzzer, HIGH);
  //while (1) {
    //if (velocity == 0) {
    digitalWrite(buzzer, HIGH);
    //}
  //}
  Serial.println("dead");
}//end kill sequence

//LANDED
void landed() {
  rudder.detach();
  digitalWrite(para, HIGH);
  delay(1000);
  digitalWrite(para, LOW);
  digitalWrite(buzzer, HIGH);
  //recieved = 0;
  /*while (recieved != 3) {
    xbee.write("final GPS Loc: " + gps_location);
    recieved = xbee.read();
  }*/
}//end landed

//Sunspot
void sunSpot(){
  while(propRun == 0){
    int photo = analogRead(photoRes_Pin);
    if(light==0){                        // kiwi turned on, net yet inside rocket
      if(photo>light_Hi){                     // kiwi identifies shift from light to darkness
        light = 1;
      }
    }
    else if(light==1){
      if(photo<light_Lo){                     // kiwi reading darkness inside rocket, waiting to be ejected
        light = 2;                       // kiwi identifies shift from dark to light
      }
    }  
    else if(light==2){                   // kiwi determines it is outside of the rocket
      delay(3000);
      propRun = 1;                       // kiwi gives prop function permission to run
    }                                    // function that determines which phase kiwi is in
    if(propRun==1){                 // kiwi withholds permission for prop to run until outside of rocket
      digitalWrite(propPin, HIGH);
    }
  }
}
