#include <stdint.h>
#include <Arduino.h>

#include "ShiftRegister8.h"

#pragma once

// Implementation for 5461AS-1 four digit 7-segment display
class FourSegDisplay 
{
  public:

  FourSegDisplay(ShiftRegister8& dataShiftRegister, uint8_t pinDigit1, uint8_t pinDigit2 = 0, uint8_t pinDigit3 = 0, uint8_t pinDigit4 = 0) :
    m_DataShiftRegister(dataShiftRegister),
    m_PinDigit1(pinDigit1),
    m_PinDigit2(pinDigit2),
    m_PinDigit3(pinDigit3),
    m_PinDigit4(pinDigit4)
  {
    ResetDisplayedValues();
  }

  void Update()
  {
    microsToNextSwap -= (micros() - lastUpdateMicros);
    lastUpdateMicros = micros();

    if (microsToNextSwap < 0) {
      microsToNextSwap += DIGIT_SWAP_INTERVAL_MICROSECONDS;
      SwapDigit();
    }
  }

  void DisplayInt(int16_t value)
  {
    auto displayedValue = min(max(-999, value), 9999);
    bool sign = displayedValue < 0;

    ResetDisplayedValues();

    if (displayedValue == 0) {
      m_DigitValues[0] = ShiftRegister8::DIGIT_0;
      return;
    }

    auto i = 0;
    for ( ; i < 4; i++) {
      if (displayedValue == 0)
        break;

      m_DigitValues[i] = ShiftRegister8::IndexToDigit(abs(displayedValue) % 10);
      displayedValue /= 10;
      
    }

    if (sign)
      m_DigitValues[i] = ShiftRegister8::MINUS;
  }

  private:

  void ResetDisplayedValues()
  {
    m_DigitValues[0] = m_DigitValues[1] = m_DigitValues[2] = m_DigitValues[3] = 0;
  }

  void SwapDigit()
  {
    currentDigit = (currentDigit + 1) % 4;
    m_DataShiftRegister.PushValue(m_DigitValues[currentDigit]);
    for (auto i = 0; i < 4; i++) {
      SetDigitEnable(i, i == currentDigit);
    }
  }

  void SetDigitEnable(uint8_t digitIndex, bool enable) const
  {
    uint8_t pin = 0;
    switch (digitIndex) {
      case 0: pin = m_PinDigit1; break;
      case 1: pin = m_PinDigit2; break;
      case 2: pin = m_PinDigit3; break;
      case 3: pin = m_PinDigit4; break;
      default: break;
    }

    if (pin == 0)
      return;

    digitalWrite(pin, enable ? LOW : HIGH);
  }

  static constexpr uint16_t DIGIT_SWAP_INTERVAL_MICROSECONDS = 4000;

  ShiftRegister8& m_DataShiftRegister;
  uint8_t m_PinDigit1;
  uint8_t m_PinDigit2;
  uint8_t m_PinDigit3;
  uint8_t m_PinDigit4;

  uint8_t m_DigitValues[4];

  uint8_t currentDigit = 0;
  unsigned long lastUpdateMicros = 0;
  long microsToNextSwap = 0;

};