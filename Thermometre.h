#pragma once

#include <OneWire.h>
#include <DallasTemperature.h>

class Thermometre {
private:
  OneWire* OneWireBus;
  DallasTemperature* Dallas;
  DeviceAddress DS18Sensor;
  byte Resolution;
  byte DeviceId;

  bool ReadingTemperature;

  void Initialize();

public:
  float Temperature;
  volatile bool DataReady;

  Thermometre();
  Thermometre(byte pin_oneWireBus, byte deviceId = 0);
  ~Thermometre();

  void SetResolution(byte resolution);
  byte GetResolution();
  void Thermometre::ReadTemperatureAsync();
};

