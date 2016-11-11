#pragma once

#include <OneWire.h>
#include <DallasTemperature.h>

class Thermometre {
public:
  Thermometre();
  Thermometre(byte pin_oneWireBus);
  ~Thermometre();
  float Temperature;
  byte Resolution;

  void Initialize();
  void SetResolution(byte resolution);
  byte GetResolution();

};

