#pragma once

#include <Arduino.h>

class RotaryActuator {

private:
  int MinValue;
  int MaxValue;
  int CurrentValue;
  int LastFullValue;

  bool ButtonPushed;

  void Initialize(int pin1, int pin2, int pin_Button);
  void InitPinInputHigh(int pin);

  int IncreaseValue(int value);
  int DecreaseValue(int value);
  int Step;

public:
  int Pin1;
  int Pin2;
  int Pin_Button;

  RotaryActuator();
  RotaryActuator(int pin1, int pin2, int pin_Button);
  ~RotaryActuator();

  void SetMinValue(int value);
  int GetMinValue();
  void SetMaxValue(int value);
  int GetMaxValue();
  void SetCurrentValue(int value);
  int GetCurrentValue();

  void SetStep(int value);
  int GetStep();
  
  //bool IsButtonPushed(); // Changed from up to down
  //bool IsButtonReleased(); // Changed from down to up
  //bool ButtonStatus();  // true = up, false = down
  //bool ResetButtonPushed(); // Reset status to up

};

