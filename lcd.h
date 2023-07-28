/* 
 * File:                lcd.h
 * Author:              J. Striebel
 * Project:             Weather Station
 * Device:              PIC18F47Q10 @ 16 MHz internal OSC, 5V 
 * Platform:            Curiosity HPC board (DM164136)
 * Compiler:            XC8 (v2.41)
 * IDE:                 MPLAB X (v6.10), MCC (5.3.7)
 * Program version:     1.0
 
 * Program Description:
 * --------------------
 * This header file declares routines for controlling the Hitachi HD44780
 * LCD 16x2 display via a 4-bit interface, where one data byte is transferred
 * sequentially by four bus-lines (DB4...DB7). First the higher nibble (4-bit)
 * data is transferred, followed by the lower nibble.
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

#ifndef LCD_HD447780_H
#define	LCD_HD447780_H

#ifdef	__cplusplus
extern "C" {
#endif

// Include and defines
#include <xc.h>
#include "mcc_generated_files/pin_manager.h"

#define LCD_BL      LATDbits.LATD0  // LCD backlight LED
#define LCD_RS      LATDbits.LATD1  // 1 = data register, 2 = instruction register
#define LCD_RW      LATDbits.LATD2  // 1 = read mode, 2 = write mode
#define LCD_EN      LATDbits.LATD3  // enable signal for writing/reading data
#define LCD_DB4     LATDbits.LATD4  
#define LCD_DB5     LATDbits.LATD5
#define LCD_DB6     LATDbits.LATD6
#define LCD_DB7     LATDbits.LATD7
#define LCD_BUSY    PORTDbits.RD7   // busy flag
    
// Function prototypes
void LCD_Initialise(void);
void LCD_PrintCharacter(uint8_t var);
void LCD_PrintString(const uint8_t *var);
void LCD_PrintInteger(uint16_t var);
void LCD_PrintFloat(float var);    
    
#ifdef	__cplusplus
}
#endif

#endif	/* LCD_HD447780_H */

