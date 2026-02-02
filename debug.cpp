/*
  debug.cpp
  ------------------------------------------------------
  Implementa impresión formateada por Serial.

  Responsabilidades:
  ------------------
  • Decodificar campos de bits
  • Mostrar valores claramente
  • Hacer legible la depuración

  Este archivo NUNCA maneja Bluetooth ni memoria.
*/
#include "debug.h"
// Incluye prototipos y variables globales


// ----------------------------------------------------
// printStatePacket()
// Imprime el último paquete RC recibido
// ----------------------------------------------------
void printStatePacket() {

  Serial.println("\n--- RC STATE PACKET ---");
  // Separador visual

  Serial.printf("L Stick: X=%u  Y=%u\n",
                rcStatePacket.data.leftStickX,
                rcStatePacket.data.leftStickY);
  // Joystick izquierdo

  Serial.printf("R Stick: X=%u  Y=%u\n",
                rcStatePacket.data.rightStickX,
                rcStatePacket.data.rightStickY);
  // Joystick derecho

  Serial.printf("Knobs:   L=%u  R=%u\n",
                rcStatePacket.data.leftKnob,
                rcStatePacket.data.rightKnob);
  // Perillas analógicas

  byte s = rcStatePacket.data.switches;
  // Guarda los interruptores

  Serial.printf("Switch Byte: 0x%02X\n", s);
  // Muestra en hexadecimal

  for (int i = 0; i < 6; i++) {
    Serial.printf("  S%d = %s\n",
                  i + 1,
                  (s & (1 << i)) ? "ON" : "OFF");
  }
  // Decodifica cada bit

  Serial.printf("Checksum: %u\n", rcStatePacket.data.checksum);
  // Muestra checksum

  Serial.println("------------------------");
}


// ----------------------------------------------------
// printEventPacket()
// Imprime paquete de evento
// ----------------------------------------------------
void printEventPacket() {

  Serial.println("\n--- EVENT PACKET ---");

  Serial.printf("Event ID: 0x%02X\n", rcEventPacket.data.eventId);
  // ID del evento

  Serial.printf("Checksum: 0x%02X\n", rcEventPacket.data.checksum);
  // Checksum en hex

  Serial.println("---------------------");
}


// ----------------------------------------------------
// printPanelPacket()
// Imprime telemetría de panel
// ----------------------------------------------------
void printPanelPacket() {

  Serial.println("\n--- PANEL TELEMETRY ---");

  Serial.printf("Left: %u  Right: %u\n",
                panelPacket.leftPanelValue,
                panelPacket.rightPanelValue);
  // Valores del display

  Serial.printf("States: 0x%02X\n", panelPacket.panelStates);
  // Estados de LEDs

  Serial.printf("Checksum: %u\n", panelPacket.checksum);
}


// ----------------------------------------------------
// printIndicatorPacket()
// Imprime indicadores
// ----------------------------------------------------
void printIndicatorPacket() {

  Serial.println("\n--- INDICATOR TELEMETRY ---");

  Serial.printf("Analog: %u\n", indicatorPacket.analogValue);
  // Sensor analógico

  Serial.printf("Battery: %u%%\n", indicatorPacket.batteryLevel);
  // Nivel de batería

  Serial.printf("Checksum: %u\n", indicatorPacket.checksum);
}


// ----------------------------------------------------
// printPlotPacket()
// Imprime valores de gráficos
// ----------------------------------------------------
void printPlotPacket() {

  Serial.println("\n--- PLOT TELEMETRY ---");

  Serial.printf("Plots: %u\n", plotPacket.numPlots);
  // Cantidad de valores

  Serial.printf("P1=%u  P2=%u  P3=%u\n",
                plotPacket.plotValue1,
                plotPacket.plotValue2,
                plotPacket.plotValue3);
  // Valores

  Serial.printf("Checksum: %u\n", plotPacket.checksum);
}