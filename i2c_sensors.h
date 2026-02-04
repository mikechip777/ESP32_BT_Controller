#ifndef I2C_SENSORS_H
#define I2C_SENSORS_H

#include <Arduino.h>

/* =====================================================
   I2C SENSOR TELEMETRY MODULE

   ENGLISH:
   --------
   This module reads I2C sensors (temperature, IMU, etc.)
   and sends them as a telemetry packet to the Android app.

   It does NOT parse incoming data.
   It only sends telemetry.

   Packet ID:
     0xCC 0x66

   Functions:
   ----------
   void i2cInit();
   void sendI2CTelemetry();

   ESPAÑOL:
   --------
   Este módulo lee sensores I2C (temperatura, IMU, etc.)
   y los envía como telemetría a la app Android.

   No procesa datos entrantes.
   Solo envía telemetría.
   ===================================================== */

void i2cInit();
void sendI2CTelemetry();

#endif
