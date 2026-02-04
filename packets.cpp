/*
  packets.cpp
  ------------------------------------------------------
  This file provides the actual global packet instances
  declared in packets.h.

  Why this exists:
  ----------------
  Headers should not allocate memory.
  This file prevents multiple-definition linker errors.

  Contents:
  ---------
  RcPacketUnion   rcStatePacket;
  EventPacketUnion rcEventPacket;
  PanelPacket     panelPacket;
  IndicatorPacket indicatorPacket;
  PlotPacket      plotPacket;
*/
#include "packets.h"

StatePacketUnion rcStatePacket;
const int STATE_PACKET_SIZE = sizeof(RcPacket);

EventPacketUnion rcEventPacket;
const int EVENT_PACKET_SIZE = sizeof(EventPacket);

PanelPacket panelPacket;
const int PANEL_PACKET_SIZE = sizeof(PanelPacket);

IndicatorPacket indicatorPacket;
const int INDICATOR_PACKET_SIZE = sizeof(IndicatorPacket);

PlotPacket plotPacket;
const int PLOT_PACKET_SIZE = sizeof(PlotPacket);

InputPacket inputPacket;

I2CPacket i2cPacket;