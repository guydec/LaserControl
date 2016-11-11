#pragma once

#include <OneWire.h>
#include <DallasTemperature.h>

class Thermometre {
private:
  OneWire* oneWire;
  DallasTemperature* sensors;
  DeviceAddress DS18sensor;
  byte Resolution;

public:
  float Temperature;

  Thermometre();
  Thermometre(byte pin_oneWireBus);
  ~Thermometre();

  void Initialize();
  void SetResolution(byte resolution);
  byte GetResolution();

};

