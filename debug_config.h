#ifndef DEBUG_CONFIG_H
#define DEBUG_CONFIG_H

#define DEBUG_MODE 1   // 1 = debug ON, 0 = debug OFF

#if DEBUG_MODE
  #define DBG_STICKS   1
  #define DBG_KNOBS    0
  #define DBG_SWITCHES 0
  #define DBG_EVENTS   0
#else
  #define DBG_STICKS   0
  #define DBG_KNOBS    0
  #define DBG_SWITCHES 0
  #define DBG_EVENTS   0
#endif

#endif