/*
  telemetry.cpp
  ------------------------------------------------------
  Implements all telemetry packet generators.

  Responsibilities:
  -----------------
  • Fill telemetry packet structs
  • Compute checksums
  • Send raw bytes over SerialBT
  • Control timing of transmissions

  Packet Types:
  -------------
  0xCC 0x11 -> PanelPacket
  0xCC 0x22 -> IndicatorPacket
  0xCC 0x33 -> PlotPacket
  0xCC 0x44 -> ConfigPacket

  This module is WRITE-ONLY.
*/

#include <Arduino.h>
#include <math.h>
#include "bluetooth.h"
#include "packets.h"
#include "telemetry.h"
#include "pins.h"
#include "debug_config.h"

/* =====================================================
   TIMING
   ===================================================== */
const unsigned long indicatorInterval = 500;
const unsigned long plotInterval      = 50;

const unsigned long resendWindow      = 300;
const unsigned long resendInterval    = 100;

/* =====================================================
   STATE
   ===================================================== */
static unsigned long lastIndicatorSend = 0;
static unsigned long lastPlotSend      = 0;

static uint16_t lastPanelL = 0;
static uint16_t lastPanelR = 0;

static uint16_t pendingL = 0;
static uint16_t pendingR = 0;
static unsigned long panelRetryUntil = 0;
static unsigned long lastPanelTx     = 0;

/* =====================================================
   DEBUG VALUES
   ===================================================== */
#if DEBUG_MODE == DBG_PANEL
static uint16_t dbgLeft  = 0;
static uint16_t dbgRight = 0;
#endif

#if DEBUG_MODE == DBG_INDICATOR
static uint8_t dbgIndicatorValue = 0;
static uint8_t dbgBatteryValue   = 0;
#endif

#if DEBUG_MODE == DBG_PLOT
static uint8_t dbgPlot1 = 0;
static uint8_t dbgPlot2 = 0;
static uint8_t dbgPlot3 = 0;
#endif

/* =====================================================
   HELPERS
   ===================================================== */
static bool panelChanged(uint16_t l, uint16_t r) {
  return (l != lastPanelL || r != lastPanelR);
}

static void computeChecksum(void* pkt, uint8_t size) {
  byte* b = (byte*)pkt;
  byte c = 0;
  for (int i = 2; i < size - 1; i++)
    c += b[i];
  b[size - 1] = c;
}

/* =====================================================
   REAL SENSOR READERS
   ===================================================== */
static uint8_t readRealPlot1() {
  return map(analogRead(34), 0, 4095, 0, 255);
}

static uint8_t readRealPlot2() {
  return map(analogRead(35), 0, 4095, 0, 255);
}

static uint8_t readRealPlot3() {
  return map(analogRead(32), 0, 4095, 0, 255);
}

/* =====================================================
   SERIAL DEBUG INPUT (Unified)
   ===================================================== */
#if DEBUG_MODE != DBG_NONE
static void readDebugFromSerial() {

  if (!Serial.available()) return;

  String line = Serial.readStringUntil('\n');
  line.trim();

#if DEBUG_MODE == DBG_PANEL

  int comma = line.indexOf(',');
  if (comma < 0) return;

  int l = line.substring(0, comma).toInt();
  int r = line.substring(comma + 1).toInt();

  if (l >= 0 && l <= 9999 && r >= 0 && r <= 9999) {
    dbgLeft = l;
    dbgRight = r;
    Serial.printf("Panel debug set: %u , %u\n", l, r);
  }

#elif DEBUG_MODE == DBG_INDICATOR

  int comma = line.indexOf(',');
  if (comma < 0) return;

  int a = line.substring(0, comma).toInt();
  int b = line.substring(comma + 1).toInt();

  if (a >= 0 && a <= 100 && b >= 0 && b <= 100) {
    dbgIndicatorValue = a;
    dbgBatteryValue   = b;
    Serial.printf("Indicator debug set: %u%% , %u%%\n", a, b);
  }

#elif DEBUG_MODE == DBG_PLOT

  int c1 = line.indexOf(',');
  int c2 = line.lastIndexOf(',');

  if (c1 < 0 || c2 <= c1) return;

  int v1 = line.substring(0, c1).toInt();
  int v2 = line.substring(c1 + 1, c2).toInt();
  int v3 = line.substring(c2 + 1).toInt();

  if (v1 >= 0 && v1 <= 255 &&
      v2 >= 0 && v2 <= 255 &&
      v3 >= 0 && v3 <= 255) {

    dbgPlot1 = v1;
    dbgPlot2 = v2;
    dbgPlot3 = v3;

    Serial.printf("Plot debug set: %u , %u , %u\n",
                  v1, v2, v3);
  }

#endif
}
#endif

