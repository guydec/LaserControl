#pragma once

#include <OneWire.h>
#include <DallasTemperature.h>

class Thermometre {
private:
  OneWire* OneWireBus;
  DallasTemperature* sensors;
  DeviceAddress DS18Sensor;
  byte Resolution;
  byte DeviceId;

  void Initialize();

public:
  float Temperature;

  Thermometre();
  Thermometre(byte pin_oneWireBus, byte deviceId = 0);
  ~Thermometre();

  void SetResolution(byte resolution);
  byte GetResolution();
  void Thermometre::FindDevice(byte deviceId);
  float Thermometre::ReadTemperatureAsync();
};

