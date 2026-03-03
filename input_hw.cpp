#include <Arduino.h>
#include "input_hw.h"
#include "pins.h"

/* =====================================================
   INTERNAL DIGITAL MASK BUILDER (Telemetry side)
   ===================================================== */

static uint8_t buildDigitalMaskInternal() {
    uint8_t mask = 0;

    // Active LOW example (change if needed)
    if (!digitalRead(PIN_IND1)) mask |= (1 << 0);
    if (!digitalRead(PIN_IND2)) mask |= (1 << 1);
    if (!digitalRead(PIN_IND3)) mask |= (1 << 2);
    if (!digitalRead(PIN_IND4)) mask |= (1 << 3);

    return mask;
}

/* =====================================================
   CONTROLLER INPUT READ (Raw input structure)
   ===================================================== */

uint8_t buildDigitalMask() {
    uint8_t mask = 0;

    if (digitalRead(PIN_IND1)) mask |= (1 << 0);
    if (digitalRead(PIN_IND2)) mask |= (1 << 1);
    if (digitalRead(PIN_IND3)) mask |= (1 << 2);
    if (digitalRead(PIN_IND4)) mask |= (1 << 3);
    if (digitalRead(PIN_IND1)) mask |= (1 << 4);
    if (digitalRead(PIN_IND2)) mask |= (1 << 5);
    if (digitalRead(PIN_IND3)) mask |= (1 << 6);
    if (digitalRead(PIN_IND4)) mask |= (1 << 7);

    return mask;
}

void inputHwRead(InputRaw& out) {

    /* -------- Analog Inputs -------- */

    out.analog1 = analogRead(PIN_ANALOG1);
    out.analog2 = analogRead(PIN_ANALOG2);
    out.analog3 = analogRead(PIN_ANALOG3);
    out.analog4 = analogRead(PIN_ANALOG4);

    /* -------- Digital Inputs -------- */

    uint8_t d1 = digitalRead(PIN_IND1);
    uint8_t d2 = digitalRead(PIN_IND2);
    uint8_t d3 = digitalRead(PIN_IND3);
    uint8_t d4 = digitalRead(PIN_IND4);

    out.d0  = d1;
    out.d16 = d2;
    out.d17 = d3;

    /* -------- Digital Mask -------- */

    uint8_t mask = 0;

    if (d1) mask |= (1 << 0);
    if (d2) mask |= (1 << 1);
    if (d3) mask |= (1 << 2);
    if (d4) mask |= (1 << 3);
    if (d1) mask |= (1 << 4);
    if (d2) mask |= (1 << 5);
    if (d3) mask |= (1 << 6);
    if (d4) mask |= (1 << 7);

   // out.digitalMask = mask;
}

/* =====================================================
   TELEMETRY HARDWARE READS
   ===================================================== */

uint16_t hwReadNumeric1() {
    return map(analogRead(PIN_NUMERIC1), 0, 4095, 0, 9999);
}

uint16_t hwReadNumeric2() {
    return map(analogRead(PIN_NUMERIC2), 0, 4095, 0, 9999);
}

uint8_t hwReadIndicatorAnalog() {
    return map(analogRead(PIN_ANALOG_IND1), 0, 4095, 0, 100);
}

uint8_t hwReadIndicatorBattery() {
    return map(analogRead(PIN_ANALOG_IND2), 0, 4095, 0, 100);
}

uint8_t hwReadPlot1() {
    return map(analogRead(PIN_ANALOG1), 0, 4095, 0, 255);
}

uint8_t hwReadPlot2() {
    return map(analogRead(PIN_ANALOG2), 0, 4095, 0, 255);
}

uint8_t hwReadPlot3() {
    return map(analogRead(PIN_ANALOG3), 0, 4095, 0, 255);
}

uint8_t hwReadDigitalMask() {
    return buildDigitalMaskInternal();
}
