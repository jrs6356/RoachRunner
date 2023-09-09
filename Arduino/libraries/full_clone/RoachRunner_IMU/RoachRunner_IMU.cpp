#include <Arduino.h>
#include <SPI.h>
#include "RoachRunner_IMU.h"
#include "RoachRunner_MUX.h"
#include <Adafruit_Sensor.h>

IMU::IMU(int8_t xgcs, int8_t mcs, int8_t esdo, MUX* _mux, int32_t sensorID){
	mux = _mux;
	_csm = mcs;
	_csxg = xgcs;
	_esdo = esdo;
	_lsm9dso_sensorid_accel = sensorID + 1;
	//_lsm9dso_sensorid_mag = sensorID + 2;
	_lsm9dso_sensorid_gyro = sensorID + 3;
	_lsm9dso_sensorid_temp = sensorID + 4;
	_accelSensor = Sensor(this, &IMU::readAccel, &IMU::getAccelEvent, &IMU::getAccelSensor);
	_gyroSensor = Sensor(this, &IMU::readGyro, &IMU::getGyroEvent, &IMU::getGyroSensor);
	_tempSensor = Sensor(this, &IMU::readTemp, &IMU::getTempEvent, &IMU::getTempSensor);
}

bool IMU::begin() {
	SPI.begin();
	//if (!_magSensor.begin_SPI(_csm)) {
		//return false;
	//}

	write8(XGTYPE, LSM9DS1_REGISTER_CTRL_REG8, 0x05);

	delay(10);

	uint8_t id = read8(XGTYPE, LSM9DS1_REGISTER_WHO_AM_I_XG);
	//print8(id);
	//Serial.print("\n");
	//print8(LSM9DS1_XG_ID);
	//Serial.print("\n");
	
	if (id != LSM9DS1_XG_ID){
		return false;
	}

	write8(XGTYPE, LSM9DS1_REGISTER_CTRL_REG1_G, 0xC0); // on XYZ

	// Enable the accelerometer continous
	write8(XGTYPE, LSM9DS1_REGISTER_CTRL_REG5_XL, 0x38); // enable X Y and Z axis
	write8(XGTYPE, LSM9DS1_REGISTER_CTRL_REG6_XL, 0xC0); // 1 KHz out data rate, BW set by ODR, 408Hz anti-aliasing

	//_magSensor.setOperationMode(LIS3MDL_CONTINUOUSMODE);

	setupAccel(LSM9DS1_ACCELRANGE_2G);
	//setupMag(LSM9DS1_MAGGAIN_4GAUSS);
	setupGyro(LSM9DS1_GYROSCALE_245DPS);

	return true;
}

void IMU::read() {
	readAccel();
	readGyro();
	readTemp();
	//readMag();
}

void IMU::readMag() {
	_magSensor.read();
	magData.x = _magSensor.x;
	magData.y = _magSensor.y;
	magData.z = _magSensor.z;
}

void IMU::readAccel() {
	// Read the accelerometer
	byte buffer[6];
	readBuffer(XGTYPE, 0x80 | LSM9DS1_REGISTER_OUT_X_L_XL, 6, buffer);

	uint8_t xlo = buffer[0];
	int16_t xhi = buffer[1];
	uint8_t ylo = buffer[2];
	int16_t yhi = buffer[3];
	uint8_t zlo = buffer[4];
	int16_t zhi = buffer[5];

	// Shift values to create properly formed integer (low byte first)
	xhi <<= 8;
	xhi |= xlo;
	yhi <<= 8;
	yhi |= ylo;
	zhi <<= 8;
	zhi |= zlo;
	accelData.x = xhi;
	accelData.y = yhi;
	accelData.z = zhi;
}

void IMU::readGyro() {
	// Read gyro
	byte buffer[6];
	readBuffer(XGTYPE, 0x80 | LSM9DS1_REGISTER_OUT_X_L_G, 6, buffer);

	uint8_t xlo = buffer[0];
	int16_t xhi = buffer[1];
	uint8_t ylo = buffer[2];
	int16_t yhi = buffer[3];
	uint8_t zlo = buffer[4];
	int16_t zhi = buffer[5];

	// Shift values to create properly formed integer (low byte first)
	xhi <<= 8;
	xhi |= xlo;
	yhi <<= 8;
	yhi |= ylo;
	zhi <<= 8;
	zhi |= zlo;

	gyroData.x = xhi;
	gyroData.y = yhi;
	gyroData.z = zhi;
}

void IMU::readTemp() {
	// Read temp sensor
	byte buffer[2];
	readBuffer(XGTYPE, 0x80 | LSM9DS1_REGISTER_TEMP_OUT_L, 2, buffer);
	uint8_t xlo = buffer[0];
	int16_t xhi = buffer[1];

	xhi <<= 8;
	xhi |= xlo;

	temperature = xhi;
}

