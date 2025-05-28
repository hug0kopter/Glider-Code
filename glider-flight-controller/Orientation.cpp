#include "Orientation.h"
#include "KalmanOrientation.h"
#include "Pins.h"

#define ORIENTATION_DISABLED false  // Set to true to disable IMU access

#if !ORIENTATION_DISABLED
#include <Wire.h>
#include <Adafruit_LSM6DS33.h>
#include <Adafruit_LIS3MDL.h>

Adafruit_LSM6DS33 lsm6ds33;
Adafruit_LIS3MDL lis3mdl;
KalmanOrientation kalman;
bool imuAvailable = false;
#endif

// Orientation state
unsigned long lastOrientationUpdate = 0;
unsigned long lastPrintTime = 0;
const unsigned long updateInterval = 5000;      // in microseconds (200 Hz)
const unsigned long printInterval = 1000000;    // in microseconds (1 Hz)

void initOrientation() {
#if ORIENTATION_DISABLED
  Serial.println("Orientation system disabled.");
  return;
#else
  Wire.begin();

  bool imuOK = lsm6ds33.begin_I2C();
  bool magOK = lis3mdl.begin_I2C();

  if (!imuOK || !magOK) {
    Serial.println("IMU or Magnetometer not detected. Fallback to dummy orientation.");
    imuAvailable = false;
    return;
  }

  imuAvailable = true;

  lsm6ds33.setAccelRange(LSM6DS_ACCEL_RANGE_2_G);
  lsm6ds33.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS);
  lsm6ds33.setAccelDataRate(LSM6DS_RATE_104_HZ);
  lsm6ds33.setGyroDataRate(LSM6DS_RATE_104_HZ);
  lis3mdl.setPerformanceMode(LIS3MDL_MEDIUMMODE);
  lis3mdl.setOperationMode(LIS3MDL_CONTINUOUSMODE);
  lis3mdl.setDataRate(LIS3MDL_DATARATE_155_HZ);
  lis3mdl.setRange(LIS3MDL_RANGE_4_GAUSS);

  kalman.begin();
#endif
}

void getOrientation(float& pitch, float& roll, float& yaw) {
#if ORIENTATION_DISABLED
  pitch = roll = yaw = 0.0f;
  return;
#else
  if (!imuAvailable) {
    pitch = roll = yaw = 0.0f;
    return;
  }

  static unsigned long lastMicros = micros();
  unsigned long now = micros();
  float dt = (now - lastMicros) / 1e6;
  lastMicros = now;

  sensors_event_t accel, gyro, mag;
  lsm6ds33.getEvent(&accel, &gyro, nullptr);
  lis3mdl.getEvent(&mag);

  kalman.update(accel.acceleration.x, accel.acceleration.y, accel.acceleration.z,
                gyro.gyro.x, gyro.gyro.y, gyro.gyro.z,
                mag.magnetic.x, mag.magnetic.y, mag.magnetic.z,
                dt);

  pitch = kalman.getPitch();
  roll  = kalman.getRoll();
  yaw   = kalman.getYaw();
#endif
}
