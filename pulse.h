#ifndef PULSE_H
#define PULSE_H

#include <Arduino.h>
#include "user_config.h"

/* ================= GPIO Pulse ================= */

void pulseStart(uint8_t pin, uint16_t durationMs = 50);
void pulseUpdate();

/* ================= MCP Pulse ================= */

#if PROJECT_MODE == MODE_FULL_RC_MCP
void mcpPulseStart(uint8_t pin, uint16_t durationMs = 50);
void mcpPulseUpdate();
#endif

#endif
