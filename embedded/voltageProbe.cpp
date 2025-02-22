#include <Arduino.h>

#include "constants.h"
#include "voltageProbe.h"

/**
 * Function from Arduino WMath.cpp modified to work for floating point number
**/
float VoltageProbe::map_f(double value, double fromLow, double fromHigh, double toLow, double toHigh) {
  return (value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;
}

/**
* Function to calculate battery voltage from resistor bridge voltages
* @param volt (int): Voltage reading from the resistor bridge
* @param batteryNumber (int): Battery Number
* @param prevSeriesLinkVoltage (float): Series Link Voltage until the previous Battery
* @return (float) Battery Voltage
**/
float VoltageProbe::calcVoltage(float volt, int batteryNumber, float prevSeriesLinkVoltage) {
  if (batteryNumber == BATTERY_01) {
    return map_f(volt, 1.91, 1.75, 12.20, 12.40);
  }
  if (batteryNumber == BATTERY_02) {
    return map_f(volt, 2.09, 2, 12.40, 11.77);
  }
  if (batteryNumber == BATTERY_03) {
    return map_f(volt, 2.13, 1.95, 11.77, 11.15);
  }
  if (batteryNumber == BATTERY_04) {
    return map_f(volt, 1.99, 2.87, 11.15, 11.73);
  }
  if (batteryNumber == BATTERY_05) {
    return map_f(volt, 2.13, 2.43, 11.73, 12.20);
  }
  if (batteryNumber == BATTERY_06) {
    return map_f(volt, 2.12, 2.86, 12.20, 11.80);
  }
  if (batteryNumber == BATTERY_07) {
    return map_f(volt, 2.02, 1.98, 11.80, 12.17);
  }
  if (batteryNumber == BATTERY_08) {
    return map_f(volt, 1, 1.75, 12.17, 11.87);
  }
  if (batteryNumber == BATTERY_09) {
    return map_f(volt, 2.13, 2.86, 11.87, 12.01);
  }
  if (batteryNumber == BATTERY_10) {
    return map_f(volt, 0.82, 2.43, 12.01, 12.53);
  }
  if (batteryNumber == BATTERY_11) {
    return map_f(volt, 2.84, 2.58, 12.53, 12.47);
  }
  if (batteryNumber == BATTERY_12) {
    return map_f(volt, 2.39, 2.42, 12.47, 12.62);
  }
  if (batteryNumber == BATTERY_13) {
    return map_f(volt, 2.66, 2.7, 12.62, 13.86);
  }
  if (batteryNumber == BATTERY_14) {
    return map_f(volt, 3.04, 3.07, 12.68, 13.86);
  }
  if (batteryNumber == BATTERY_15) {
    return map_f(volt, 3.17, 3.29, 10.02, 13.97);
  }
  if (batteryNumber == BATTERY_16) {
    return map_f(volt, 2.19, 2.23, 12.09, 14.05);
  }
  if (batteryNumber == BATTERY_17) {
    return map_f(volt, 2.86, 2.95, 10.01, 14.01);
  }
  return -1;
}

/**
 * Get raw voltage reading from Resistor Bridge
 * @param batteryNumber (int): Battery Index
 * @return (int) raw ADC Battery Voltage Reading
 **/
float VoltageProbe::readVoltage(int batteryNumber) {
  if (batteryNumber > BATTERY_17 || batteryNumber < BATTERY_01) {
    return 0;
  }

  else if (batteryNumber == BATTERY_17) {
    return sampleAverage(READ_BATTERY_17_PIN);
  }

  setMux(batteryNumber);
  return sampleAverage(READ_MUX);
}


/** 
 * Set the Mux Select line to the channel
 * @param line2Select(int): Channel to select
 * @return (int): -1 on error, 0 otherwise
 **/
int VoltageProbe::setMux(int line2Select) {
  if (line2Select < 0 || line2Select > 15) {
    return -1;
  }

  Serial.print("Binary bits: ");
  Serial.print(!bool(int(line2Select % 2)));
  Serial.print(!bool(int(line2Select / 2) % 2));
  Serial.print(!bool(int(line2Select / 4) % 2));
  Serial.println(!bool(int(line2Select / 8) % 2));

  digitalWrite(selectLines[0], !bool(line2Select % 2));
  digitalWrite(selectLines[1], !bool(int(line2Select / 2) % 2));
  digitalWrite(selectLines[2], !bool(int(line2Select / 4) % 2));
  digitalWrite(selectLines[3], !bool(int(line2Select / 8) % 2));

  // Delay to switch the MUX and establish connection
  delay(1000);

  return 0;
}

/**
 * Sample the ADC Channel NO_SAMPLE times and average it out
 * @param pinNumber (int): ADC Pin to Channel
 * @return (float): Averaged out ADC Value
 **/
float VoltageProbe::sampleAverage(int pinNumber){
  long int sum = 0;
  for(int x = 0;x < NO_SAMPLES; x++){
    sum += analogRead(pinNumber);
    delay(1);
  }

  return sum / NO_SAMPLES;
}