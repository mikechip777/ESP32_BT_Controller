/*
  telemetry.h
  ------------------------------------------------------
  Telemetry transmission interface.

  Declares:
  ----------
  • telemetrySendLoop()
  • sendPanelTelemetry()
  • sendIndicatorTelemetry()
  • sendPlotTelemetry()
  • sendConfigTelemetry()

  Purpose:
  --------
  Periodically sends telemetry packets
  to the Android app over Bluetooth.
*/
#ifndef TELEMETRY_H
#define TELEMETRY_H
void sendTelemetryIfDue();
void sendConfigTelemetry();
#endif
