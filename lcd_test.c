/**
 * 
 * File:                lcd_test.c
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
*/

#include <stdio.h>
#include <string.h>
#include "lcd.h"

#if LCD_DEBUG_COMPILE_TEST

char *pDisplayText[] = {
    "Weather Station", // Index 0
    "PIC18F47Q10",
    "Centered",
    "Pressure:",
    "HEX:", // Index 4
    "UINT16:", 
    "UINT8:",
    "INT16:",
    "INT8:"
};

// Function prototypes
static void testPrintStringInFirstLine(uint8_t textIndex);
static void testPrintStringInSecondLine(uint8_t textIndex);
static void testPrintStringCentered(uint8_t textIndex);
static void testPrintCharInFirstLine(void);
static void testPrintInteger(void);
static void testShiftDisplayLeft(void);
static void testShiftDisplayRight(void);
static void testShiftCursor(void);


void LCD_TestRoutine(void) {

    LCD_Clear();
    testPrintStringInFirstLine();
    testPrintStringInSecondLine();
    __delay_ms(1000);
    testShiftDisplayLeft();
    testShiftDisplayRight();
    __delay_ms(1000);
    testPrintStringCentered();
    testPrintCharInFirstLine();

    testPrintInteger();
}

static void testPrintStringInFirstLine(uint8_t textIndex) {
    
    LCD_SetCursor(LCD_FIRST_LINE, 0);
    LCD_PrintString(pDisplayText[textIndex]);    
}


static void testPrintStringInSecondLine(uint8_t textIndex) {

    LCD_SetCursor(LCD_SECOND_LINE, 0);
    LCD_PrintString(pDisplayText[textIndex]);    
}


static void testPrintStringCentered(uint8_t textIndex) {
    
    LCD_Clear();
    LCD_SetCursor(LCD_FIRST_LINE, (uint8_t) (16 - strlen(pDisplayText[2])) / 2);
    LCD_PrintString(pDisplayText[textIndex]);
    __delay_ms(2000);
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


static void testPrintCharInFirstLine(void) {

    LCD_Clear();
    LCD_PrintCharacter(**pDisplayText);
    __delay_ms(2000);
}


static void testPrintInteger(void) {
    
    const uint16_t hexValue = 0x78FC;
    const uint16_t posUint16 = 4939;
    const uint8_t posUint8 = 200;
    const int16_t negUint16 = -27800;
    const int8_t negUint8 = -8;
    
    LCD_Clear();
    LCD_SetCursor(LCD_FIRST_LINE, 0);
    LCD_PrintString(pDisplayText[4]);    
    LCD_ShiftCursorRight();
    LCD_PrintInteger(hexValue, INT_BASE_HEX);
    __delay_ms(4000);
        
    LCD_Clear();
    LCD_SetCursor(LCD_FIRST_LINE, 0);
    LCD_PrintString(pDisplayText[5]);
    LCD_ShiftCursorRight();
    LCD_PrintInteger(posUint16, INT_BASE_DECIMAL);
    LCD_SetCursor(LCD_SECOND_LINE, 0);
    LCD_PrintString(pDisplayText[6]);    
    LCD_ShiftCursorRight();
    LCD_PrintInteger(posUint8, INT_BASE_DECIMAL);
    __delay_ms(4000);
    
    LCD_Clear();
    LCD_SetCursor(LCD_FIRST_LINE, 0);
    LCD_PrintString(pDisplayText[7]);    
    LCD_ShiftCursorRight();
    LCD_PrintInteger(negUint16, INT_BASE_DECIMAL);
    LCD_SetCursor(LCD_SECOND_LINE, 0);
    LCD_PrintString(pDisplayText[8]);    
    LCD_ShiftCursorRight();
    LCD_PrintInteger(negUint8, INT_BASE_DECIMAL);
    __delay_ms(4000);    
}


static void testShiftCursor(void) {

    // Test shift cursor right
    LCD_Clear();
    testPrintStringInFirstLine(3);
    LCD_ShiftCursorRight();
    LCD_PrintCharacter('x');
    __delay_ms(3000);

    // Test shift cursor left
    LCD_Clear();
    LCD_SetCursor(LCD_SECOND_LINE, 8);
    LCD_PrintCharacter('y');
    LCD_ShiftCursorLeft();
    LCD_ShiftCursorLeft();
    LCD_PrintCharacter('x');    
    __delay_ms(3000);  
}

#endif