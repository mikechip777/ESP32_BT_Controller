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

static unsigned long lastPanelSend = 0;
static unsigned long lastIndicatorSend = 0;
static unsigned long lastPlotSend = 0;

const long panelInterval = 1000;
const long indicatorInterval = 250;
const long plotInterval = 50;

const char* plotNames[] = {"Volts","Amps","RPMs"};
const int NUM_PLOT_NAMES = 3;

void sendConfigTelemetry() {
  byte buf[128]; int idx=0;
  buf[idx++]=0xCC; buf[idx++]=0x44;
  buf[idx++]=NUM_PLOT_NAMES;
  for(int i=0;i<NUM_PLOT_NAMES;i++){
    byte l=strlen(plotNames[i]);
    buf[idx++]=l;
    memcpy(&buf[idx],plotNames[i],l);
    idx+=l;
  }
  if(SerialBT.hasClient()) SerialBT.write(buf,idx);
}

void sendTelemetryIfDue() {
  unsigned long t=millis();
  if(!SerialBT.hasClient()) return;

  if(t-lastPanelSend>panelInterval){
    lastPanelSend=t;
    panelPacket={0xCC,0x11,1234,5678,0,0};
    byte c=0; for(int i=2;i<PANEL_PACKET_SIZE-1;i++) c+=((byte*)&panelPacket)[i];
    panelPacket.checksum=c;
    SerialBT.write((byte*)&panelPacket,PANEL_PACKET_SIZE);
  }

  if(t-lastIndicatorSend>indicatorInterval){
    lastIndicatorSend=t;
    indicatorPacket={0xCC,0x22,75,98,0};
    byte c=0; for(int i=2;i<INDICATOR_PACKET_SIZE-1;i++) c+=((byte*)&indicatorPacket)[i];
    indicatorPacket.checksum=c;
    SerialBT.write((byte*)&indicatorPacket,INDICATOR_PACKET_SIZE);
  }

  if(t-lastPlotSend>plotInterval){
    lastPlotSend=t;
    float s=t/1000.0f;
    plotPacket={0xCC,0x33,3,
      (byte)(((sin(s)*0.4)+0.6)*255),
      (byte)(((cos(s)*0.2)+0.25)*255),
      (byte)(((cos(s)*0.2)+0.5)*255),0};
    byte c=0; for(int i=2;i<PLOT_PACKET_SIZE-1;i++) c+=((byte*)&plotPacket)[i];
    plotPacket.checksum=c;
    SerialBT.write((byte*)&plotPacket,PLOT_PACKET_SIZE);
  }
}
