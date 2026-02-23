/*
  telemetry.cpp
  ------------------------------------------------------
  Implements all telemetry packet generators.

  This module is WRITE-ONLY.
  It builds packets and sends them via Bluetooth.
  It does NOT decide where data comes from.
*/

#include <Arduino.h>
#include <string.h>

#include "bluetooth.h"
#include "packets.h"
#include "telemetry.h"
#include "telemetry_source.h"
#include "debug_config.h"

/* =====================================================
   TIMING
   ===================================================== */

const unsigned long indicatorInterval = 500;
const unsigned long plotInterval = 50;
const unsigned long resendWindow = 300;
const unsigned long resendInterval = 100;

/* =====================================================
   STATE
   ===================================================== */

static unsigned long lastIndicatorSend = 0;
static unsigned long lastPlotSend = 0;

static uint16_t lastPanelL = 0;
static uint16_t lastPanelR = 0;

static uint16_t pendingL = 0;
static uint16_t pendingR = 0;

static unsigned long panelRetryUntil = 0;
static unsigned long lastPanelTx = 0;

static bool configSent = false;

/* =====================================================
   CONFIG LABELS
   ===================================================== */

void sendConfigTelemetry() {

  const char* plotNames[] = { "Volts", "Amps", "RPMs" };
  const char* panelNames[] = { "Left", "Right" };
  const char* indicatorNames[] = { "Throttle", "Battery" };

  const uint8_t plotCount = 3;
  const uint8_t panelCount = 2;
  const uint8_t indicatorCount = 2;

  byte buf[200];
  int idx = 0;

  buf[idx++] = 0xCC;
  buf[idx++] = 0x44;

  // reserve space for length (2 bytes)
  int lengthIndex = idx;
  buf[idx++] = 0;
  buf[idx++] = 0;

  buf[idx++] = 3;  // number of sections

  // ---------- PLOT SECTION ----------
  buf[idx++] = 0x01;
  buf[idx++] = plotCount;

  for (int i = 0; i < plotCount; i++) {
    uint8_t len = strlen(plotNames[i]);
    buf[idx++] = len;
    memcpy(&buf[idx], plotNames[i], len);
    idx += len;
  }

  // ---------- PANEL SECTION ----------
  buf[idx++] = 0x02;
  buf[idx++] = panelCount;

  for (int i = 0; i < panelCount; i++) {
    uint8_t len = strlen(panelNames[i]);
    buf[idx++] = len;
    memcpy(&buf[idx], panelNames[i], len);
    idx += len;
  }

  // ---------- INDICATOR SECTION ----------
  buf[idx++] = 0x03;
  buf[idx++] = indicatorCount;

  for (int i = 0; i < indicatorCount; i++) {
    uint8_t len = strlen(indicatorNames[i]);
    buf[idx++] = len;
    memcpy(&buf[idx], indicatorNames[i], len);
    idx += len;
  }

  // Compute payload length (everything after header+length until checksum)
  uint16_t payloadLength = idx - 4;
  buf[lengthIndex] = payloadLength & 0xFF;
  buf[lengthIndex + 1] = (payloadLength >> 8) & 0xFF;

  // Compute checksum (from byte 2 to last payload byte)
  uint8_t checksum = 0;
  for (int i = 2; i < idx; i++)
    checksum += buf[i];

  buf[idx++] = checksum;

  if (SerialBT.hasClient()) {
    Serial.println("Telemetry config sent...");
    SerialBT.write(buf, idx);
  }
}

/* =====================================================
   HELPER
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
   MAIN TELEMETRY LOOP
   ===================================================== */

void sendTelemetryIfDue() {

  // Handle connection state
  if (!SerialBT.hasClient()) {
    configSent = false;
    return;
  }

  // Send config once after connection
  if (!configSent) {
    sendConfigTelemetry();
    configSent = true;
  }

  unsigned long t = millis();

  // Update debug input if active
  telemetrySourceUpdate();

  /* ---------- PANEL ---------- */

  uint16_t newL = getPanelLeft();
  uint16_t newR = getPanelRight();

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
    panelPacket.leftPanelValue = pendingL;
    panelPacket.rightPanelValue = pendingR;
    panelPacket.panelStates = 0b00000111;  // example state bits

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

    indicatorPacket.analogValue = getIndicatorAnalog();
    indicatorPacket.batteryLevel = getIndicatorBattery();

    computeChecksum(&indicatorPacket, INDICATOR_PACKET_SIZE);
    SerialBT.write((byte*)&indicatorPacket, INDICATOR_PACKET_SIZE);
  }

  /* ---------- PLOT ---------- */

  if (t - lastPlotSend > plotInterval) {

    lastPlotSend = t;

    uint8_t v1 = getPlot1();
    uint8_t v2 = getPlot2();
    uint8_t v3 = getPlot3();

    byte buf[16];
    int idx = 0;

    buf[idx++] = 0xCC;
    buf[idx++] = 0x33;

    int lengthIndex = idx;
    buf[idx++] = 0;  // len low
    buf[idx++] = 0;  // len high

    buf[idx++] = 3;  // count
    buf[idx++] = v1;
    buf[idx++] = v2;
    buf[idx++] = v3;

    uint16_t payloadLength = idx - 4;
    buf[lengthIndex] = payloadLength & 0xFF;
    buf[lengthIndex + 1] = (payloadLength >> 8) & 0xFF;

    uint8_t checksum = 0;
    for (int i = 2; i < idx; i++)
      checksum += buf[i];

    buf[idx++] = checksum;

    SerialBT.write(buf, idx);
  }
}
