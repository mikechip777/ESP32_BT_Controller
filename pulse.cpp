#include "pulse.h"
#include "mcp_io.h"

/* =====================================================
   GPIO NON-BLOCKING PULSE
   ===================================================== */

struct PulseState {
  bool active;
  uint8_t pin;
  uint32_t startTime;
  uint16_t duration;
};

static PulseState gpioPulse = {false, 0, 0, 0};

void pulseStart(uint8_t pin, uint16_t durationMs) {

  if (gpioPulse.active) return;

  digitalWrite(pin, HIGH);

  gpioPulse.active = true;
  gpioPulse.pin = pin;
  gpioPulse.startTime = millis();
  gpioPulse.duration = durationMs;
}

void pulseUpdate() {

  if (!gpioPulse.active) return;

  if (millis() - gpioPulse.startTime >= gpioPulse.duration) {
    digitalWrite(gpioPulse.pin, LOW);
    gpioPulse.active = false;
  }
}

/* =====================================================
   MCP NON-BLOCKING PULSE
   ===================================================== */

#if USE_MCP23017

static PulseState mcpPulse = {false, 0, 0, 0};

void mcpPulseStart(uint8_t pin, uint16_t durationMs) {

  if (mcpPulse.active) return;

  mcpWriteOutput(pin, HIGH);

  mcpPulse.active = true;
  mcpPulse.pin = pin;
  mcpPulse.startTime = millis();
  mcpPulse.duration = durationMs;
}

void mcpPulseUpdate() {

  if (!mcpPulse.active) return;

  if (millis() - mcpPulse.startTime >= mcpPulse.duration) {
    mcpWriteOutput(mcpPulse.pin, LOW);
    mcpPulse.active = false;
  }
}

#endif
