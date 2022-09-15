#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>  // not used in this demo but required!

#define LSM9DS1_ADDRESS_ACCELGYRO (0x6B)
#define LSM9DS1_ADDRESS_MAG (0x1E)
#define LSM9DS1_XG_ID (0b01101000)
#define MAGTYPE (true)
#define XGTYPE (false)

#define LSM9DS1_SCK 52//A5
#define LSM9DS1_MISO 50////12
#define LSM9DS1_MOSI 51//A4
#define LSM9DS1_XGCS 53//6
#define LSM9DS1_MCS 22//5

class sauce {
public:
  sauce(int8_t clk, int8_t miso, int8_t mosi, int8_t xmcs, int8_t gcs, int32_t sensorID = 0){
    _i2c = false;
    // software SPI!
    _csm = mcs;
    _csxg = xgcs;
    _mosi = smosi;
    _miso = smiso;
    _clk = sclk;
    _lsm9dso_sensorid_accel = sensorID + 1;
    //_lsm9dso_sensorid_mag = sensorID + 2;
    _lsm9dso_sensorid_gyro = sensorID + 3;
    _lsm9dso_sensorid_temp = sensorID + 4;
    _accelSensor = Sensor(this, &Adafruit_LSM9DS1::readAccel, &Adafruit_LSM9DS1::getAccelEvent, &Adafruit_LSM9DS1::getAccelSensor);
    _gyroSensor = Sensor(this, &Adafruit_LSM9DS1::readGyro, &Adafruit_LSM9DS1::getGyroEvent, &Adafruit_LSM9DS1::getGyroSensor);
    _tempSensor = Sensor(this, &Adafruit_LSM9DS1::readTemp, &Adafruit_LSM9DS1::getTempEvent, &Adafruit_LSM9DS1::getTempSensor);
  }
  bool begin() {
    // Serial.println("softSPI");
    // Sofware SPI
    pinMode(_clk, OUTPUT);
    pinMode(_mosi, OUTPUT);
    pinMode(_miso, INPUT);
    pinMode(_csxg, OUTPUT);
    pinMode(_csm, OUTPUT);
    digitalWrite(_csxg, HIGH);
    digitalWrite(_csm, HIGH);
    digitalWrite(_clk, HIGH);
    if (!_magSensor.begin_SPI(_csm, _clk, _miso, _mosi)) { // soft SPI
      return false;
    }
    // soft reset & reboot accel/gyro
    write8(XGTYPE, LSM9DS1_REGISTER_CTRL_REG8, 0x05);
  
    delay(10);
  
    uint8_t id = read8(XGTYPE, LSM9DS1_REGISTER_WHO_AM_I_XG);
    if (id != LSM9DS1_XG_ID)
      return false;
  
    // enable gyro continuous
    write8(XGTYPE, LSM9DS1_REGISTER_CTRL_REG1_G, 0xC0); // on XYZ
  
    // Enable the accelerometer continous
    write8(XGTYPE, LSM9DS1_REGISTER_CTRL_REG5_XL, 0x38); // enable X Y and Z axis
    write8(XGTYPE, LSM9DS1_REGISTER_CTRL_REG6_XL,
           0xC0); // 1 KHz out data rate, BW set by ODR, 408Hz anti-aliasing
  
    // enable mag continuous
    _magSensor.setOperationMode(LIS3MDL_CONTINUOUSMODE);
  
    // Set default ranges for the various sensors
    setupAccel(LSM9DS1_ACCELRANGE_2G);
    setupMag(LSM9DS1_MAGGAIN_4GAUSS);
    setupGyro(LSM9DS1_GYROSCALE_245DPS);
  
    return true;
  }
}

Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1(LSM9DS1_SCK, LSM9DS1_MISO, LSM9DS1_MOSI, LSM9DS1_XGCS, LSM9DS1_MCS);

void setupSensor()
{
  // 1.) Set the accelerometer range
  lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_2G);
  //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_4G);
  //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_8G);
  //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_16G);
  
  // 2.) Set the magnetometer sensitivity
  lsm.setupMag(lsm.LSM9DS1_MAGGAIN_4GAUSS);
  //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_8GAUSS);
  //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_12GAUSS);
  //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_16GAUSS);

  // 3.) Setup the gyroscope
  lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_245DPS);
  //lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_500DPS);
  //lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_2000DPS);
}


