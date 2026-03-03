#include <Arduino.h>
#include "pins.h"
#include "mcp_io.h"
#include "pulse.h"
#include "feature_config.h"

/* ================= PWM ================= */

void halSetSteering(uint32_t value) {
  ledcWrite(PIN_PWM_STICK_LX, value);
}

void halSetMotorLeft(uint32_t value) {
  ledcWrite(PIN_PWM_STICK_LY, value);
}

void halSetMotorRight(uint32_t value) {
  ledcWrite(PIN_PWM_STICK_RX, value);
}

void halSetCameraPan(uint32_t value) {
  ledcWrite(PIN_PWM_STICK_RY, value);
}

void halSetLed(uint32_t value) {
  ledcWrite(PIN_PWM_KNOB_L, value);
}

void halSetBuzzer(uint32_t value) {
  ledcWrite(PIN_PWM_KNOB_R, value);
}

/* ================= SWITCHES ================= */

void halSetSwitch(uint8_t index, bool state) {

#if PROJECT_MODE == FULL_RC_MODE
  const uint8_t pins[6] = {
    PIN_SW1, PIN_SW2, PIN_SW3,
    PIN_SW4, PIN_SW5, PIN_SW6
  };

  if (index < 6)
    digitalWrite(pins[index], state);

#elif PROJECT_MODE == FULL_RC_MODE_MCP
  const uint8_t mcpPins[6] = {
    GPB0, GPB1, GPB2,
    GPB3, GPB4, GPB5
  };

  if (index < 6)
    mcpWriteOutput(mcpPins[index], state);
#endif
}

/* ================= PULSES ================= */

void halPulseSwitch(uint8_t index) {

#if PROJECT_MODE == FULL_RC_MODE_MCP
  const uint8_t mcpPulsePins[2] = { GPB6, GPB7 };

  if (index < 2)
    mcpPulseStart(mcpPulsePins[index]);

#else
  const uint8_t gpioPulsePins[2] = { PIN_FREE1, PIN_FREE2 };

  if (index < 2)
    pulseStart(gpioPulsePins[index]);
#endif
}
