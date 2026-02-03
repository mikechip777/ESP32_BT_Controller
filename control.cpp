/*
  control.cpp
  ------------------------------------------------------
  CONTROL LAYER – Hardware Output Logic

  ENGLISH:
  --------
  This file converts decoded RC data received from the
  Android app into real ESP32 hardware outputs.

  It reads:
   - Stick positions
   - Knob values
   - Switch states
   - Event button presses

  And drives:
   - Servo (steering)
   - DC motors (PWM)
   - LED indicator
   - Buzzer
   - Digital event outputs
   - 6 digital switch outputs
   - 4 digital event outputs

  This file NEVER handles Bluetooth or packet parsing.

  ESPAÑOL:
  --------
  Este archivo convierte los datos RC recibidos desde
  la app Android en salidas físicas del ESP32.

  Lee:
   - Posición de sticks
   - Valores de perillas
   - Estados de switches
   - Eventos de botones

  Controla:
   - Servo (dirección)
   - Motores DC (PWM)
   - LED indicador
   - Buzzer
   - Salidas digitales por evento
   - 6 salidas por switches
   - 4 salidas por eventos
*/

#include <Arduino.h>
#include "packets.h"
#include "control.h"

/* =====================================================
   PIN DEFINITIONS
   Definición de pines
   ===================================================== */

#define PIN_STEER_SERVO   18
#define PIN_MOTOR_L       19
#define PIN_MOTOR_R       21
#define PIN_LED           2
#define PIN_BUZZER        27
// SWITCH OUTPUTS (from switch byte)
#define PIN_SW1  32
#define PIN_SW2  33
#define PIN_SW3  25
#define PIN_SW4  26
#define PIN_SW5  4
#define PIN_SW6  5

#define PIN_EVT_1         12
#define PIN_EVT_2         13
#define PIN_EVT_3         14
#define PIN_EVT_4         15

/* =====================================================
   PWM PARAMETERS
   Parámetros PWM
   ===================================================== */

#define SERVO_FREQ   50     // Hz
#define SERVO_RES    16     // bits

#define MOTOR_FREQ   20000  // Hz
#define MOTOR_RES    8

#define LED_FREQ     5000
#define LED_RES      8

#define BUZZ_FREQ    4000
#define BUZZ_RES     8

/* =====================================================
   INTERNAL HELPERS
   Funciones auxiliares internas
   ===================================================== */

// Map stick (0–4095) to servo pulse width
static uint32_t mapServo(uint16_t v) {
  return map(v, 0, 4095, 3277, 6553); // ≈ 1ms–2ms (16-bit)
}

// Map stick (0–4095) to motor PWM
static uint32_t mapMotor(uint16_t v) {
  return map(v, 0, 4095, 0, 255);
}

/* =====================================================
   INITIALIZATION
   Inicialización
   ===================================================== */

void controlInit() {

    // Switch outputs
  pinMode(PIN_SW1, OUTPUT);
  pinMode(PIN_SW2, OUTPUT);
  pinMode(PIN_SW3, OUTPUT);
  pinMode(PIN_SW4, OUTPUT);
  pinMode(PIN_SW5, OUTPUT);
  pinMode(PIN_SW6, OUTPUT);

  // Digital outputs for event buttons
  pinMode(PIN_EVT_1, OUTPUT);
  pinMode(PIN_EVT_2, OUTPUT);
  pinMode(PIN_EVT_3, OUTPUT);
  pinMode(PIN_EVT_4, OUTPUT);

  pinMode(PIN_LED, OUTPUT);

  // Attach PWM outputs (NEW Arduino-ESP32 API)
  ledcAttach(PIN_STEER_SERVO, SERVO_FREQ, SERVO_RES);
  ledcAttach(PIN_MOTOR_L,     MOTOR_FREQ, MOTOR_RES);
  ledcAttach(PIN_MOTOR_R,     MOTOR_FREQ, MOTOR_RES);
  ledcAttach(PIN_LED,         LED_FREQ,   LED_RES);
  ledcAttach(PIN_BUZZER,      BUZZ_FREQ,  BUZZ_RES);
}

/* =====================================================
   MAIN CONTROL UPDATE
   Actualización principal de control
   ===================================================== */

void controlUpdate() {

  /* ---------- STICKS → SERVO + MOTORS ---------- */

  uint32_t steerPWM = mapServo(rcStatePacket.data.leftStickX);
  uint32_t motorL   = mapMotor(rcStatePacket.data.leftStickY);
  uint32_t motorR   = mapMotor(rcStatePacket.data.rightStickY);

  ledcWrite(PIN_STEER_SERVO, steerPWM);
  ledcWrite(PIN_MOTOR_L, motorL);
  ledcWrite(PIN_MOTOR_R, motorR);

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
}

/* =====================================================
   EVENT HANDLING
   Manejo de eventos (botones)
   ===================================================== */

void controlHandleEvent(byte eventId) {
  
  switch (eventId) {
    case 0x01: digitalWrite(PIN_EVT_1, HIGH); delay(50); digitalWrite(PIN_EVT_1, LOW); break;
    case 0x02: digitalWrite(PIN_EVT_2, HIGH); delay(50); digitalWrite(PIN_EVT_2, LOW); break;
    case 0x03: digitalWrite(PIN_EVT_3, HIGH); delay(50); digitalWrite(PIN_EVT_3, LOW); break;
    case 0x04: digitalWrite(PIN_EVT_4, HIGH); delay(50); digitalWrite(PIN_EVT_4, LOW); break;
    default: break;
  }
}