/* =====================================================
   CONFIG TELEMETRY
   ===================================================== */
const char* plotNames[] = { "Volts", "Amps", "RPMs" };
const int NUM_PLOT_NAMES = 3;

void sendConfigTelemetry() {

  byte buf[128];
  int idx = 0;

  buf[idx++] = 0xCC;
  buf[idx++] = 0x44;
  buf[idx++] = NUM_PLOT_NAMES;

  for (int i = 0; i < NUM_PLOT_NAMES; i++) {
    byte l = strlen(plotNames[i]);
    buf[idx++] = l;
    memcpy(&buf[idx], plotNames[i], l);
    idx += l;
  }

  if (SerialBT.hasClient())
    SerialBT.write(buf, idx);
}

/* =====================================================
   MAIN TELEMETRY LOOP
   ===================================================== */
void sendTelemetryIfDue() {

  if (!SerialBT.hasClient()) return;

  unsigned long t = millis();

#if DEBUG_MODE != DBG_NONE
  readDebugFromSerial();
#endif

  /* ---------- PANEL ---------- */
  uint16_t newL, newR;

#if DEBUG_MODE == DBG_PANEL
  newL = dbgLeft;
  newR = dbgRight;
#else
  newL = map(analogRead(PIN_A34), 0, 4095, 0, 9999);
  newR = map(analogRead(PIN_A35), 0, 4095, 0, 9999);
#endif

  unsigned long now = millis();

  if (panelChanged(newL, newR)) {
    pendingL = newL;
    pendingR = newR;
    panelRetryUntil = now + resendWindow;
    lastPanelTx = 0;
  }

  if (now < panelRetryUntil && now - lastPanelTx >= resendInterval) {

    panelPacket.header1 = 0xCC;
    panelPacket.header2 = 0x11;
    panelPacket.leftPanelValue  = pendingL;
    panelPacket.rightPanelValue = pendingR;
    panelPacket.panelStates     = 0x07;

    computeChecksum(&panelPacket, PANEL_PACKET_SIZE);
    SerialBT.write((byte*)&panelPacket, PANEL_PACKET_SIZE);

    lastPanelTx = now;
  }

  if (now >= panelRetryUntil && panelRetryUntil != 0) {
    lastPanelL = pendingL;
    lastPanelR = pendingR;
    panelRetryUntil = 0;
  }

  /* ---------- INDICATOR ---------- */
  if (t - lastIndicatorSend > indicatorInterval) {

    lastIndicatorSend = t;

    indicatorPacket.header1 = 0xCC;
    indicatorPacket.header2 = 0x22;

#if DEBUG_MODE == DBG_INDICATOR
    indicatorPacket.analogValue = dbgIndicatorValue;
    indicatorPacket.batteryLevel = dbgBatteryValue;
#else
    indicatorPacket.analogValue =
      map(analogRead(PIN_A36), 0, 4095, 0, 100);

    indicatorPacket.batteryLevel =
      map(analogRead(PIN_A39), 0, 4095, 0, 100);
#endif

    computeChecksum(&indicatorPacket, INDICATOR_PACKET_SIZE);
    SerialBT.write((byte*)&indicatorPacket, INDICATOR_PACKET_SIZE);
  }

  /* ---------- PLOT ---------- */
  if (t - lastPlotSend > plotInterval) {

    lastPlotSend = t;

    uint8_t v1, v2, v3;

#if DEBUG_MODE == DBG_PLOT
    v1 = dbgPlot1;
    v2 = dbgPlot2;
    v3 = dbgPlot3;
#else
    v1 = readRealPlot1();
    v2 = readRealPlot2();
    v3 = readRealPlot3();
#endif

    plotPacket = {
      0xCC,
      0x33,
      3,
      v1,
      v2,
      v3,
      0
    };

    computeChecksum(&plotPacket, PLOT_PACKET_SIZE);
    SerialBT.write((byte*)&plotPacket, PLOT_PACKET_SIZE);
  }
}
