/**
 * 
 * File:                lcd_hd44780.c
 * Author:              J. Striebel
 * Project:             Weather Station
 * Device:              PIC18F47Q10 @ 16 MHz internal OSC, 5V 
 * Platform:            Curiosity HPC board (DM164136)
 * Compiler:            XC8 (v2.41)
 * IDE:                 MPLAB X (v6.10), MCC (5.3.7)
 * Program version:     1.0
 
 * Program Description:
 * xxx
 * 
 * Hardware Description:
 * xxx
 * 
*/

#include "lcd_hd44780.h"

// Internal function prototypes
static void lcd_set_data_bits(uint8_t data);
static void lcd_cmd(uint8_t cmd);
static void lcd_enable(void);


/******************************************************************************
 * Function: lcd_scale_backlight_brightness(...)
 *
 * Returns: Duty cycle for PWM (0..100%)
 * 
 * Description: This function scales the LCD backlight brightness based on the 
 * inputted dimValue and returns the duty cycle that can be used by a PWM.  
 ******************************************************************************/
uint16_t lcd_scale_backlight_brightness(    uint16_t dimValue, 
                                            uint16_t dimResolution)
{
    // Check for zero division
    if (dimResolution != 0)
        /* Scale brightness using 32-bit integer to avoid overflow and then 
        cast result to 16-bit integer */
        return (uint16_t) dimValue * 100UL / dimResolution;
}
