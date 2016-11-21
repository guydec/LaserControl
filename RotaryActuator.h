#pragma once

#include <Arduino.h>

#define DELAY_NOT_EXPIRED(S, D) ((millis()-S) < D)

class RotaryActuator {

public:
  enum class EButtonStatus {
    Unknown,
    Released,
    Pushed
  };

private:
  int _Pin1;
  int _Pin2;
  int _Pin_Button;

  int _MinValue;
  int _MaxValue;
  int _CurrentValue;
  int _LastFullValue;
  long _StartReadingButtonStatus;
  EButtonStatus _LastButtonStatus;
  bool _IsButtonStatusChanged;

  EButtonStatus _ButtonStatus;

  void Initialize(int pin1, int pin2, int pin_Button);
  void InitPinInputHigh(int pin);

  int IncreaseValue(int value);
  int DecreaseValue(int value);
  int Step;

public:
  long DebouceDelayForButton = 200;

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
  
  EButtonStatus GetButtonStatus(bool forceRead = false);
  void ResetButtonStatus();
  bool IsButtonStatusChanged();
  bool ButtonPushedThenReleased();

};

