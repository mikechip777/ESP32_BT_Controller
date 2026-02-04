/*
  packets.h
  ------------------------------------------------------
  Central definition of ALL communication packets.

  This file defines:
  ------------------
  • RC State packet (18 bytes)
  • Event packet (4 bytes)
  • Telemetry packets:
      - PanelPacket
      - IndicatorPacket
      - PlotPacket

  It also defines:
  ----------------
  • Union wrappers to allow:
        struct access  <-> raw byte array access
  • Packet size constants.
  • Global packet instances used across files.

  Design Notes:
  -------------
  All packets use:
    - Packed layout
    - Fixed headers
    - Simple additive checksum

  This file contains ONLY data definitions.
*/
#ifndef PACKETS_H
#define PACKETS_H
#define INPUT_PACKET_SIZE 13
#define I2C_PACKET_SIZE 14

#include <Arduino.h>

/* ---- INPUT PACKETS ---- */

typedef struct __attribute__((packed)) {
  byte startByte1, startByte2;
  uint16_t leftStickX, leftStickY;
  uint16_t rightStickX, rightStickY;
  uint16_t leftKnob, rightKnob;
  byte switches;
  byte checksum;
  byte endByte1, endByte2;
} RcPacket;

typedef union { RcPacket data; byte bytes[sizeof(RcPacket)]; } StatePacketUnion;
extern StatePacketUnion rcStatePacket;
extern const int STATE_PACKET_SIZE;

typedef struct __attribute__((packed)) {
  byte startByte1, startByte2;
  byte eventId;
  byte checksum;
} EventPacket;

typedef union { EventPacket data; byte bytes[sizeof(EventPacket)]; } EventPacketUnion;
extern EventPacketUnion rcEventPacket;
extern const int EVENT_PACKET_SIZE;

/* ---- OUTPUT PACKETS ---- */

typedef struct __attribute__((packed)) {
  byte header1, header2;
  uint16_t leftPanelValue, rightPanelValue;
  byte panelStates;
  byte checksum;
} PanelPacket;
extern PanelPacket panelPacket;
extern const int PANEL_PACKET_SIZE;

typedef struct __attribute__((packed)) {
  byte header1, header2;
  byte analogValue, batteryLevel;
  byte checksum;
} IndicatorPacket;
extern IndicatorPacket indicatorPacket;
extern const int INDICATOR_PACKET_SIZE;

typedef struct __attribute__((packed)) {
  byte header1, header2;
  byte numPlots;
  byte plotValue1, plotValue2, plotValue3;
  byte checksum;
} PlotPacket;
extern PlotPacket plotPacket;
extern const int PLOT_PACKET_SIZE;


typedef struct {
  byte h1;      // 0xCC
  byte h2;      // 0x55
  uint16_t a34;
  uint16_t a35;
  uint16_t a36;
  uint16_t a39;
  byte d0;
  byte d16;
  byte d17;
  byte checksum;
} InputPacket;

extern InputPacket inputPacket;

typedef struct {
  byte h1;     // 0xCC
  byte h2;     // 0x66
  uint16_t temp;
  uint16_t ax;
  uint16_t ay;
  uint16_t az;
  byte sensorFlags;
  byte checksum;
} I2CPacket;

extern I2CPacket i2cPacket;
#endif
