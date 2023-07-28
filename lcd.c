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
static void     LCD_SetDataBits(uint8_t data);
static void     LCD_Cmd(uint8_t cmd);
static void     LCD_Enable(void);
static _Bool    LCD_IsNotBusy(void);

/******************************************************************************
 * Function: LCD_SetDataBits(uint8_t data)
 *
 * Returns: Nothing
 * 
 * Description: Sets the data ports (4-bit) according to the provided hex value
 ******************************************************************************/
static void LCD_SetDataBits(uint8_t data) {
    
    if (data & 1)
        LCD_DB4 = 1;
    else
        LCD_DB4 = 0;
        
    if (data & 2)
        LCD_DB5 = 1;
    else
        LCD_DB5 = 0;

    if (data & 4)
        LCD_DB6 = 1;
    else
        LCD_DB6 = 0;

    if (data & 8)
        LCD_DB7 = 1;
    else
        LCD_DB7 = 0;
}


/******************************************************************************
 * Function: LCD_Cmd(uint8_t cmd)
 *
 * Returns: Nothing
 * 
 * Description: Sets the instruction to control the LCD display
 ******************************************************************************/
static void LCD_Cmd(uint8_t cmd) {

    LCD_RS = 0; // Select instruction register
    LCD_RW = 0; // Select write mode
    LCD_SetDataBits(cmd); // Set hex value
    LCD_Enable();

}


/******************************************************************************
 * Function: LCD_Enable(void)
 *
 * Returns: 
 * 
 * Description: 
 ******************************************************************************/
static void LCD_Enable(void) {
    
    LCD_RS = 0; // Select instruction register
    LCD_RW = 0; // Select write mode
    LCD_EN = 1; // Enable read/write
    __delay_ms(2);
    LCD_EN = 0; // Disable read/write
    
}


/******************************************************************************
 * Function: LCD_IsBusy(void)
 *
 * Returns: "true" if LCD is idle 
 * 
 * Description: When the busy flag is 1, the HD44780U is in the internal 
 * operation mode, and the next instruction will not be accepted. 
 ******************************************************************************/
static _Bool LCD_IsNotBusy(void) {
    
    LCD_RS = 0; // Select instruction register
    LCD_RW = 1; // Select read mode
    __delay_ms(2);
    while (!LCD_DB7) // wait while being busy (B7 = 1)
    return true;
}

/******************************************************************************
 * Function:
 *
 * Returns: 
 * 
 * Description: 
 ******************************************************************************/
