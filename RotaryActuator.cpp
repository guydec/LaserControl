#include "RotaryActuator.h"

#pragma region --- Constructors ---
RotaryActuator::RotaryActuator() {
  Initialize(10, 11, 12);
}
RotaryActuator::RotaryActuator(int pin1, int pin2, int pin_Button) {
  Initialize(pin1, pin2, pin_Button);
}
RotaryActuator::~RotaryActuator() {
}
#pragma endregion


void RotaryActuator::Initialize(int pin1, int pin2, int pin_Button) {
  Pin1 = pin1;
  Pin2 = pin2;
  Pin_Button = pin_Button;
  CurrentValue = 0;
  MinValue = 0;
  MaxValue = 100;
  InitPinInputHigh(Pin1);
  InitPinInputHigh(Pin2);
  InitPinInputHigh(Pin_Button);
  Step = 1;
}

void RotaryActuator::InitPinInputHigh(int pin) {
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH);
}

void RotaryActuator::SetMinValue(int value) {
  MinValue = value;
}

void RotaryActuator::SetMaxValue(int value) {
  MaxValue = value;
}

void RotaryActuator::SetCurrentValue(int value) {
  CurrentValue = value;
}

int RotaryActuator::GetMinValue() {
  return MinValue;
}
int RotaryActuator::GetMaxValue() {
  return MaxValue;
}

void RotaryActuator::SetStep(int value) {
  Step = value;
}

int RotaryActuator::GetStep() {
  return Step;
}


int RotaryActuator::GetCurrentValue() {

  int MSB = digitalRead(Pin1);                               // MSB = most significant bit
  int LSB = digitalRead(Pin2);                               // LSB = least significant bit
  int FullValue = (MSB << 1) | LSB;                                    // converting the 2 pin value to single number
  int sum = (LastFullValue << 2) | FullValue;                          // adding it to the previous encoded value

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
    CurrentValue = IncreaseValue(CurrentValue);
  }
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
    CurrentValue = DecreaseValue(CurrentValue);
  }

  LastFullValue = FullValue;                                            // store this value for next time

  return CurrentValue;

}

int RotaryActuator::IncreaseValue(int value) {
  Serial.print(">>");
  Serial.println(value);
  value += Step;
  if (value > MaxValue) {
    return MaxValue;
  }
  return value;
}

int RotaryActuator::DecreaseValue(int value) {
  Serial.print("<<");
  Serial.println(value);
  value -= Step;
  if (value < MinValue) {
    return MinValue;
  }
  return value;
}
