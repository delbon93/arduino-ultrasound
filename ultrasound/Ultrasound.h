#include <stdint.h>
#include <Arduino.h>

#pragma once

// Implementation for HC-SR04 ultrasound echo sensor
class Ultrasound 
{
  public:
  
  typedef void (*UltrasoundSensorCallback)(uint16_t);
  typedef void (*UltrasoundOutOfRangeCallback)();

  Ultrasound(uint8_t triggerPin, uint8_t echoPin, uint16_t triggerDelay, uint16_t rangeLimit) : 
    m_SensorCallback(nullptr),
    m_OutOfRangeCallback(nullptr),
    m_TriggerPin(triggerPin), 
    m_EchoPin(echoPin),
    m_TriggerDelayReset(triggerDelay),
    m_TriggerDelay(0U),
    m_RangeLimit(rangeLimit)
  {
    m_LastFrameMillis = millis();
  }

  void SetSensorCallback(UltrasoundSensorCallback sensorCallback)
  {
    m_SensorCallback = sensorCallback;
  }

  void SetOutOfRangeCallback(UltrasoundOutOfRangeCallback outOfRangeCallback)
  {
    m_OutOfRangeCallback = outOfRangeCallback;
  }

  void Update() noexcept
  {
    auto delta = millis() - m_LastFrameMillis;

    m_LastFrameMillis = millis();
    
    m_TriggerDelay = m_TriggerDelay > delta ? m_TriggerDelay - delta : 0U;
    if (m_TriggerDelay == 0U && !m_TriggerWasSent) {
      triggerUltrasound();
      m_TriggerWasSent = true;
      m_TriggerDelay = m_TriggerDelayReset;
    }
    
    if (m_TriggerWasSent && digitalRead(m_EchoPin) == HIGH && !m_MeasurementStarted) {
      m_MeasurementStarted = true;
      m_TriggerStart = micros();
    }
    
    if (m_MeasurementStarted && digitalRead(m_EchoPin) == LOW) {
      auto delay = micros() - m_TriggerStart;

      m_TriggerStart = 0U;
      m_MeasurementStarted = false;
      m_TriggerWasSent = false;

      if (delay < 200000) {
        auto cmDistance = delay / 58U;
        if (m_SensorCallback != nullptr && cmDistance <= m_RangeLimit) {
          m_SensorCallback(cmDistance);
        }
        else if (m_OutOfRangeCallback != nullptr && cmDistance > m_RangeLimit) {
          m_OutOfRangeCallback();
        }
      }
    }
  }

  private: 

  void triggerUltrasound() {
    digitalWrite(m_TriggerPin, HIGH);
    delayMicroseconds(20);
    digitalWrite(m_TriggerPin, LOW);
  }

  UltrasoundSensorCallback m_SensorCallback;
  UltrasoundOutOfRangeCallback m_OutOfRangeCallback;

  uint8_t m_TriggerPin;
  uint8_t m_EchoPin;
  uint16_t m_TriggerDelayReset;
  uint16_t m_TriggerDelay;
  uint16_t m_RangeLimit;

  unsigned long m_TriggerStart = 0U;
  bool m_TriggerWasSent = false;
  bool m_MeasurementStarted = false;

  unsigned long m_LastFrameMillis;

};