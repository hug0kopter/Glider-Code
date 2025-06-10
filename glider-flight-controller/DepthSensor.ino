#include "DepthSensor.h"
#include <Wire.h>
#include "KellerLD.h"
#include "Pins.h"

TwoWire depthWire = TwoWire(1);  // Use second I2C bus
KellerLD depthSensor;

static float currentDepth = -1.0;
static float currentPressure = -1.0;
static float currentTemp = -1.0;

void initDepthSensor() {
  depthWire.begin(DEPTH_SDA, DEPTH_SCL);
  if (!depthSensor.init(&depthWire)) {
    Serial.println("[ERROR] Failed to init KellerLD sensor!");
    return;
  }
  depthSensor.setFluidDensity(1029);  // Seawater density in kg/m^3
  Serial.println("[INFO] KellerLD sensor initialized.");
}

void updateDepthReading() {
  depthSensor.read();
  currentDepth = depthSensor.depth();
  currentPressure = depthSensor.pressure();
  currentTemp = depthSensor.temperature();
}

float getDepth() {
  updateDepthReading();
  return currentDepth;
}

float getRawPressure() {
  return currentPressure;
}

float getTemperature() {
  return currentTemp;
}
