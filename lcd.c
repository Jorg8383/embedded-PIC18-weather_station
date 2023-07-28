/**
 * 
 * File:                lcd.c
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

#include "lcd.h"

// Internal function prototypes
static void LCD_SetDataBits(uint8_t data);
static void LCD_Cmd(uint8_t cmd);
static void LCD_Enable(void);


/******************************************************************************
 * Function: 
 *
 * Returns: 
 * 
 * Description: 
 ******************************************************************************/
