/*
  bluetooth.h
  ------------------------------------------------------
  Cabecera central para uso de Bluetooth Serial (SPP) en ESP32.

  Responsabilidades:
  -----------------
  • Declarar el objeto global SerialBT para usarlo en todos los módulos
  • Definir el nombre del dispositivo Bluetooth
  • Incluir la librería necesaria

  Notas:
  ------
  • Solo contiene declaraciones y constantes, no asigna memoria
  • Se usa include guard para evitar definiciones múltiples
*/

#ifndef BLUETOOTH_H
#define BLUETOOTH_H
// Include guard: evita incluir este archivo más de una vez
// Protege de inclusiones múltiples

#include <BluetoothSerial.h>
extern BluetoothSerial SerialBT;
#define DEVICE_NAME "ESP32-BT"

#endif