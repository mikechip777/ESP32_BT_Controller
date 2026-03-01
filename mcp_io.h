#ifndef MCP_IO_H
#define MCP_IO_H

#include <Arduino.h>

/* MCP23017 I2C Address */
#define MCP23017_ADDR 0x20

/* =============================
   Initialization
   ============================= */
void mcpInit();

/* =============================
   Digital Inputs (Indicators)
   Reads Port A (GPIO 0–7)
   ============================= */
uint8_t mcpReadDigitalInputs();

/* =============================
   Digital Outputs (Switches / Events)
   Writes to Port B (GPIO 8–15)
   ============================= */
void mcpWriteOutput(uint8_t pin, bool state);
void mcpWritePortB(uint8_t value);

#endif