void IMU::setupAccel(lsm9ds1AccelRange_t range) {
	uint8_t reg = read8(XGTYPE, LSM9DS1_REGISTER_CTRL_REG6_XL);
	reg &= ~(0b00011000);
	reg |= range;
	// Serial.println("set range: ");
	write8(XGTYPE, LSM9DS1_REGISTER_CTRL_REG6_XL, reg);

	switch (range) {
		case LSM9DS1_ACCELRANGE_2G:
			_accel_mg_lsb = LSM9DS1_ACCEL_MG_LSB_2G;
			break;
		case LSM9DS1_ACCELRANGE_4G:
			_accel_mg_lsb = LSM9DS1_ACCEL_MG_LSB_4G;
			break;
		case LSM9DS1_ACCELRANGE_8G:
			_accel_mg_lsb = LSM9DS1_ACCEL_MG_LSB_8G;
			break;
		case LSM9DS1_ACCELRANGE_16G:
			_accel_mg_lsb = LSM9DS1_ACCEL_MG_LSB_16G;
			break;
	}
}

void IMU::setupMag(lsm9ds1MagGain_t gain) {
	switch (gain) {
		case LSM9DS1_MAGGAIN_4GAUSS:
			_magSensor.setRange(LIS3MDL_RANGE_4_GAUSS);
			break;
		case LSM9DS1_MAGGAIN_8GAUSS:
			_magSensor.setRange(LIS3MDL_RANGE_8_GAUSS);
			break;
		case LSM9DS1_MAGGAIN_12GAUSS:
			_magSensor.setRange(LIS3MDL_RANGE_12_GAUSS);
			break;
		case LSM9DS1_MAGGAIN_16GAUSS:
			_magSensor.setRange(LIS3MDL_RANGE_16_GAUSS);
			break;
	}
}

void IMU::setupGyro(lsm9ds1GyroScale_t scale) {
	uint8_t reg = read8(XGTYPE, LSM9DS1_REGISTER_CTRL_REG1_G);
	reg &= ~(0b00110000);
	reg |= scale;
	write8(XGTYPE, LSM9DS1_REGISTER_CTRL_REG1_G, reg);

	switch (scale) {
		case LSM9DS1_GYROSCALE_245DPS:
			_gyro_dps_digit = LSM9DS1_GYRO_DPS_DIGIT_245DPS;
			break;
		case LSM9DS1_GYROSCALE_500DPS:
			_gyro_dps_digit = LSM9DS1_GYRO_DPS_DIGIT_500DPS;
			break;
		case LSM9DS1_GYROSCALE_2000DPS:
			_gyro_dps_digit = LSM9DS1_GYRO_DPS_DIGIT_2000DPS;
			break;
	}
}

bool IMU::getEvent(sensors_event_t *accelEvent, sensors_event_t *magEvent, sensors_event_t *gyroEvent, sensors_event_t *tempEvent) {
	/* Grab new sensor reading and timestamp. */
	read();
	uint32_t timestamp = micros();

	/* Update appropriate sensor events. */
	if (accelEvent){
		getAccelEvent(accelEvent, timestamp);
	}
	//if (magEvent){
		//_magSensor.getEvent(magEvent);
	//}
	if (gyroEvent){
		getGyroEvent(gyroEvent, timestamp);
	}
	if (tempEvent){
		getTempEvent(tempEvent, timestamp);
	}
	return true;
}

void IMU::getSensor(sensor_t *accel, sensor_t *mag, sensor_t *gyro, sensor_t *temp) {
  /* Update appropriate sensor metadata. */
	if (accel){
		getAccelSensor(accel);
	}
	//if (mag){
		//_magSensor.getSensor(mag);
	//}
	if (gyro){
		getGyroSensor(gyro);
	}
	if (temp){
		getTempSensor(temp);
	}
}

void IMU::getAccelEvent(sensors_event_t *event, uint32_t timestamp) {
	memset(event, 0, sizeof(sensors_event_t));
	event->version = sizeof(sensors_event_t);
	event->sensor_id = _lsm9dso_sensorid_accel;
	event->type = SENSOR_TYPE_ACCELEROMETER;
	event->timestamp = timestamp;
	event->acceleration.x = accelData.x * _accel_mg_lsb;
	event->acceleration.x /= 1000;
	event->acceleration.x *= SENSORS_GRAVITY_STANDARD;
	event->acceleration.y = accelData.y * _accel_mg_lsb;
	event->acceleration.y /= 1000;
	event->acceleration.y *= SENSORS_GRAVITY_STANDARD;
	event->acceleration.z = accelData.z * _accel_mg_lsb;
	event->acceleration.z /= 1000;
	event->acceleration.z *= SENSORS_GRAVITY_STANDARD;
}

void IMU::getMagEvent(sensors_event_t *event, uint32_t timestamp) {
	_magSensor.getEvent(event);
}

void IMU::getGyroEvent(sensors_event_t *event, uint32_t timestamp) {
	memset(event, 0, sizeof(sensors_event_t));
	event->version = sizeof(sensors_event_t);
	event->sensor_id = _lsm9dso_sensorid_accel;
	event->type = SENSOR_TYPE_GYROSCOPE;
	event->timestamp = timestamp;
	event->gyro.x = gyroData.x * _gyro_dps_digit * SENSORS_DPS_TO_RADS;
	event->gyro.y = gyroData.y * _gyro_dps_digit * SENSORS_DPS_TO_RADS;
	event->gyro.z = gyroData.z * _gyro_dps_digit * SENSORS_DPS_TO_RADS;
}

