#ifndef PINS_H
#define PINS_H

#if PROJECT_MODE == FULL_RC_MODE

/* =====================================================
   I2C BUS (reserved)
   ===================================================== */
#define PIN_I2C_SDA 21
#define PIN_I2C_SCL 22


/* =====================================================
   PWM OUTPUTS (6 total)
   ===================================================== */
#define PIN_PWM_STICK_LX 18
#define PIN_PWM_STICK_LY 19
#define PIN_PWM_STICK_RX 23
#define PIN_PWM_STICK_RY 25
#define PIN_PWM_KNOB_L   26
#define PIN_PWM_KNOB_R   27


/* =====================================================
   SWITCH OUTPUTS (6)
   ===================================================== */
#define PIN_SW1 16
#define PIN_SW2 17
#define PIN_SW3 13
#define PIN_SW4 14
#define PIN_SW5 12   // keep LOW at boot
#define PIN_SW6 15   // keep LOW at boot


/* =====================================================
   ANALOG INPUTS (ADC1 ONLY — SAFE WITH BT)
   ===================================================== */
#define PIN_NUMERIC1      32
#define PIN_NUMERIC2      33
#define PIN_ANALOG_IND1   34
#define PIN_ANALOG_IND2   35


/* =====================================================
   DIGITAL INPUTS (4 indicators instead of 8)
   ===================================================== */
#define PIN_IND1 4
#define PIN_IND2 5
#define PIN_IND3 2
#define PIN_IND4 0   // careful: avoid pull-up at boot

#endif
/* ========================================================================================================== */




#if PROJECT_MODE == MODE_BASIC_RC

/* ================= I2C ================= */
#define PIN_I2C_SDA 21
#define PIN_I2C_SCL 22

/* ================= STICKS (PWM) ================= */
/* 4 stick axes */
#define PIN_PWM_LX   18
#define PIN_PWM_LY   19
#define PIN_PWM_RX   23
#define PIN_PWM_RY   25

/* ================= SWITCHES ================= */
#define PIN_SW1  26
#define PIN_SW2  27
#define PIN_SW3  32
#define PIN_SW4  33

/* ================= ANALOG INPUTS (ADC1 SAFE) ================= */
#define PIN_ANALOG1 32
#define PIN_ANALOG2 33
#define PIN_ANALOG3 34
#define PIN_ANALOG4 35
#endif



#if PROJECT_MODE == MODE_SENSOR_NODE

/* ================= I2C ================= */
#define PIN_I2C_SDA 21
#define PIN_I2C_SCL 22

/* ================= STICKS (PWM) ================= */
#define PIN_PWM_LX   18
#define PIN_PWM_LY   19
#define PIN_PWM_RX   23
#define PIN_PWM_RY   25

/* ================= SWITCHES ================= */
#define PIN_SW1  26
#define PIN_SW2  27

/* ================= ANALOG INPUTS (ADC1 ONLY) ================= */
#define PIN_ADC1 32
#define PIN_ADC2 33
#define PIN_ADC3 34
#define PIN_ADC4 35

/* ================= DIGITAL INPUTS (Indicators) ================= */
/* 34–39 are input-only and safe */
#define PIN_IND1 36
#define PIN_IND2 39
#define PIN_IND3 4
#define PIN_IND4 5

#endif

#if PROJECT_MODE == MODE_MINIMAL

/* ================= I2C ================= */
#define PIN_I2C_SDA 21
#define PIN_I2C_SCL 22

/* ================= STICKS (2 AXES ONLY) ================= */
#define PIN_PWM_LX   18
#define PIN_PWM_LY   19

/* ================= SWITCHES ================= */
#define PIN_SW1  23
#define PIN_SW2  25

/* ================= ANALOG INPUT ================= */
#define PIN_ANALOG1 32

#endif

#endif