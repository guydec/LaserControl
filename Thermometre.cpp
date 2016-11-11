#include "Thermometre.h"

Thermometre::Thermometre() {
  OneWireBus = new OneWire(15);
  DeviceId = 0;
  Initialize();
}
Thermometre::Thermometre(byte pin_oneWireBus, byte deviceId = 0) {
  OneWireBus = new OneWire(pin_oneWireBus);
  DeviceId = deviceId;
  Initialize();
}
Thermometre::~Thermometre() {
  delete sensors;
  delete OneWireBus;
}

void Thermometre::Initialize() {
  Temperature = 0;
  sensors = new DallasTemperature(OneWireBus);
  sensors->begin();
  SetResolution(9);
  
  sensors->getAddress(DS18Sensor, DeviceId);
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

void Thermometre::FindDevice(byte deviceId) {

}
float Thermometre::ReadTemperatureAsync() {
}

