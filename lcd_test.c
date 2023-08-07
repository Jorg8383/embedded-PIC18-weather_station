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

#include <stdio.h>
#include <string.h>
#include "lcd_test.h"
#include "lcd.h"

char * pDisplayText[] = {
    "Weather Station",
    "PIC18F47Q10",
    "Centered",
    "Pressure:"
};

// Function prototypes
static void testPrintStringInFirstLine(void);
static void testPrintStringInSecondLine(void);
static void testPrintStringCentered(void);
static void testShiftDisplayLeft(void);
static void testShiftDisplayRight(void);


/******************************************************************************
 * Function: 
 * 
 * Returns: Nothing
 * 
 * Description: 
 ******************************************************************************/
void LCD_TestRoutine(void) {
    
    LCD_Clear();
    testPrintStringInFirstLine();
    testPrintStringInSecondLine();
    __delay_ms(1000);
    testShiftDisplayLeft();
    testShiftDisplayRight();
    __delay_ms(1000);
    LCD_Clear();
    testPrintStringCentered();
    __delay_ms(1000);
        
}

static void testPrintStringInFirstLine(void) {
    
    LCD_SetCursor(LCD_FIRST_LINE, 0);
    LCD_PrintString(pDisplayText[0]);    
}


static void testPrintStringInSecondLine(void) {

    LCD_SetCursor(LCD_SECOND_LINE, 0);
    LCD_PrintString(pDisplayText[1]);    
}


static void testPrintStringCentered(void) {
    
    LCD_Clear();
    LCD_SetCursor(LCD_FIRST_LINE, (uint8_t) (16 - strlen(pDisplayText[2])) / 2);
    LCD_PrintString(pDisplayText[2]);
}

static void testShiftDisplayLeft(void) {
    uint8_t i;
    
    // Test shifting display to the left
    for (i = 0; i < 15; i++) {
        __delay_ms(300);
        LCD_ShiftDisplayLeft();
    }
}
static void testShiftDisplayRight(void) {
    uint8_t i;
    
    // Test shifting display to the right
    for (i = 0; i < 15; i++) {
        __delay_ms(300);
        LCD_ShiftDisplayRight();
    }
}



