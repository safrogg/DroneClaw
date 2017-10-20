/*
    DroneClaw copyright 2016
*/

#ifndef _MPU_
#define _MPU_

#define VECTOR_3D 3

#define X 0
#define Y 1
#define Z 2

#define COUNT 2000
#define ADDRESS 0x68
#define START_ADDRESS 0x3b

#define to_int(stream) stream.read() << 8 | stream.read()

/** The data from the MPU in a struct form */
class MPU {
  private:
    static long _offset_gyro[VECTOR_3D];
    static long _offset_accelerometer[VECTOR_3D];
    long _accelerometer[VECTOR_3D];
    int _gyro[VECTOR_3D];
    int _temp;
  public:
    /** Create the instance of this MPU struct with the values from the sensor */
    inline MPU() {
      Wire.beginTransmission(ADDRESS);
      Wire.write(START_ADDRESS);
      Wire.endTransmission();
      Wire.requestFrom(ADDRESS, 14);
      while (Wire.available() < 14);
      _accelerometer[X] = to_int(Wire);
      _accelerometer[Y] = to_int(Wire);
      _accelerometer[Z] = to_int(Wire);
      _temp = to_int(Wire);
      _gyro[X] = to_int(Wire);
      _gyro[Y] = to_int(Wire);
      _gyro[Z] = to_int(Wire);
    }
    /** Return a pointer of the calibrated accelerometer data */
    inline long* accelerometer() {
      _accelerometer[X] -= _offset_accelerometer[X];
      _accelerometer[Y] -= _offset_accelerometer[Y];
      _accelerometer[Z] -= _offset_accelerometer[Z];
      return _accelerometer;
    }
    /** Return a pointer of the raw accelerometer data */
    inline long* raw_accelerometer() {
      return _accelerometer;
    }
    /** Return a pointer of the raw gyro data */
    inline int* raw_gyro() {
      return _gyro;
    }
    /** Return a pointer of the calibrated gyro data */
    inline int* gyro() {
      _gyro[X] -= _offset_gyro[X];
      _gyro[Y] -= _offset_gyro[Y];
      _gyro[Z] -= _offset_gyro[Z];
      return _gyro;
    }
    /** Inits the Wire lib and the sensors */
    inline static void init() {
      Wire.begin();
      Wire.beginTransmission(ADDRESS);
      Wire.write(0x6b);
      Wire.write(0);
      Wire.endTransmission();
      Wire.beginTransmission(ADDRESS);
      Wire.write(0x1c);
      Wire.write(0x10);
      Wire.endTransmission();
      Wire.beginTransmission(ADDRESS);
      Wire.write(0x1b);
      Wire.write(0x08);
      Wire.endTransmission();
      Wire.beginTransmission(ADDRESS);
      Wire.write(0x1a);
      Wire.write(0x03);
      Wire.endTransmission();
    }
    /** Calibrate the gyro */
    inline static void calibrate() {
      for (int i = 0 ; i < COUNT; i++) {
        MPU mpu;
        _offset_gyro[X] += mpu._gyro[X];
        _offset_gyro[Y] += mpu._gyro[Y];
        _offset_gyro[Z] += mpu._gyro[Z];
        _offset_accelerometer[X] += mpu._accelerometer[X];
        _offset_accelerometer[Y] += mpu._accelerometer[Y];
        _offset_accelerometer[Z] += mpu._accelerometer[Z];
        delay(4); // Must be 4 to simulate clock cycle
      }
      _offset_gyro[X] /= COUNT;
      _offset_gyro[Y] /= COUNT;
      _offset_gyro[Z] /= COUNT;
      _offset_accelerometer[X] /= COUNT;
      _offset_accelerometer[Y] /= COUNT;
      _offset_accelerometer[Z] /= COUNT;
    }
};

long MPU::_offset_gyro[VECTOR_3D] = {};
long MPU::_offset_accelerometer[VECTOR_3D] = {};

#endif

