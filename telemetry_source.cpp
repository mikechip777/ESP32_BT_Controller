#include <Arduino.h>
#include "debug_config.h"
#include "telemetry_source.h"
#include "input_hw.h"

/* =====================================================
   DEBUG VARIABLES
   ===================================================== */

#if TELEMETRY_DEBUG_MODE == DBG_PANEL
static uint16_t dbgLeft  = 0;
static uint16_t dbgRight = 0;
#endif

#if TELEMETRY_DEBUG_MODE == DBG_INDICATOR
static uint8_t dbgIndicatorValue = 0;
static uint8_t dbgBatteryValue   = 0;
static uint8_t dbgDigitalMask    = 0;
#endif

#if TELEMETRY_DEBUG_MODE == DBG_PLOT
static uint8_t dbgPlot1 = 0;
static uint8_t dbgPlot2 = 0;
static uint8_t dbgPlot3 = 0;
#endif


/* =====================================================
   SERIAL DEBUG INPUT
   ===================================================== */

void telemetrySourceUpdate() {

#if TELEMETRY_DEBUG_MODE == DBG_NONE
    return;
#endif

    if (!Serial.available())
        return;

    String line = Serial.readStringUntil('\n');
    line.trim();

#if TELEMETRY_DEBUG_MODE == DBG_PANEL

    int comma = line.indexOf(',');
    if (comma < 0) return;

    int l = line.substring(0, comma).toInt();
    int r = line.substring(comma + 1).toInt();

    if (l >= 0 && l <= 9999 && r >= 0 && r <= 9999) {
        dbgLeft  = l;
        dbgRight = r;
    }

#elif TELEMETRY_DEBUG_MODE == DBG_INDICATOR

    int c1 = line.indexOf(',');
    int c2 = line.lastIndexOf(',');

    if (c1 < 0 || c2 <= c1) return;

    int a = line.substring(0, c1).toInt();
    int b = line.substring(c1 + 1, c2).toInt();
    int d = line.substring(c2 + 1).toInt();

    if (a >= 0 && a <= 100 &&
        b >= 0 && b <= 100 &&
        d >= 0 && d <= 255) {

        dbgIndicatorValue = a;
        dbgBatteryValue   = b;
        dbgDigitalMask    = (uint8_t)d;
    }

#elif TELEMETRY_DEBUG_MODE == DBG_PLOT

    int c1 = line.indexOf(',');
    int c2 = line.lastIndexOf(',');

    if (c1 < 0 || c2 <= c1) return;

    int v1 = line.substring(0, c1).toInt();
    int v2 = line.substring(c1 + 1, c2).toInt();
    int v3 = line.substring(c2 + 1).toInt();

    if (v1 >= 0 && v1 <= 255 &&
        v2 >= 0 && v2 <= 255 &&
        v3 >= 0 && v3 <= 255) {

        dbgPlot1 = v1;
        dbgPlot2 = v2;
        dbgPlot3 = v3;
    }

#endif
}


/* =====================================================
   PANEL
   ===================================================== */

uint16_t getPanelLeft() {

#if TELEMETRY_DEBUG_MODE == DBG_PANEL
    return dbgLeft;

#elif TELEMETRY_DEBUG_MODE == DBG_PLOT
    return 0;

#else
    return hwReadNumeric1();
#endif
}

uint16_t getPanelRight() {

#if TELEMETRY_DEBUG_MODE == DBG_PANEL
    return dbgRight;

#elif TELEMETRY_DEBUG_MODE == DBG_PLOT
    return 0;

#else
    return hwReadNumeric2();
#endif
}


/* =====================================================
   INDICATOR
   ===================================================== */

uint8_t getIndicatorAnalog() {

#if TELEMETRY_DEBUG_MODE == DBG_INDICATOR
    return dbgIndicatorValue;

#elif TELEMETRY_DEBUG_MODE == DBG_PLOT
    return 50;

#else
    return hwReadIndicatorAnalog();
#endif
}

uint8_t getIndicatorBattery() {

#if TELEMETRY_DEBUG_MODE == DBG_INDICATOR
    return dbgBatteryValue;

#elif TELEMETRY_DEBUG_MODE == DBG_PLOT
    return 75;

#else
    return hwReadIndicatorBattery();
#endif
}

uint8_t getIndicatorDigitalMask() {

#if TELEMETRY_DEBUG_MODE == DBG_INDICATOR
    return dbgDigitalMask;
#else
    return hwReadDigitalMask();
#endif
}


/* =====================================================
   PLOT
   ===================================================== */

uint8_t getPlot1() {

#if TELEMETRY_DEBUG_MODE == DBG_PLOT
    return dbgPlot1;
#else
    return hwReadPlot1();
#endif
}

uint8_t getPlot2() {

#if TELEMETRY_DEBUG_MODE == DBG_PLOT
    return dbgPlot2;
#else
    return hwReadPlot2();
#endif
}

uint8_t getPlot3() {

#if TELEMETRY_DEBUG_MODE == DBG_PLOT
    return dbgPlot3;
#else
    return hwReadPlot3();
#endif
}