#include "Ultrasound.h"
#include "FourSegDisplay.h"
#include "RingBuffer.h"

constexpr auto PIN_USS_ECHO = 2;
constexpr auto PIN_USS_TRG = 4U;

constexpr auto PIN_BSR_OUTPUTEN = 0;
constexpr auto PIN_BSR_SER = 5;
constexpr auto PIN_BSR_SRCLK = 6;
constexpr auto PIN_BSR_SRCLR = 0;
constexpr auto PIN_BSR_RCLK = 7;

constexpr auto PIN_FSD_DIGIT1 = 8;
constexpr auto PIN_FSD_DIGIT2 = 9;
constexpr auto PIN_FSD_DIGIT3 = 10;
constexpr auto PIN_FSD_DIGIT4 = 11;

constexpr auto TRIGGER_DELAY = 80U;
constexpr auto RANGE_LIMIT = 400;

auto lastFrameMillis = millis();

Ultrasound distanceSensor(PIN_USS_TRG, PIN_USS_ECHO, TRIGGER_DELAY, RANGE_LIMIT);
ShiftRegister8 ledBsr(PIN_BSR_OUTPUTEN, PIN_BSR_SER, PIN_BSR_SRCLK, PIN_BSR_RCLK, PIN_BSR_SRCLR);
FourSegDisplay fourSegDisplay(ledBsr, PIN_FSD_DIGIT1, PIN_FSD_DIGIT2, PIN_FSD_DIGIT3, PIN_FSD_DIGIT4);

RingBuffer<uint16_t, 10> sensorDataBuffer(0);

void triggerUltrasound() {
  digitalWrite(PIN_USS_TRG, HIGH);
  delayMicroseconds(20);
  digitalWrite(PIN_USS_TRG, LOW);
}

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(PIN_USS_ECHO, INPUT);
  pinMode(PIN_USS_TRG, OUTPUT);

  // pinMode(PIN_BSR_OUTPUTEN, OUTPUT);
  pinMode(PIN_BSR_SER, OUTPUT);
  pinMode(PIN_BSR_SRCLK, OUTPUT);
  pinMode(PIN_BSR_RCLK, OUTPUT);
  // pinMode(PIN_BSR_SRCLR, OUTPUT);

  pinMode(PIN_FSD_DIGIT1, OUTPUT);
  pinMode(PIN_FSD_DIGIT2, OUTPUT);
  pinMode(PIN_FSD_DIGIT3, OUTPUT);
  pinMode(PIN_FSD_DIGIT4, OUTPUT);

  distanceSensor.SetSensorCallback([](uint16_t cmDistance)
  {
    //Serial.print(",distance_cm:"); 
    //Serial.println(cmDistance);
    sensorDataBuffer.Insert(cmDistance);
  });

  distanceSensor.SetOutOfRangeCallback([]() {
    //Serial.print(",distance_cm:"); 
    //Serial.println(RANGE_LIMIT);
    return;
  });
}

void loop() 
{
  fourSegDisplay.DisplayInt(sensorDataBuffer.GetAverage());

  fourSegDisplay.Update();
  distanceSensor.Update();
}
