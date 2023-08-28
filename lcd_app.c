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

// Define text messages to be displayed on the LCD
const char *pText[LCD_NUM_MESSAGES] = {
    "Weather Station",
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

// Function that converts the temperature to a string with one decimal place
void convertTemperatureToString(int16_t temperature, char *buffer) {
        
    uint8_t i = 0;
    uint8_t strStart = 0; 
    uint8_t strEnd;
    _Bool isNegative = false;
    _Bool addZeroDigit = false;

    // Check for null pointer
    if (buffer == 0)
        return;
    
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
        buffer[i++] = temperature % 10 + '0';
        if (i == 1) {
            buffer[i++] = '.';
            if (addZeroDigit) {
                buffer[i++] = '0';
            }
        }
        temperature /= 10;
    } while (temperature > 0);
    
    // Add a '-' sign if the number was negative
    if (isNegative) {
        buffer[i++] = '-';
    }
    
    // Null-terminate the string
    buffer[i] = '\0';
    
    // Reverse the string to get the correct order
    strEnd = (uint8_t) strlen(buffer) - 1;
    while (strStart < strEnd) {
        char temp = buffer[strStart];
        buffer[strStart] = buffer[strEnd];
        buffer[strEnd] = temp;
        strStart++;
        strEnd--;
    }
    
    return;
    
}
