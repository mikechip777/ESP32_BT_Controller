#include "bluetooth.h"
// Include the bluetooth.h header to get the SerialBT declaration and DEVICE_NAME

BluetoothSerial SerialBT;
// Define the global SerialBT object
// This is the actual instance of the BluetoothSerial class
// All other files that include bluetooth.h can use this object via 'extern'