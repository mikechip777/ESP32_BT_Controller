/*
  control.cpp
  ------------------------------------------------------
  CONTROL LAYER – Hardware Output Logic
*/


/*
=========================================================
        ESP32 RC CONTROLLER – PIN MAP DIAGRAM
=========================================================

                    ┌──────────────────────┐
                    │        ESP32          │
                    │    (WROOM / DevKit)   │
                    └──────────────────────┘

=========================================================
        RC DATA → HARDWARE FLOW
=========================================================

 Android App
      │
      ▼
 Bluetooth SPP
      │
      ▼
 receiver.cpp
      │
      ▼
 packets.h  (rcStatePacket)
      │
      ▼
 control.cpp
      │
      ├──► Servo + Motors (PWM)
      ├──► LED + Buzzer (PWM)
      ├──► Switch Outputs
      └──► Event Outputs

=========================================================
*/


#include <Arduino.h>
#include "packets.h"
#include "control.h"
#include "pins.h"
#include "mcp_io.h"
#include "pulse.h"

/* =====================================================
   INTERNAL HELPERS
   ===================================================== */

static uint32_t mapServo(uint16_t v) {
  return map(v, 0, 4095, 3277, 6553);
}

static uint32_t mapMotor(uint16_t v) {
  return map(v, 0, 4095, 0, 255);
}

/* =====================================================
   MAIN CONTROL UPDATE
   ===================================================== */

void controlUpdate() {

  /* ---------- STICKS → SERVO + MOTORS ---------- */

  uint32_t steerPWM = mapServo(rcStatePacket.data.leftStickX);
  uint32_t motorL = mapMotor(rcStatePacket.data.leftStickY);
  uint32_t motorR = mapMotor(rcStatePacket.data.rightStickY);
  uint32_t panPWM = mapServo(rcStatePacket.data.rightStickX);

  ledcWrite(PIN_PWM_STICK_LX, steerPWM);
  ledcWrite(PIN_PWM_STICK_LY, motorL);
  ledcWrite(PIN_PWM_STICK_RX, motorR);
  ledcWrite(PIN_PWM_STICK_RY, panPWM);

  /* ---------- KNOBS → LED + BUZZER ---------- */

  uint32_t ledPWM = map(rcStatePacket.data.leftKnob, 0, 4095, 0, 255);
  uint32_t buzzPWM = map(rcStatePacket.data.rightKnob, 0, 4095, 0, 255);

  ledcWrite(PIN_PWM_KNOB_L, ledPWM);
  ledcWrite(PIN_PWM_KNOB_R, buzzPWM);

  /* ---------- SWITCHES → DIGITAL OUTPUTS ---------- */

  byte sw = rcStatePacket.data.switches;
#if PROJECT_MODE == FULL_RC_MODE
  digitalWrite(PIN_SW1, sw & (1 << 0));
  digitalWrite(PIN_SW2, sw & (1 << 1));
  digitalWrite(PIN_SW3, sw & (1 << 2));
  digitalWrite(PIN_SW4, sw & (1 << 3));
  digitalWrite(PIN_SW5, sw & (1 << 4));
  digitalWrite(PIN_SW6, sw & (1 << 5));
#endif

#if PROJECT_MODE == FULL_RC_MODE_MCP
  mcpWriteOutput(GPB0, sw & (1 << 0));
  mcpWriteOutput(GPB1, sw & (1 << 1));
  mcpWriteOutput(GPB2, sw & (1 << 2));
  mcpWriteOutput(GPB3, sw & (1 << 3));
  mcpWriteOutput(GPB4, sw & (1 << 4));
  mcpWriteOutput(GPB5, sw & (1 << 5));
#endif
}

/* =====================================================
   EVENT HANDLING
   ===================================================== */

void controlHandleEvent(byte eventId) {
#if PROJECT_MODE == FULL_RC_MODE_MCP
  switch (eventId) {

    case 0x01: mcpPulseStart(GPB6);; break;
    case 0x02: mcpPulseStart(GPB7);; break;
    case 0x03: pulseStart(PIN_FREE1); break;
    case 0x04: pulseStart(PIN_FREE2); break;

    default: break;
  }
#endif
}