void setup() 
{
  Serial.begin(115200);

  while (!Serial) {
    delay(1); // will pause Zero, Leonardo, etc until serial console opens
  }
  
  Serial.println("LSM9DS1 data read demo");
  
  // Try to initialise and warn if we couldn't detect the chip
  if (!lsm.begin())
  {
    Serial.println("Oops ... unable to initialize the LSM9DS1. Check your wiring!");
    while (1);
  }
  Serial.println("Found LSM9DS1 9DOF");

  // helper to just set the default scaling we want, see above!
  setupSensor();
}

void loop() 
{
  lsm.read();  /* ask it to read in the data */ 

  /* Get a new sensor event */ 
  sensors_event_t a, m, g, temp;

  lsm.getEvent(&a, &m, &g, &temp); 

  Serial.print("Accel X: "); Serial.print(a.acceleration.x); Serial.print(" m/s^2");
  Serial.print("\tY: "); Serial.print(a.acceleration.y);     Serial.print(" m/s^2 ");
  Serial.print("\tZ: "); Serial.print(a.acceleration.z);     Serial.println(" m/s^2 ");

  Serial.print("Mag X: "); Serial.print(m.magnetic.x);   Serial.print(" uT");
  Serial.print("\tY: "); Serial.print(m.magnetic.y);     Serial.print(" uT");
  Serial.print("\tZ: "); Serial.print(m.magnetic.z);     Serial.println(" uT");

  Serial.print("Gyro X: "); Serial.print(g.gyro.x);   Serial.print(" rad/s");
  Serial.print("\tY: "); Serial.print(g.gyro.y);      Serial.print(" rad/s");
  Serial.print("\tZ: "); Serial.print(g.gyro.z);      Serial.println(" rad/s");

  Serial.println();
  delay(200);
}

bool Adafruit_LSM9DS1::begin() {
  if (_i2c) {
    _wire->begin();
    if (!_magSensor.begin_I2C(LSM9DS1_ADDRESS_MAG, _wire)) {
      return false;
    }
  } else if (_clk == -1) {
    // Hardware SPI
    pinMode(_csxg, OUTPUT);
    pinMode(_csm, OUTPUT);
    digitalWrite(_csxg, HIGH);
    digitalWrite(_csm, HIGH);
    SPI.begin();
    if (!_magSensor.begin_SPI(_csm)) {
      return false;
    }
  } else {
    // Serial.println("softSPI");
    // Sofware SPI
    pinMode(_clk, OUTPUT);
    pinMode(_mosi, OUTPUT);
    pinMode(_miso, INPUT);
    pinMode(_csxg, OUTPUT);
    pinMode(_csm, OUTPUT);
    digitalWrite(_csxg, HIGH);
    digitalWrite(_csm, HIGH);
    digitalWrite(_clk, HIGH);
    if (!_magSensor.begin_SPI(_csm, _clk, _miso, _mosi)) { // soft SPI
      return false;
    }
  }

  // soft reset & reboot accel/gyro
  write8(XGTYPE, LSM9DS1_REGISTER_CTRL_REG8, 0x05);

  delay(10);

  uint8_t id = read8(XGTYPE, LSM9DS1_REGISTER_WHO_AM_I_XG);
  if (id != LSM9DS1_XG_ID)
    return false;

  // enable gyro continuous
  write8(XGTYPE, LSM9DS1_REGISTER_CTRL_REG1_G, 0xC0); // on XYZ

  // Enable the accelerometer continous
  write8(XGTYPE, LSM9DS1_REGISTER_CTRL_REG5_XL, 0x38); // enable X Y and Z axis
  write8(XGTYPE, LSM9DS1_REGISTER_CTRL_REG6_XL,
         0xC0); // 1 KHz out data rate, BW set by ODR, 408Hz anti-aliasing

  // enable mag continuous
  _magSensor.setOperationMode(LIS3MDL_CONTINUOUSMODE);

  // Set default ranges for the various sensors
  setupAccel(LSM9DS1_ACCELRANGE_2G);
  setupMag(LSM9DS1_MAGGAIN_4GAUSS);
  setupGyro(LSM9DS1_GYROSCALE_245DPS);

  return true;
}
