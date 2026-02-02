/*
  debug.h
  ------------------------------------------------------
  Debug and diagnostic utilities.

  Declares:
  ----------
  • printStatePacket()
  • printEventPacket()

  Purpose:
  --------
  Keeps all Serial output formatting
  out of the logic files.
*/
#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>
#include "packets.h"

void printStatePacket();
void printEventPacket();
void printPanelPacket();
void printIndicatorPacket();
void printPlotPacket();

#endif
