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
#include <Arduino.h>
#include "bluetooth.h"
#include "packets.h"
#include "telemetry.h"
#include "receiver.h"
#include "debug.h"
#include "control.h"

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Booting...");

  SerialBT.begin(DEVICE_NAME);
  Serial.println("ESP32 Bluetooth Receiver Ready.");
  Serial.printf("Listening for State (%d) and Event (%d)\n",
                STATE_PACKET_SIZE, EVENT_PACKET_SIZE);
  controlInit();
}

void loop() {
  handleBluetooth();
  sendTelemetryIfDue();
  controlUpdate();
}
