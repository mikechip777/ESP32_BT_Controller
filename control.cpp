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

 --------------------- PWM OUTPUTS ----------------------

   GPIO 18 ───► STEERING SERVO        (Left Stick X)
   GPIO 19 ───► LEFT MOTOR PWM        (Left Stick Y)
   GPIO 21 ───► CAMERA PAN / AUX PWM  (Right Stick X)
   GPIO 22 ───► RIGHT MOTOR PWM       (Right Stick Y)
   GPIO 23 ───► LED DIMMER PWM        (Left Knob)
   GPIO 27 ───► BUZZER PWM            (Right Knob)

 --------------------- SWITCH OUTPUTS -------------------

   GPIO 32 ───► SWITCH 1
   GPIO 33 ───► SWITCH 2
   GPIO 25 ───► SWITCH 3
   GPIO 26 ───► SWITCH 4
   GPIO  4 ───► SWITCH 5
   GPIO  5 ───► SWITCH 6
   GPIO 12 ───► SWITCH 7
   GPIO 13 ───► SWITCH 8

 --------------------- EVENT OUTPUTS --------------------

   GPIO 14 ───► EVENT 1  (momentary pulse)
   GPIO 15 ───► EVENT 2
   GPIO  2 ───► EVENT 3
   GPIO  0 ───► EVENT 4   ⚠ BOOT PIN (must be HIGH at boot!)

 --------------------- I2C BUS --------------------------

   GPIO 21 ───► SDA
   GPIO 22 ───► SCL

   Used for:
     - OLED display
     - Sensors
     - Telemetry modules

 --------------------- BOOT WARNINGS --------------------

   GPIO 0  → MUST be HIGH during boot
   GPIO 2  → MUST be LOW during boot
   GPIO 12 → Avoid pull-ups at boot

   ❌ NEVER USE:
      GPIO 6–11 (Flash memory pins)

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


/*
=========================================================
        MAPEO DE PINES – ESP32 RC
=========================================================

 ---------------- SALIDAS PWM ----------------

 GPIO 18 ─► SERVO DIRECCIÓN
 GPIO 19 ─► MOTOR IZQUIERDO
 GPIO 21 ─► PAN CÁMARA
 GPIO 22 ─► MOTOR DERECHO
 GPIO 23 ─► LED
 GPIO 27 ─► BUZZER

 ---------------- SWITCHES ------------------

 GPIO 32 ─► SW1
 GPIO 33 ─► SW2
 GPIO 25 ─► SW3
 GPIO 26 ─► SW4
 GPIO  4 ─► SW5
 GPIO  5 ─► SW6
 GPIO 12 ─► SW7
 GPIO 13 ─► SW8

 ---------------- EVENTOS -------------------

 GPIO 14 ─► EVT1
 GPIO 15 ─► EVT2
 GPIO  2 ─► EVT3
 GPIO  0 ─► EVT4 (PIN DE BOOT)

 ---------------- I2C -----------------------

 GPIO 21 ─► SDA
 GPIO 22 ─► SCL

 ⚠ NO USAR: GPIO 6–11 (Flash)

=========================================================
*/


#include <Arduino.h>
#include "packets.h"
#include "control.h"

/* =====================================================
   PIN DEFINITIONS
   Definición de pines
   ===================================================== */

/* -------- ANALOG / PWM OUTPUTS -------- */
#define PIN_STEER     18   // Servo steering (PWM)
#define PIN_MOTOR_L  19   // Left motor (PWM)
#define PIN_PAN      17   // Camera pan / aux servo (PWM)
#define PIN_MOTOR_R  16   // Right motor (PWM)
#define PIN_LED      23   // LED brightness (PWM)
#define PIN_BUZZER   27   // Buzzer (PWM)

/* -------- SWITCH DIGITAL OUTPUTS -------- */
#define PIN_SW1  32
#define PIN_SW2  33
#define PIN_SW3  25
#define PIN_SW4  26
#define PIN_SW5   4
#define PIN_SW6   5
#define PIN_SW7  12
#define PIN_SW8  13

/* -------- EVENT DIGITAL OUTPUTS -------- */
#define PIN_EVT1 14
#define PIN_EVT2 15
#define PIN_EVT3  2   // Onboard LED pin
#define PIN_EVT4  0   // ⚠ Boot pin: must NOT be held LOW at reset