void IMU::getTempEvent(sensors_event_t *event, uint32_t timestamp) {
	memset(event, 0, sizeof(sensors_event_t));
	event->version = sizeof(sensors_event_t);
	event->sensor_id = _lsm9dso_sensorid_temp;
	event->type = SENSOR_TYPE_AMBIENT_TEMPERATURE;
	event->timestamp = timestamp;
	// This is just a guess since the staring point (21C here) isn't documented :(
	event->temperature = 21.0 + (float)temperature / 8;
	// event->temperature /= LSM9DS1_TEMP_LSB_DEGREE_CELSIUS;
}

void IMU::getAccelSensor(sensor_t *sensor) {
	memset(sensor, 0, sizeof(sensor_t));
	strncpy(sensor->name, "LSM9DS1_A", sizeof(sensor->name) - 1);
	sensor->name[sizeof(sensor->name) - 1] = 0;
	sensor->version = 1;
	sensor->sensor_id = _lsm9dso_sensorid_accel;
	sensor->type = SENSOR_TYPE_ACCELEROMETER;
	sensor->min_delay = 0;
	sensor->max_value = 156.8;      // +16 g = 156.8 m/s^s
	sensor->min_value = -156.8;     // -16 g = 156.8 m/s^s
	sensor->resolution = 0.0005978; // 0.061 mg = 0.0005978 m/s^2
}

void IMU::getMagSensor(sensor_t *sensor) {
	_magSensor.getSensor(sensor);
}

void IMU::getGyroSensor(sensor_t *sensor) {
	memset(sensor, 0, sizeof(sensor_t));
	strncpy(sensor->name, "LSM9DS1_G", sizeof(sensor->name) - 1);
	sensor->name[sizeof(sensor->name) - 1] = 0;
	sensor->version = 1;
	sensor->sensor_id = _lsm9dso_sensorid_gyro;
	sensor->type = SENSOR_TYPE_GYROSCOPE;
	sensor->min_delay = 0;
	sensor->max_value = 34.91;             // +2000 dps = 34.906586 rad/s
	sensor->min_value = -34.91;            // "
	sensor->resolution = 0.00015271631375; // 8.75 mdps = 0.00015271631375 rad/s
}

void IMU::getTempSensor(sensor_t *sensor) {
	memset(sensor, 0, sizeof(sensor_t));
	strncpy(sensor->name, "LSM9DS1_T", sizeof(sensor->name) - 1);
	sensor->name[sizeof(sensor->name) - 1] = 0;
	sensor->version = 1;
	sensor->sensor_id = _lsm9dso_sensorid_temp;
	sensor->type = SENSOR_TYPE_AMBIENT_TEMPERATURE;
	sensor->min_delay = 0;
	sensor->max_value = 0.0;  // ToDo
	sensor->min_value = 0.0;  // ToDo
	sensor->resolution = 0.0; // ToDo
}

void IMU::write8(boolean type, byte reg, byte value) {
	byte address, _cs;

	if (type == MAGTYPE) {
		address = LSM9DS1_ADDRESS_MAG;
		_cs = _csm;
	} else {
		address = LSM9DS1_ADDRESS_ACCELGYRO;
		_cs = _csxg;
	}
	mux->on(_esdo);
	mux->on(_cs);
	//delay(1);
    SPI.beginTransaction(SPISettings(200000, MSBFIRST, SPI_MODE0));
    spixfer(reg & 0x7F); // write data
    spixfer(value);
    SPI.endTransaction();
    mux->off(_cs);
	mux->off(_esdo);
	//delay(1);
}

byte IMU::read8(boolean type, byte reg) {
	uint8_t value;
	readBuffer(type, reg, 1, &value);
	return value;
}

void IMU::print8(uint8_t _b){
	for(int i=0;i<8;++i){
		if((i>0)&&((i%4)==0)){
			Serial.print(" | ");
		}
		Serial.print(1&(_b>>(7-i)));
	}
}

byte IMU::readBuffer(boolean type, byte reg, byte len, uint8_t *buffer) {
	byte address, _cs;
	if (type == MAGTYPE) {
		address = LSM9DS1_ADDRESS_MAG;
		_cs = _csm;
	} else {
		address = LSM9DS1_ADDRESS_ACCELGYRO;
		_cs = _csxg;
	}

    SPI.beginTransaction(SPISettings(200000, MSBFIRST, SPI_MODE0));
	mux->on(_esdo);
    mux->on(_cs);
	//delay(1);
    spixfer(reg | 0x80); // readdata
    for (uint8_t i = 0; i < len; i++) {
		buffer[i] = spixfer(0);
    }
    SPI.endTransaction();
    mux->off(_cs);
	mux->off(_esdo);
	//delay(1);

	return len;
}

uint8_t IMU::spixfer(uint8_t data) {
	return SPI.transfer(data);
}
