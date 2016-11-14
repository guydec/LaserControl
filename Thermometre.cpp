#include "Thermometre.h"

#pragma region Constructors
Thermometre::Thermometre() {
  Serial.write("Building thermometre\n");
  OneWireBus = new OneWire(13);
  DeviceId = 0;
  Initialize();
}
Thermometre::Thermometre(byte pin_oneWireBus, byte deviceId) {
  Serial.write("Building thermometre with params\n");
  OneWireBus = new OneWire(pin_oneWireBus);
  DeviceId = deviceId;
  Initialize();
}
Thermometre::~Thermometre() {
  delete Dallas;
  delete OneWireBus;
}
#pragma endregion

void Thermometre::Initialize() {
  Serial.write("Initialize thermometre\n");
  ReadingTemperature = false;
  Temperature = 0;
  Dallas = new DallasTemperature(OneWireBus);
  Dallas->begin();
  Dallas->getAddress(DS18Sensor, DeviceId);
  SetResolution(9);
  DataReady = false;
}

void Thermometre::SetResolution(byte resolution) {
  if (resolution < 9 || resolution>12) {
    return;
  }
  Resolution = resolution;
  switch (Resolution) {
    case 9: DelayToHaveValidData = 94; break;
    case 10: DelayToHaveValidData = 188; break;
    case 11: DelayToHaveValidData = 375; break;
    case 12: DelayToHaveValidData = 750; break;
  }
  // 9 bits	0.5°C	93.75 ms     
  // 10 bits	0.25°C	187.5 ms     
  // 11 bits	0.125°C	375 ms     
  // 12 bits 	0.0625°C	750 ms
  Dallas->setResolution(DS18Sensor, Resolution);
}
byte Thermometre::GetResolution() {
  return Resolution;
}

void Thermometre::ReadTemperatureAsync() {

  if (ReadingTemperature) {
    if ((millis() - StartReading) < DelayToHaveValidData) {
      return;
    }
    if (Dallas->isConversionAvailable(DS18Sensor)) {
      OldTemperature = Temperature;
      Temperature = Dallas->getTempC(DS18Sensor);
      if (OldTemperature != Temperature) {
        DataChanged = true;
      }
      //Serial.println(Temperature);
      DataReady = true;
      ReadingTemperature = false;
    }
    return;
  }

  ReadingTemperature = true;
  //Serial.write("Start read async\n");
  Dallas->setWaitForConversion(false);
  Dallas->setCheckForConversion(true);
  Dallas->requestTemperaturesByAddress(DS18Sensor);
  StartReading = millis();
  DataReady = false;
  return;
}

