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
  0xCC 0x44 -> ConfigPacket (variable length)

  This module is WRITE-ONLY.
  It never parses incoming data.
*/
#include <Arduino.h>
#include <math.h>
#include "bluetooth.h"
#include "packets.h"
#include "telemetry.h"
#include "pins.h"

static unsigned long lastPanelSend = 0;
static unsigned long lastIndicatorSend = 0;
static unsigned long lastPlotSend = 0;

uint16_t lastPanelL = 0;
uint16_t lastPanelR = 0;

uint16_t pendingL = 0;
uint16_t pendingR = 0;

unsigned long panelRetryUntil = 0;
unsigned long lastPanelTx = 0;

const unsigned long resendWindow = 300;
const unsigned long resendInterval = 100;

const long indicatorInterval = 250;
const long plotInterval = 50;

const char* plotNames[] = { "Volts", "Amps", "RPMs" };
const int NUM_PLOT_NAMES = 3;

uint16_t dbgLeft = 0;
uint16_t dbgRight = 0;
bool dbgPanelMode = true;  // true = use Serial input

bool panelChanged(uint16_t l, uint16_t r) {
  return (l != lastPanelL || r != lastPanelR);
}

void readPanelFromSerial() {
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    line.trim();

    int comma = line.indexOf(',');
    if (comma > 0) {
      int l = line.substring(0, comma).toInt();
      int r = line.substring(comma + 1).toInt();

      if (l >= 0 && l <= 9999 && r >= 0 && r <= 9999) {
        dbgLeft = l;
        dbgRight = r;
        Serial.printf("Panel set: L=%u R=%u\n", dbgLeft, dbgRight);
      } else {
        Serial.println("Range must be 0–9999");
      }
    } else {
      Serial.println("Format: left,right   (e.g. 1234,5678)");
    }
  }
}

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
  if (SerialBT.hasClient()) SerialBT.write(buf, idx);
}

void sendTelemetryIfDue() {
  unsigned long t = millis();
  if (!SerialBT.hasClient()) return;

  uint16_t newL, newR;
  if (dbgPanelMode) {
    newL = dbgLeft;
    newR = dbgRight;
  } else {
    newL = map(analogRead(PIN_A34), 0, 4095, 0, 9999);
    newR = map(analogRead(PIN_A35), 0, 4095, 0, 9999);
  }

  unsigned long now = millis();

  if(panelChanged(newL, newR)){
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
    panelPacket.panelStates = 0x07;  // both on

    byte c = 0;
    for (int i = 2; i < PANEL_PACKET_SIZE - 1; i++)
      c += ((byte*)&panelPacket)[i];

    panelPacket.checksum = c;

    SerialBT.write((byte*)&panelPacket, PANEL_PACKET_SIZE);

    lastPanelTx = now;
  }

  if(now >= panelRetryUntil && panelRetryUntil != 0){
    lastPanelL = pendingL;
    lastPanelR = pendingR;
    panelRetryUntil = 0;
  }

  if (t - lastIndicatorSend > indicatorInterval) {
    lastIndicatorSend = t;
    indicatorPacket = { 0xCC, 0x22, 75, 98, 0 };
    byte c = 0;
    for (int i = 2; i < INDICATOR_PACKET_SIZE - 1; i++) c += ((byte*)&indicatorPacket)[i];
    indicatorPacket.checksum = c;
    SerialBT.write((byte*)&indicatorPacket, INDICATOR_PACKET_SIZE);
  }

  if (t - lastPlotSend > plotInterval) {
    lastPlotSend = t;
    float s = t / 1000.0f;
    plotPacket = { 0xCC, 0x33, 3,
                   (byte)(((sin(s) * 0.4) + 0.6) * 255),
                   (byte)(((cos(s) * 0.2) + 0.25) * 255),
                   (byte)(((cos(s) * 0.2) + 0.5) * 255), 0 };
    byte c = 0;
    for (int i = 2; i < PLOT_PACKET_SIZE - 1; i++) c += ((byte*)&plotPacket)[i];
    plotPacket.checksum = c;
    SerialBT.write((byte*)&plotPacket, PLOT_PACKET_SIZE);
  }
}
