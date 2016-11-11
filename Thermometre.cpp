#include "Thermometre.h"

Thermometre::Thermometre() {
  oneWire = new OneWire(15);
  Initialize();
}
Thermometre::Thermometre(byte pin_oneWireBus) {
  oneWire = new OneWire(pin_oneWireBus);
  Initialize();
}
Thermometre::~Thermometre() {
  delete sensors;
  delete oneWire;
}
void Thermometre::Initialize() {
  Temperature = 0;
  sensors = new DallasTemperature(oneWire);
  sensors->begin();
  SetResolution(9);
  sensors->getAddress(DS18sensor, 0);
}

void Thermometre::SetResolution(byte resolution) {
  if (resolution < 9 || resolution>12) {
    return;
  }
  Resolution = resolution;

  // 9 bits	0.5°C	93.75 ms     
  // 10 bits	0.25°C	187.5 ms     
  // 11 bits	0.125°C	375 ms     
  // 12 bits 	0.0625°C	750 ms
  sensors->setResolution(Resolution);
}
byte Thermometre::GetResolution() {
  return Resolution;
}



