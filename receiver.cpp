/*
  receiver.cpp
  ------------------------------------------------------
  Bluetooth RX and packet decoder.

  Responsibilities:
  -----------------
  • Read bytes from SerialBT.
  • Maintain a sliding buffer.
  • Detect:
        AA 55 -> State packet
        AA 66 -> Event packet
  • Extract full packets.
  • Copy into union structs.
  • Call:
        printStatePacket()
        printEventPacket()

  Key Design:
  -----------
  Uses a rolling buffer to survive:
    - Fragmented packets
    - Back-to-back packets
    - Noise

  This file NEVER prints raw bytes directly.
  It always uses debug.cpp helpers.
*/
#include <Arduino.h>
#include "bluetooth.h"
#include "packets.h"
#include "receiver.h"
#include "debug.h"
#include "control.h"

#define RX_BUFFER_SIZE 64

void handleBluetooth() {
    static byte buffer[RX_BUFFER_SIZE];
    static int bytesRead = 0;

    // --- Read incoming Bluetooth bytes ---
    while (SerialBT.available()) {
        if (bytesRead < RX_BUFFER_SIZE) {
            buffer[bytesRead++] = SerialBT.read();
        } else {
            // shift buffer if full
            memmove(buffer, buffer + 1, RX_BUFFER_SIZE - 1);
            buffer[RX_BUFFER_SIZE - 1] = SerialBT.read();
        }
    }

    // --- Process packets in buffer ---
    while (true) {
    int packetStart = -1;
    int packetType = 0; // 1 = STATE, 2 = EVENT

    for (int i = 0; i <= bytesRead - 2; i++) {

        // --- STATE packet header ---
        if (buffer[i] == 0xAA && buffer[i + 1] == 0x55) {
            if (bytesRead - i >= STATE_PACKET_SIZE) {
                packetStart = i;
                packetType = 1;
                break;
            }
        }

        // --- EVENT packet header ---
        if (buffer[i] == 0xBB && buffer[i + 1] == 0x66) {
            if (bytesRead - i >= EVENT_PACKET_SIZE) {
                packetStart = i;
                packetType = 2;
                break;
            }
        }
    }

    if (packetStart == -1) break;

    if (packetType == 1) {
        memcpy(rcStatePacket.bytes, &buffer[packetStart], STATE_PACKET_SIZE);
        Serial.println("State packet received");
        printStatePacket();
    }
    else if (packetType == 2) {
        memcpy(rcEventPacket.bytes, &buffer[packetStart], EVENT_PACKET_SIZE);
        Serial.println("Event packet received");
        printEventPacket();
        controlHandleEvent(rcEventPacket.data.eventId);
    }

    int removeCount = packetStart +
        (packetType == 1 ? STATE_PACKET_SIZE : EVENT_PACKET_SIZE);

    memmove(buffer, buffer + removeCount, bytesRead - removeCount);
    bytesRead -= removeCount;
}
}
