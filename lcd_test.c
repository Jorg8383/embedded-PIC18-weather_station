/**
 * 
 * File:                lcd.c
 * Author:              J. Striebel
 * Project:             Weather Station
 * Device:              PIC18F47Q10 @ 16 MHz internal OSC, 5V
 * Platform:            Curiosity HPC board (DM164136)
 * Display:             LCD 16x2 - Hitachi HD44780
 * Compiler:            XC8 (v2.41)
 * IDE:                 MPLAB X (v6.10), MCC (5.3.7)
 * Program version:     1.0
 
 * Program Description:
 * This module contains test cases for HD44780 API functions provided by lcd.c 
 * 
 * Hardware Description:
 * xxx
 * 
*/

#include "lcd_test.h"
#include "lcd.h"

/******************************************************************************
 * Function: 
 * 
 * Returns: Nothing
 * 
 * Description: 
 ******************************************************************************/
void LCD_TestRoutine(void) {
    
    uint8_t i;

    LCD_Clear();
    LCD_SetCursor(LCD_FIRST_LINE, 1);
    LCD_PrintString("Hello World");
    LCD_SetCursor(LCD_SECOND_LINE, 1);
    LCD_PrintString("PIC18F47Q10");

    __delay_ms(1000);

    // Shift display to the left
    for (i = 0; i < 15; i++) {
        __delay_ms(500);
        LCD_ShiftDisplayLeft();
    }

    // Shift display to the right
    for (i = 0; i < 15; i++) {
        __delay_ms(500);
        LCD_ShiftDisplayRight();
    }

    __delay_ms(1000);

    LCD_Clear();
    LCD_SetCursor(LCD_FIRST_LINE, 0);
    LCD_PrintString("End!");

    __delay_ms(1000);
        
}
        


