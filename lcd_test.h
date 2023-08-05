/* 
 * File:                lcd_test.h
 * Author:              J. Striebel
 * Project:             Weather Station
 * Device:              PIC18F47Q10 @ 16 MHz internal OSC, 5V
 * Platform:            Curiosity HPC board (DM164136)
 * Display:             LCD 16x2 - Hitachi HD44780
 * Compiler:            XC8 (v2.41)
 * IDE:                 MPLAB X (v6.10), MCC (5.3.7)
 * Program version:     1.0
 
 * Program Description:
 * --------------------
 * This module contains test cases for HD44780 API functions provided by lcd.c 
 * 
 * Hardware Description:
 * --------------------
 * The Curiosity HPC board is wired to the HD44780 display as follows:
 * 
 * RD0  ---->   LED+ (pin 16)   LCD backlight
 * RD1  ---->   RS (pin 4)      Selecting register: 1 = data, 2 = instruction
 * RD2  ---->   R/W (pin 5)     High = read mode, low = write mode
 * RD3  ---->   E (pin 6)       Enable signal for writing/reading data
 * RD4  ---->   DB4 (pin 11)    4-bit bi-directional data bus
 * RD5  ---->   DB5 (pin 12)    4-bit bi-directional data bus
 * RD6  ---->   DB6 (pin 13)    4-bit bi-directional data bus
 * RD7  ---->   DB7 (pin 14)    4-bit bi-directional data bus
 *    
 */

#ifndef LCD_TEST_H
#define	LCD_TEST_H

#ifdef	__cplusplus
extern "C" {
#endif

// Include and defines
#include "mcc_generated_files/mcc.h"
#include <stdbool.h>
#include <string.h>

extern void LCD_TestRoutine(void);


#ifdef	__cplusplus
}
#endif

#endif	/* LCD_HD447780_H */

