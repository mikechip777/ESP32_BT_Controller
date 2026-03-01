#include <Wire.h>
#include "pins.h"
#include "i2c_bus.h"

void i2cBusInit() {
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
}