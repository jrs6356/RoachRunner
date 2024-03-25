#include <SPI.h>
#include "RoachRunner_MUX.h"
#include "RoachRunner_IMU.h"
//#include <Adafruit_LSM9DS1.h>
//#include <Adafruit_Sensor.h>  // not used in this demo but required!

#define LSM9DS1_XGCS 6
#define LSM9DS1_MCS 5

//Pins
byte rx0      = 0;
byte tx0      = 1;
byte gui_sw   = 2;
byte gui_b    = 3;
byte gui_a    = 4;
byte motor_b  = 5;
byte motor_a  = 6;
byte osc2     = 7;
byte esc_tx   = 8;
byte esc_pwm  = 9;
byte lcd2_rs  = 10;
byte lcd2_a0  = 11;
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

MUX mux(le,e_mux_0,e_mux_1,e_mux_2,e_mux_3,a0_0,a1_0,a2_0,a3_0,a0_1,a1_1,a2_1,a3_1);

// You can also use software SPI
//Adafruit_LSM9DS1 imu = Adafruit_LSM9DS1(LSM9DS1_SCK, LSM9DS1_MISO, LSM9DS1_MOSI, LSM9DS1_XGCS, LSM9DS1_MCS);
// Or hardware SPI! In this case, only CS pins are passed in
IMU imu = IMU(cs_ag, cs_m, e_sdo_imu, mux);


void setupSensor()
{
  // 1.) Set the accelerometer range
  imu.setupAccel(imu.LSM9DS1_ACCELRANGE_2G);
  //imu.setupAccel(imu.LSM9DS1_ACCELRANGE_4G);
  //imu.setupAccel(imu.LSM9DS1_ACCELRANGE_8G);
  //imu.setupAccel(imu.LSM9DS1_ACCELRANGE_16G);
  
  // 2.) Set the magnetometer sensitivity
  //imu.setupMag(imu.LSM9DS1_MAGGAIN_4GAUSS);
  //imu.setupMag(imu.LSM9DS1_MAGGAIN_8GAUSS);
  //imu.setupMag(imu.LSM9DS1_MAGGAIN_12GAUSS);
  //imu.setupMag(imu.LSM9DS1_MAGGAIN_16GAUSS);

  // 3.) Setup the gyroscope
  imu.setupGyro(imu.LSM9DS1_GYROSCALE_245DPS);
  //imu.setupGyro(imu.LSM9DS1_GYROSCALE_500DPS);
  //imu.setupGyro(imu.LSM9DS1_GYROSCALE_2000DPS);
}


void setup() 
{
  Serial.begin(250000);

  while (!Serial) {
    delay(1); // will pause Zero, Leonardo, etc until serial console opens
  }

  mux.init();
  pinMode(sdo_imu, INPUT);
  
  //Serial.println("LSM9DS1 data read demo");
  
  // Try to initialise and warn if we couldn't detect the chip
  if (!imu.begin())
  {
    //Serial.println("Oops ... unable to initialize the LSM9DS1. Check your wiring!");
    while (1);
  }
  //Serial.println("Found LSM9DS1 9DOF");

  // helper to just set the default scaling we want, see above!
  setupSensor();
  delay(10);
}

void loop(){

  accel();
  delay(50);
}

void accel(){
  imu.read();  /* ask it to read in the data */ 

  /* Get a new sensor event */ 
  sensors_event_t a, m, g, temp;

  imu.getEvent(&a, &m, &g, &temp); 

  Serial.print(a.timestamp);
  Serial.print(",");

  //Serial.print("Accel X: ");
  Serial.print(a.acceleration.x);
  Serial.print(",");
  //Serial.print(" m/s^2");
  
  //Serial.print("\tY: ");
  Serial.print(a.acceleration.y);
  Serial.print(",");
  //Serial.print(" m/s^2 ");
  
  //Serial.print("\tZ: ");
  Serial.println(a.acceleration.z);
  //Serial.println(" m/s^2 ");

  //Serial.print("Mag X: "); Serial.print(m.magnetic.x);   Serial.print(" uT");
  //Serial.print("\tY: "); Serial.print(m.magnetic.y);     Serial.print(" uT");
  //Serial.print("\tZ: "); Serial.print(m.magnetic.z);     Serial.println(" uT");

  //Serial.print("Gyro X: "); Serial.print(g.gyro.x);   Serial.print(" rad/s");
  //Serial.print("\tY: "); Serial.print(g.gyro.y);      Serial.print(" rad/s");
  //Serial.print("\tZ: "); Serial.print(g.gyro.z);      Serial.println(" rad/s");

  //Serial.println();
}
