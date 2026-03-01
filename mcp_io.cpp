#include <Wire.h>
#include "mcp_io.h"

/* MCP23017 Registers */
#define IODIRA   0x00
#define IODIRB   0x01
#define GPIOA    0x12
#define GPIOB    0x13
#define OLATB    0x15

static uint8_t portBState = 0x00;

/* =====================================================
   Low Level Write
   ===================================================== */
static void mcpWriteRegister(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(MCP23017_ADDR);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

/* =====================================================
   Low Level Read
   ===================================================== */
static uint8_t mcpReadRegister(uint8_t reg) {
    Wire.beginTransmission(MCP23017_ADDR);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(MCP23017_ADDR, (uint8_t)1);
    return Wire.read();
}


/* =====================================================
   Initialization
   ===================================================== */
void mcpInit() {

    /* Port A → INPUT (Indicators) */
    mcpWriteRegister(IODIRA, 0xFF);

    /* Port B → OUTPUT (Switches / Events) */
    mcpWriteRegister(IODIRB, 0x00);

    /* Clear outputs */
    portBState = 0x00;
    mcpWriteRegister(GPIOB, portBState);
}

/* =====================================================
   Read 8 Digital Inputs (Port A)
   ===================================================== */
uint8_t mcpReadDigitalInputs() {
    return mcpReadRegister(GPIOA);
}

/* =====================================================
   Write Single Output (Port B)
   pin: 0–7  (mapped to MCP pins 8–15)
   ===================================================== */
void mcpWriteOutput(uint8_t pin, bool state) {

    if (pin > 7) return;

    if (state)
        portBState |= (1 << pin);
    else
        portBState &= ~(1 << pin);

    mcpWriteRegister(GPIOB, portBState);
}

/* =====================================================
   Write Entire Port B at Once
   ===================================================== */
void mcpWritePortB(uint8_t value) {
    portBState = value;
    mcpWriteRegister(GPIOB, portBState);
}