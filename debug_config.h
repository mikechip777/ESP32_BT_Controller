#ifndef DEBUG_CONFIG_H
#define DEBUG_CONFIG_H

#define DEBUG_MODE 0   // 1 = debug ON, 0 = debug OFF

#if DEBUG_MODE
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
   DEBUG TELEMETRY MODES
   ===================================================== */
// set ONE of these to 1, others to 0
#define DBG_NONE        0
#define DBG_PANEL      0
#define DBG_INDICATOR  1

#define DEBUG_MODE  DBG_INDICATOR   // <<< change here


#endif