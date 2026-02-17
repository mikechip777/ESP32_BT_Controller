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
#include "debug_config.h"

void printPanelPacket();
void printIndicatorPacket();

#if DBG_STICKS
void debugStickLX();
void debugStickLY();
void debugStickRX();
void debugStickRY();
#endif

#if DBG_KNOBS
void debugKnobL();
void debugKnobR();
#endif

#if DBG_SWITCHES
void debugSwitches();
#endif

#if DBG_EVENTS
void printEventPacketOnPress();
#endif

#endif
