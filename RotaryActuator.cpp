#include "RotaryActuator.h"

#pragma region --- Constructors and initialization ---
RotaryActuator::RotaryActuator() {
  Initialize(10, 11, 12);
}
RotaryActuator::RotaryActuator(int pin1, int pin2, int pin_Button) {
  Initialize(pin1, pin2, pin_Button);
}
RotaryActuator::~RotaryActuator() {
}

void RotaryActuator::Initialize(int pin1, int pin2, int pin_Button) {
  _Pin1 = pin1;
  _Pin2 = pin2;
  _Pin_Button = pin_Button;
  _CurrentValue = 0;
  _MinValue = 0;
  _MaxValue = 100;
  InitPinInputHigh(_Pin1);
  InitPinInputHigh(_Pin2);
  InitPinInputHigh(_Pin_Button);
  Step = 1;
  _StartReadingButtonStatus = millis();
  _ButtonStatus = EButtonStatus::Unknown;
  _IsButtonStatusChanged = false;
}
void RotaryActuator::InitPinInputHigh(int pin) {
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH);
}
#pragma endregion

#pragma region --- Rotary encoder ---
void RotaryActuator::SetMinValue(int value) {
  _MinValue = value;
}
int RotaryActuator::GetMinValue() {
  return _MinValue;
}

void RotaryActuator::SetMaxValue(int value) {
  _MaxValue = value;
}
int RotaryActuator::GetMaxValue() {
  return _MaxValue;
}

void RotaryActuator::SetStep(int value) {
  if (value < 1) {
    return;
  }
  if (value > (_MaxValue - _MinValue)) {
    return;
  }
  Step = value;
}
int RotaryActuator::GetStep() {
  return Step;
}

void RotaryActuator::SetCurrentValue(int value) {
  if (value < _MinValue) {
    _CurrentValue = _MinValue;
    return;
  }
  if (value > _MaxValue) {
    _CurrentValue = _MaxValue;
    return;
  }
  _CurrentValue = value;
}
int RotaryActuator::GetCurrentValue() {

  int MSB = digitalRead(_Pin1);                               // MSB = most significant bit
  int LSB = digitalRead(_Pin2);                               // LSB = least significant bit
  int FullValue = (MSB << 1) | LSB;                                    // converting the 2 pin value to single number
  int sum = (_LastFullValue << 2) | FullValue;                          // adding it to the previous encoded value

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
    _CurrentValue = IncreaseValue(_CurrentValue);
  }
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
    _CurrentValue = DecreaseValue(_CurrentValue);
  }

  _LastFullValue = FullValue;                                            // store this value for next time

  return _CurrentValue;

}

int RotaryActuator::IncreaseValue(int value) {
  //Serial.print(">>");
  //Serial.println(value);
  value += Step;
  if (value > _MaxValue) {
    return _MaxValue;
  }
  return value;
}
int RotaryActuator::DecreaseValue(int value) {
  //Serial.print("<<");
  //Serial.println(value);
  value -= Step;
  if (value < _MinValue) {
    return _MinValue;
  }
  return value;
}
#pragma endregion

#pragma region --- Button ---
void RotaryActuator::ResetButtonStatus() {
  _ButtonStatus = RotaryActuator::EButtonStatus::Unknown;
}

RotaryActuator::EButtonStatus RotaryActuator::GetButtonStatus(bool forceRead = false) {

  //if (DELAY_NOT_EXPIRED(_StartReadingButtonStatus, DebouceDelayForButton)) {
  //  return _ButtonStatus;
  //}

  if (forceRead || _ButtonStatus == EButtonStatus::Unknown) {

    //Serial.print("Reading button status ");
    _StartReadingButtonStatus = millis();


    switch (digitalRead(_Pin_Button)) {
      case LOW:
        _ButtonStatus = EButtonStatus::Pushed;
        _IsButtonStatusChanged = (_LastButtonStatus != EButtonStatus::Pushed);
        break;
      case HIGH:
        _ButtonStatus = EButtonStatus::Released;
        _IsButtonStatusChanged = (_LastButtonStatus != EButtonStatus::Released);
        break;
    }

    //Serial.println(_ButtonStatus == EButtonStatus::Pushed ? "=> Pushed" : "=> Released");

    if (_IsButtonStatusChanged) {
      //Serial.println("Status has changed");
      _LastButtonStatus = _ButtonStatus;
    }
  }
  return _ButtonStatus;
}

bool RotaryActuator::IsButtonStatusChanged() {
  return (_IsButtonStatusChanged);
}

bool RotaryActuator::ButtonPushedThenReleased() {
  return (GetButtonStatus(true) == EButtonStatus::Released) && IsButtonStatusChanged();
}
#pragma endregion

