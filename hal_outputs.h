#pragma once
#include <stdint.h>

/* PWM Outputs */
void halSetSteering(uint32_t value);
void halSetMotorLeft(uint32_t value);
void halSetMotorRight(uint32_t value);
void halSetCameraPan(uint32_t value);
void halSetLed(uint32_t value);
void halSetBuzzer(uint32_t value);

/* Switch Outputs */
void halSetSwitch(uint8_t index, bool state);

/* Pulse Outputs */
void halPulseSwitch(uint8_t index);