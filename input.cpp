#include <Arduino.h>
#include "bluetooth.h"
#include "packets.h"
#include "input.h"
#include "pins.h"

static unsigned long lastSend = 0;
const unsigned long inputInterval = 50;


void inputUpdate() {
  if (!SerialBT.hasClient()) return;
  if (millis() - lastSend < inputInterval) return;
  lastSend = millis();

  inputPacket.h1 = 0xCC;
  inputPacket.h2 = 0x55;

  inputPacket.a34 = analogRead(PIN_ANALOG1);
  inputPacket.a35 = analogRead(PIN_ANALOG2);
  inputPacket.a36 = analogRead(PIN_ANALOG3);
  inputPacket.a39 = analogRead(PIN_ANALOG4);

  inputPacket.d0  = !digitalRead(PIN_IND1);
  inputPacket.d16 = !digitalRead(PIN_IND2);
  inputPacket.d17 = !digitalRead(PIN_IND3);

  byte c = 0;
  for (int i = 2; i < INPUT_PACKET_SIZE - 1; i++)
    c += ((byte*)&inputPacket)[i];

  inputPacket.checksum = c;

  SerialBT.write((byte*)&inputPacket, INPUT_PACKET_SIZE);
}

uint8_t getDigitalInputsMask() {
    uint8_t mask = 0;

    if (digitalRead(PIN_IND1)) mask |= (1 << 0);
    if (digitalRead(PIN_IND2)) mask |= (1 << 1);
    if (digitalRead(PIN_IND3)) mask |= (1 << 2);
    if (digitalRead(PIN_IND4)) mask |= (1 << 3);
    if (digitalRead(PIN_IND1)) mask |= (1 << 4);
    if (digitalRead(PIN_IND2)) mask |= (1 << 5);
    if (digitalRead(PIN_IND3)) mask |= (1 << 6);
    if (digitalRead(PIN_IND4)) mask |= (1 << 7);

    return mask;
}