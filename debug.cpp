/*
  debug.cpp
  ------------------------------------------------------
  Implementa impresión formateada por Serial.

  Responsabilidades:
  ------------------
  • Decodificar campos de bits
  • Mostrar valores claramente
  • Hacer legible la depuración

  Este archivo NUNCA maneja Bluetooth ni memoria.
*/
#include "debug.h"
#include "packets.h"
#include "debug_config.h"
// Incluye prototipos y variables globales

/* ---------- LAST VALUES ---------- */
static uint16_t lastLX = 0;
static uint16_t lastLY = 0;
static uint16_t lastRX = 0;
static uint16_t lastRY = 0;

static uint16_t lastKnobL = 0;
static uint16_t lastKnobR = 0;

static uint8_t lastEventId = 0;

static byte lastSwitches = 0xFF;

/* ---------- STICKS ---------- */
#if DBG_STICKS
void debugStickLX() {
  uint16_t x = rcStatePacket.data.leftStickX;
  uint16_t y = rcStatePacket.data.leftStickY;
  if (x != lastLX) {
    Serial.printf("L Stick X: %u    ", x);
    Serial.printf("L Stick Y: %u\n", y);
    lastLX = x;
  }
}

void debugStickLY() {
  uint16_t y = rcStatePacket.data.leftStickY;
  uint16_t x = rcStatePacket.data.leftStickX;
  if (y != lastLY) {
    Serial.printf("L Stick X: %u    ", x);
    Serial.printf("L Stick Y: %u\n", y);
    lastLY = y;
  }
}

void debugStickRX() {
  uint16_t x = rcStatePacket.data.rightStickX;
  uint16_t y = rcStatePacket.data.rightStickY;
  if (x != lastRX) {
    Serial.printf("R Stick X: %u    ", x);
    Serial.printf("R Stick Y: %u\n", y);
    lastRX = x;
  }
}

void debugStickRY() {
  uint16_t y = rcStatePacket.data.rightStickY;
  uint16_t x = rcStatePacket.data.rightStickX;
  if (y != lastRY) {
    Serial.printf("R Stick X: %u    ", x);
    Serial.printf("R Stick Y: %u\n", y);
    lastRY = y;
  }
}
#endif

/* ---------- KNOBS ---------- */
#if DBG_KNOBS
void debugKnobL() {
  uint16_t v = rcStatePacket.data.leftKnob;
  if (v != lastKnobL) {
    Serial.printf("Left Knob: %u\n", v);
    lastKnobL = v;
  }
}

void debugKnobR() {
  uint16_t v = rcStatePacket.data.rightKnob;
  if (v != lastKnobR) {
    Serial.printf("Right Knob: %u\n", v);
    lastKnobR = v;
  }
}
#endif

/* ---------- SWITCHES ---------- */
#if DBG_SWITCHES
void debugSwitches() {
  byte s = rcStatePacket.data.switches;

  if (s != lastSwitches) {
    Serial.printf("Switch Byte: 0x%02X\n", s);

    for (int i = 0; i < 6; i++) {
      Serial.printf("  S%d = %s\n",
        i + 1,
        (s & (1 << i)) ? "ON" : "OFF");
    }

    lastSwitches = s;
  }
}
#endif

// ----------------------------------------------------
// printPanelPacket()
// Imprime telemetría de panel
// ----------------------------------------------------
void printPanelPacket() {

  Serial.println("\n--- PANEL TELEMETRY ---");

  Serial.printf("Left: %u  Right: %u\n",
                panelPacket.leftPanelValue,
                panelPacket.rightPanelValue);
  // Valores del display

  Serial.printf("States: 0x%02X\n", panelPacket.panelStates);
  // Estados de LEDs

  Serial.printf("Checksum: %u\n", panelPacket.checksum);
}


// ----------------------------------------------------
// printIndicatorPacket()
// Imprime indicadores
// ----------------------------------------------------
void printIndicatorPacket() {

  Serial.println("\n--- INDICATOR TELEMETRY ---");

  Serial.printf("Analog: %u\n", indicatorPacket.analogValue);
  // Sensor analógico

  Serial.printf("Battery: %u%%\n", indicatorPacket.batteryLevel);
  // Nivel de batería

  Serial.printf("Checksum: %u\n", indicatorPacket.checksum);
}


