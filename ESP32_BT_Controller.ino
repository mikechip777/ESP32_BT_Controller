/*
  ESP32_BT_Controller.ino
  ------------------------------------------------------
  Main entry point for the ESP32 Bluetooth Controller.

  Responsibilities:
  -------------------
  • Initializes Serial and Bluetooth (SPP).
  • Calls subsystem handlers in the main loop:
      - handleBluetooth()   -> receive + decode packets
      - telemetrySendLoop() -> send telemetry packets
  • Tracks Bluetooth connection state.
  • Does NOT implement logic directly (separation of concerns).

  File Interaction:
  -----------------
  - packets.h      -> packet structures and globals
  - receiver.h     -> Bluetooth RX logic
  - telemetry.h    -> Telemetry TX logic
  - debug.h        -> Serial print utilities

  Data Flow:
  ----------
  Android -> Bluetooth -> receiver.cpp -> packets.h structs
                                 |
                                 v
                            debug.cpp prints

  ESP32 -> telemetry.cpp -> Bluetooth -> Android

*/
#include "system_init.h"
#include "pulse.h"

void setup() {
  systemInit();
}

void loop() {
  systemLoop();
  pulseUpdate();

#if USE_MCP23017
  mcpPulseUpdate();
#endif
}
