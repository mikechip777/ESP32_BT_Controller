/*
  receiver.h
  ------------------------------------------------------
  Declares the Bluetooth receive subsystem.

  Provides:
  ----------
  • handleBluetooth()

  Purpose:
  --------
  This module:
    - Reads raw Bluetooth bytes
    - Stores them in a rolling buffer
    - Searches for packet headers
    - Extracts complete packets
    - Verifies them
    - Calls debug print functions

  Called from:
  -------------
  ESP32_BT_Controller.ino -> loop()
*/
#ifndef RECEIVER_H
#define RECEIVER_H
void handleBluetooth();
#endif