/* =====================================================
   PWM PARAMETERS
   ===================================================== */

#define SERVO_FREQ   50
#define SERVO_RES    16

#define MOTOR_FREQ   20000
#define MOTOR_RES    8

#define LED_FREQ     5000
#define LED_RES      8

#define BUZZ_FREQ    4000
#define BUZZ_RES     8

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
   INITIALIZATION
   ===================================================== */

void controlInit() {

  // Switch outputs
  pinMode(PIN_SW1, OUTPUT);
  pinMode(PIN_SW2, OUTPUT);
  pinMode(PIN_SW3, OUTPUT);
  pinMode(PIN_SW4, OUTPUT);
  pinMode(PIN_SW5, OUTPUT);
  pinMode(PIN_SW6, OUTPUT);
  pinMode(PIN_SW7, OUTPUT);
  pinMode(PIN_SW8, OUTPUT);

  // Event outputs
  pinMode(PIN_EVT1, OUTPUT);
  pinMode(PIN_EVT2, OUTPUT);
  pinMode(PIN_EVT3, OUTPUT);
  pinMode(PIN_EVT4, OUTPUT);

  // Attach PWM outputs
  ledcAttach(PIN_STEER,    SERVO_FREQ, SERVO_RES);
  ledcAttach(PIN_MOTOR_L, MOTOR_FREQ, MOTOR_RES);
  ledcAttach(PIN_PAN,     SERVO_FREQ, SERVO_RES);
  ledcAttach(PIN_MOTOR_R, MOTOR_FREQ, MOTOR_RES);
  ledcAttach(PIN_LED,     LED_FREQ,   LED_RES);
  ledcAttach(PIN_BUZZER,  BUZZ_FREQ,  BUZZ_RES);
}

/* =====================================================
   MAIN CONTROL UPDATE
   ===================================================== */

void controlUpdate() {

  /* ---------- STICKS → SERVO + MOTORS ---------- */

  uint32_t steerPWM = mapServo(rcStatePacket.data.leftStickX);
  uint32_t motorL   = mapMotor(rcStatePacket.data.leftStickY);
  uint32_t motorR   = mapMotor(rcStatePacket.data.rightStickY);
  uint32_t panPWM   = mapServo(rcStatePacket.data.rightStickX);

  ledcWrite(PIN_STEER, steerPWM);
  ledcWrite(PIN_MOTOR_L, motorL);
  ledcWrite(PIN_MOTOR_R, motorR);
  ledcWrite(PIN_PAN, panPWM);

  /* ---------- KNOBS → LED + BUZZER ---------- */

  uint32_t ledPWM   = map(rcStatePacket.data.leftKnob,  0, 4095, 0, 255);
  uint32_t buzzPWM  = map(rcStatePacket.data.rightKnob, 0, 4095, 0, 255);

  ledcWrite(PIN_LED, ledPWM);
  ledcWrite(PIN_BUZZER, buzzPWM);

  /* ---------- SWITCHES → DIGITAL OUTPUTS ---------- */

  byte sw = rcStatePacket.data.switches;

  digitalWrite(PIN_SW1, sw & (1 << 0));
  digitalWrite(PIN_SW2, sw & (1 << 1));
  digitalWrite(PIN_SW3, sw & (1 << 2));
  digitalWrite(PIN_SW4, sw & (1 << 3));
  digitalWrite(PIN_SW5, sw & (1 << 4));
  digitalWrite(PIN_SW6, sw & (1 << 5));
  digitalWrite(PIN_SW7, sw & (1 << 6));
  digitalWrite(PIN_SW8, sw & (1 << 7));
}

/* =====================================================
   EVENT HANDLING
   ===================================================== */

void controlHandleEvent(byte eventId) {

  switch (eventId) {

    case 0x01: digitalWrite(PIN_EVT1, HIGH); delay(50); digitalWrite(PIN_EVT1, LOW); break;
    case 0x02: digitalWrite(PIN_EVT2, HIGH); delay(50); digitalWrite(PIN_EVT2, LOW); break;
    case 0x03: digitalWrite(PIN_EVT3, HIGH); delay(50); digitalWrite(PIN_EVT3, LOW); break;
    case 0x04: digitalWrite(PIN_EVT4, HIGH); delay(50); digitalWrite(PIN_EVT4, LOW); break;

    default: break;
  }
}