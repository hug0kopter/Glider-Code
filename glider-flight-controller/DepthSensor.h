#pragma once
void initDepthSensor();       // Call in setup
float getDepth();             // Get current depth in meters
float getRawPressure();       // Optional: pressure in mbar
float getTemperature();       // Optional: temp in °C
