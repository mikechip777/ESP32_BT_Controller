#pragma once
#include <stdint.h>

struct InputRaw {
  uint16_t analog1;
  uint16_t analog2;
  uint16_t analog3;
  uint16_t analog4;
  byte d0;
  byte d16;
  byte d17;
  byte checksum;
};

void inputHwRead(InputRaw& out);
uint8_t buildDigitalMask();

uint16_t hwReadNumeric1();
uint16_t hwReadNumeric2();

uint8_t  hwReadIndicatorAnalog();
uint8_t  hwReadIndicatorBattery();

uint8_t  hwReadPlot1();
uint8_t  hwReadPlot2();
uint8_t  hwReadPlot3();

uint8_t  hwReadDigitalMask();