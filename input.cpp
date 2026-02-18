#include <Arduino.h>
#include "bluetooth.h"
#include "packets.h"
#include "input.h"
#include "pins.h"

static unsigned long lastSend = 0;
const unsigned long inputInterval = 50;

void inputInit() {
  pinMode(PIN_D0,  INPUT_PULLUP);
  pinMode(PIN_D16, INPUT_PULLUP);
  pinMode(PIN_D17, INPUT_PULLUP);

  pinMode(PIN_A34, INPUT);
  pinMode(PIN_A35, INPUT);
  pinMode(PIN_A36, INPUT);
  pinMode(PIN_A39, INPUT);
}

void inputUpdate() {
  if (!SerialBT.hasClient()) return;
  if (millis() - lastSend < inputInterval) return;
  lastSend = millis();

  inputPacket.h1 = 0xCC;
  inputPacket.h2 = 0x55;

  inputPacket.a34 = analogRead(PIN_A34);
  inputPacket.a35 = analogRead(PIN_A35);
  inputPacket.a36 = analogRead(PIN_A36);
  inputPacket.a39 = analogRead(PIN_A39);

  inputPacket.d0  = !digitalRead(PIN_D0);
  inputPacket.d16 = !digitalRead(PIN_D16);
  inputPacket.d17 = !digitalRead(PIN_D17);

  byte c = 0;
  for (int i = 2; i < INPUT_PACKET_SIZE - 1; i++)
    c += ((byte*)&inputPacket)[i];

  inputPacket.checksum = c;

  SerialBT.write((byte*)&inputPacket, INPUT_PACKET_SIZE);
}