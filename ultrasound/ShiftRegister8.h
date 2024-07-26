#include <stdint.h>
#include <Arduino.h>

#pragma once

// Implementation for TI SN74HC595N 8-bit bit shift register
class ShiftRegister8 
{
  public:

  // bit layout from most to least significant:
  // A, B, C, D, E, F, G, dp
  static constexpr uint8_t DIGIT_0 = 0b11111100;
  static constexpr uint8_t DIGIT_1 = 0b01100000;
  static constexpr uint8_t DIGIT_2 = 0b11011010;
  static constexpr uint8_t DIGIT_3 = 0b11110010;
  static constexpr uint8_t DIGIT_4 = 0b01100110;
  static constexpr uint8_t DIGIT_5 = 0b10110110;
  static constexpr uint8_t DIGIT_6 = 0b10111110;
  static constexpr uint8_t DIGIT_7 = 0b11100000;
  static constexpr uint8_t DIGIT_8 = 0b11111110;
  static constexpr uint8_t DIGIT_9 = 0b11100110;
  static constexpr uint8_t MINUS   = 0b00000010;
  static constexpr uint8_t DECIMAL_POINT = 0b00000001;

  static uint8_t IndexToDigit(uint8_t index)
  {
    switch (index) {
      case 0: return DIGIT_0;
      case 1: return DIGIT_1;
      case 2: return DIGIT_2;
      case 3: return DIGIT_3;
      case 4: return DIGIT_4;
      case 5: return DIGIT_5;
      case 6: return DIGIT_6;
      case 7: return DIGIT_7;
      case 8: return DIGIT_8;
      case 9: return DIGIT_9;
      default: return 0;
    }
  }

  ShiftRegister8(uint8_t pinOutputEnable, uint8_t pinSerial, uint8_t pinShiftRegisterClock, uint8_t pinStorageRegisterClock, uint8_t pinClear) :
    m_PinOutputEnable(pinOutputEnable),
    m_PinSerial(pinSerial),
    m_PinShiftRegisterClock(pinShiftRegisterClock),
    m_PinStorageRegisterClock(pinStorageRegisterClock),
    m_PinClear(pinClear)
  {

  }

  void SetEnable(const bool enable) const
  {
    // output enable pin is inverted
    digitalWrite(m_PinOutputEnable, enable ? LOW : HIGH);
  }

  void Clear() const
  {
    digitalWrite(m_PinClear, LOW);
    Wait();
    digitalWrite(m_PinClear, HIGH);
  }

  void PushValue(uint8_t value) const
  {
    PushValueNoStore(value);
    Store();
  }

  void PushValueNoStore(uint8_t value) const
  {
    // the SN74HC595N has eight output pins labeled QA through QH
    // the value will be stored such that QH will contain the least significant bit
    // and QA will contain the most significant bit
    for (auto i = 0; i < 8; i++) {
      PushBitNoStore(static_cast<bool>((value >> i) & 1U));
    }
  }

  void PushBit(const bool value) const
  {
    PushBitNoStore(value);
    Store();
  }

  void PushBitNoStore(const bool value) const
  {
    digitalWrite(m_PinSerial, value ? HIGH : LOW);
    Shift();
  }

  private:

  void Store() const
  {
    digitalWrite(m_PinStorageRegisterClock, HIGH);
    Wait();
    digitalWrite(m_PinStorageRegisterClock, LOW);
  }

  void Shift() const
  {
    digitalWrite(m_PinShiftRegisterClock, HIGH);
    Wait();
    digitalWrite(m_PinShiftRegisterClock, LOW);
  }

  inline void Wait() const
  {
    delayMicroseconds(EDGE_TIME_MICROS);
  }

  static constexpr auto EDGE_TIME_MICROS = 1U;

  uint8_t m_PinOutputEnable;
  uint8_t m_PinSerial;
  uint8_t m_PinShiftRegisterClock;
  uint8_t m_PinStorageRegisterClock;
  uint8_t m_PinClear;

};