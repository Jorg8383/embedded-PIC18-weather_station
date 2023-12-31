/* 
 * File:                lcd_app.h
 * Author:              J. Striebel
 * Project:             Weather Station
 * Device:              PIC18F47Q10 @ 16 MHz internal OSC, 5V
 * Platform:            Curiosity HPC board (DM164136)
 * Display:             LCD 16x2 - Hitachi HD44780
 * Compiler:            XC8 (v2.41)
 * IDE:                 MPLAB X (v6.10), MCC (5.3.7)
 * 
 * Description:
 * ------------
 * This module comprises project specific definitions and functionality to 
 * interface the Hitachi HD44780 LCD (16 x 2 lines).
 * 
 */

#ifndef LCD_APP_H
#define	LCD_APP_H

#include "mcc_generated_files/mcc.h"
#include "lcd.h"

#define LCD_CHAR_LENGTH 16 // LCD 16x2 lines display
#define LCD_TEMPERATURE_BUFFER_SIZE    20

// Indices for LCD text messages
typedef enum {
    LCD_TXT_WELCOME,
    LCD_TXT_TEMPERATURE,
    LCD_TXT_PRESSURE,
    LCD_TXT_PRESSURE_UNIT,
    LCD_TXT_ALTITUDE,
    LCD_TXT_ALTITUDE_UNIT, 
    LCD_TXT_ERROR_SENSOR,
    LCD_TXT_WEATHER_TREND,
    LCD_TXT_TREND_UPWARD,
    LCD_TXT_TREND_DOWNWARD,
    LCD_TXT_TREND_STABLE,
    LCD_TXT_DELTA_PRESSURE,
    LCD_NUM_MESSAGES // This entry has to be the last!   
} LcdTextIndex;

// Function prototypes
void convertTemperatureToString(int16_t temperature, char *buffer);
char* itoa(int32_t num, char str[], int16_t maxSize);
const char *getLcdText(LcdTextIndex txtIndex);


#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* LCD_APP_H */

