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
#include "control.h"
#include "packets.h"
#include "hal_outputs.h"

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

  uint32_t steerPWM = mapServo(rcStatePacket.data.leftStickX);
  uint32_t motorL   = mapMotor(rcStatePacket.data.leftStickY);
  uint32_t motorR   = mapMotor(rcStatePacket.data.rightStickY);
  uint32_t panPWM   = mapServo(rcStatePacket.data.rightStickX);

  halSetSteering(steerPWM);
  halSetMotorLeft(motorL);
  halSetMotorRight(motorR);
  halSetCameraPan(panPWM);

  halSetLed(map(rcStatePacket.data.leftKnob, 0, 4095, 0, 255));
  halSetBuzzer(map(rcStatePacket.data.rightKnob, 0, 4095, 0, 255));

  byte sw = rcStatePacket.data.switches;

  for (int i = 0; i < 6; i++) {
    halSetSwitch(i, sw & (1 << i));
  }
}

/* =====================================================
   EVENT HANDLING
   ===================================================== */

void controlHandleEvent(byte eventId) {

  switch (eventId) {
    case 0x01: halPulseSwitch(0); break;
    case 0x02: halPulseSwitch(1); break;
    case 0x03: halPulseSwitch(0); break;
    case 0x04: halPulseSwitch(1); break;
    default: break;
  }
}