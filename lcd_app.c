/**
 * 
 * File:                lcd_app.c
 * Author:              J. Striebel
 * Project:             Weather Station
 * Device:              PIC18F47Q10 @ 16 MHz internal OSC, 5V
 * Platform:            Curiosity HPC board (DM164136)
 * Display:             LCD 16x2 - Hitachi HD44780
 * Compiler:            XC8 (v2.41)
 * IDE:                 MPLAB X (v6.10), MCC (5.3.7)
 * Program version:     1.0
 
 * Program Description:
 * 
*/

#include "lcd_app.h"

#define DISPLAY_TEMP_BUFFER_ELEMENTS    20

// Define text messages to be displayed on the LCD
const char *pText[LCD_NUM_MESSAGES] = {
    "Welcome to the weather station project!",
    "Temperature",
    "Pressure",
    "hPa",
    "Altitude",
    "metres",
    "COMM error",
};

// Function that returns a pointer to the text according to passed text index 
const char *getLcdText(LcdTextIndex txtIndex) {
    
    char *errorTxt = "Invalid text";
    
    if (txtIndex < LCD_NUM_MESSAGES) {
        return pText[txtIndex];
    } else {
        return errorTxt;
    }
}

// Function that displays the temperature as float
void displayTemperature(int16_t temperature) {
    
    char stringBuffer[DISPLAY_TEMP_BUFFER_ELEMENTS - 1] = "";
    uint8_t i = 0;
    uint8_t strStart = 0; 
    uint8_t strEnd;
    _Bool isNegative = false;
    _Bool addZeroDigit = false;
    
    // Handle negative numbers
    if (temperature < 0) {
        isNegative = true;
        temperature *= -1;
    }
    
    if (temperature >= -9 && temperature <= 9) {
        addZeroDigit = true;
    }
    
    /* Convert the number to a string in reverse order by creating a float 
     * presentation where for example a temperature value of 150 is displayed as
     * 15.0 degree Celsius */
    do {
        stringBuffer[i++] = temperature % 10 + '0';
        if (i == 1) {
            stringBuffer[i++] = '.';
            if (addZeroDigit) {
                stringBuffer[i++] = '0';
            }
        }
        temperature /= 10;
    } while (temperature > 0);
    
    // Add a '-' sign if the number was negative
    if (isNegative) {
        stringBuffer[i++] = '-';
    }
    
    // Null-terminate the string
    stringBuffer[i] = '\0';
    
    // Reverse the string to get the correct order
    strEnd = strlen(stringBuffer) - 1;
    while (strStart < strEnd) {
        char temp = stringBuffer[strStart];
        stringBuffer[strStart] = stringBuffer[strEnd];
        stringBuffer[strEnd] = temp;
        strStart++;
        strEnd--;
    }
    
    LCD_PrintString(stringBuffer);
    
}
