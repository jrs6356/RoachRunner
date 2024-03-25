
#include <Adafruit_Sensor.h>
#include <Adafruit_10DOF.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_LSM303_U.h>
#include <Wire.h>

Adafruit_LSM303_Accel_Unified accel;
sensors_event_t accelEvent;

void setup(){
  accel.begin();
  Serial.begin(9600);
}
void loop(){
  accel.getEvent(&accelEvent);
  Serial.println(accelEvent.acceleration.z);
}

