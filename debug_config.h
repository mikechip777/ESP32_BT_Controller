#ifndef DEBUG_CONFIG_H
#define DEBUG_CONFIG_H

/* =====================================================
   GENERAL DEBUG ENABLE
   ===================================================== */

#define DEBUG_ENABLED 1   // 1 = debug ON, 0 = debug OFF

#if DEBUG_ENABLED
  #define DBG_STICKS   1
  #define DBG_KNOBS    1
  #define DBG_SWITCHES 1
  #define DBG_EVENTS   1
#else
  #define DBG_STICKS   0
  #define DBG_KNOBS    0
  #define DBG_SWITCHES 0
  #define DBG_EVENTS   0
#endif


/* =====================================================
   TELEMETRY DEBUG MODES
   ===================================================== */

#define DBG_NONE        0
#define DBG_PANEL       1
#define DBG_INDICATOR   2
#define DBG_PLOT        3

// Select exactly one telemetry mode:
#define TELEMETRY_DEBUG_MODE DBG_PLOT

#endif
