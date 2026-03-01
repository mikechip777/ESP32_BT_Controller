#include <Arduino.h>
#include "bluetooth.h"
#include "packets.h"
#include "telemetry.h"
#include "receiver.h"
#include "debug.h"
#include "board_init.h"
#include "input.h"
#include "control.h"
#include "i2c_sensors.h"
#include "debug_config.h"
#include "i2c_bus.h"


static void serialInit() {
  Serial.begin(115200);
  delay(1000);

#if DEBUG_ENABLED
  Serial.println("Booting...");
#endif
}


static void bluetoothInit() {
  SerialBT.begin(DEVICE_NAME);

#if DEBUG_ENABLED
  Serial.println("ESP32 Bluetooth Receiver Ready.");
  Serial.printf("Listening for State (%d) and Event (%d)\n",
                STATE_PACKET_SIZE, EVENT_PACKET_SIZE);
#endif
}

static void hardwareInit() {

  boardInit();

  i2cBusInit();

#if USE_MCP23017
  mcpInit();
#endif

#if USE_I2C_SENSORS
  i2cSensorsInit();
#endif
}

void systemInit() {
  serialInit();
  bluetoothInit();
  hardwareInit();
}

void systemLoop() {
  handleBluetooth();
  sendTelemetryIfDue();
  controlUpdate();
  inputUpdate();
  sendI2CTelemetry();

#if DBG_STICKS
  debugStickLX();
  debugStickLY();
  debugStickRX();
  debugStickRY();
#endif

#if DBG_EVENTS
  printEventPacketOnPress();
#endif


#if DBG_KNOBS
  debugKnobL();
  debugKnobR();
#endif

#if DBG_SWITCHES
  debugSwitches();
#endif
}