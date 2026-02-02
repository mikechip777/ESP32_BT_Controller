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
// Incluye la librería BluetoothSerial de ESP32
// Permite la comunicación SPP (Serial Port Profile) en ESP32

extern BluetoothSerial SerialBT;
// Declaración del objeto global SerialBT (la instancia se define en el .ino o .cpp principal)
// Permite usar SerialBT en varios archivos sin redefinirlo

#define DEVICE_NAME "ESP32-BT"
// Nombre que el ESP32 anunciará por Bluetooth
// Visible desde Android u otros dispositivos Bluetooth

#endif