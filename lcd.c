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
#include <string.h>

// Internal function prototypes
static void     LCD_SetDataBits(char input);
static void     LCD_Cmd(char cmd);
static void     LCD_Enable(void);
static _Bool    LCD_IsIdle(void);

/******************************************************************************
 * Function: LCD_SetDataBits(char input)
 *
 * Returns: Nothing
 * 
 * Description: Sets the data ports (4-bit) according to the provided hex value.
 * For 4-bit interface data, only four bus lines (DB4 to DB7) are used for the 
 * transfer. Bus lines DB0 to DB3 are disabled. The data transfer between the 
 * HD44780U and the MPU is completed after the 4-bit data has been transferred 
 * twice. As for the order of data transfer, the four high order bits 
 * (for 8-bit operation, DB4 to DB7) are transferred before the four low order 
 * bits (for 8-bit operation, DB0 to DB3)
 ******************************************************************************/
static void LCD_SetDataBits(char input) {
    
    if (input & 1)
        LCD_DB4 = 1;
    else
        LCD_DB4 = 0;
        
    if (input & 2)
        LCD_DB5 = 1;
    else
        LCD_DB5 = 0;

    if (input & 4)
        LCD_DB6 = 1;
    else
        LCD_DB6 = 0;

    if (input & 8)
        LCD_DB7 = 1;
    else
        LCD_DB7 = 0;
}


/******************************************************************************
 * Function: LCD_Cmd(char cmd)
 *
 * Returns: Nothing
 * 
 * Description: Sets the instruction to control the LCD display
 ******************************************************************************/
static void LCD_Cmd(char cmd) {

    LCD_RS = 0; // Select instruction register
    LCD_RW = 0; // Select write mode
    __delay_ms(2);
    LCD_SetDataBits(cmd); // Set hex value
    LCD_Enable();

}


/******************************************************************************
 * Function: LCD_Enable(void)
 *
 * Returns: Nothing
 * 
 * Description: Used after writing an instruction to the LCD. It sets the enable
 * read/write pin for a short period of time and then resets it.
 ******************************************************************************/
static void LCD_Enable(void) {
    
    LCD_EN = 1; // Starts reading/writing data
    __delay_us(500);
    LCD_EN = 0;
    
}


/******************************************************************************
 * Function: LCD_IsNotBusy(void)
 *
 * Returns: "true" if LCD is idle 
 * 
 * Description: This routine indicates whether the LCD is currently executing
 * an instruction. If so, the busy (DB7) flag is 1, and no other instruction 
 * will be accepted. 
 ******************************************************************************/
static _Bool LCD_IsIdle(void) {
    
    uint8_t rsState;
    
    // Save RS state before changing it 
    rsState = LCD_RS;
    
    LCD_RS = 0; // Select instruction register
    LCD_RW = 1; // Select read mode
    __delay_ms(2);
    while (!LCD_DB7); // wait while being busy (DB7 = 1)
    
    // Restore RS state 
    LCD_RS = rsState;
    
    return true;
}

/******************************************************************************
 * Function: LCD_Initialise(void)
 *
 * Returns: Nothing
 * 
 * Description: Initialises the LCD
 ******************************************************************************/
void LCD_Initialise(void) {
    
    LCD_SetDataBits(0x00); // Clear data bits
    __delay_ms(50);
    LCD_Cmd(0x03);
    __delay_ms(10);
    LCD_Cmd(0x03);
    __delay_ms(10);
    LCD_Cmd(0x03);

    LCD_Cmd(0x02);
    LCD_Cmd(0x02);
    LCD_Cmd(0x08);
    LCD_Cmd(0x00);
    LCD_Cmd(0x0C);
    LCD_Cmd(0x00);
    LCD_Cmd(0x06);
    
    /* At this point, after completing the initialisation, the busy bit DB7
    can be checked */
    
}


/******************************************************************************
 * Function: LCD_Clear(void)
 *
 * Returns: Nothing
 * 
 * Description: This routine clears the display and sets the cursor to the left
 * edge of the first line of the display.
 ******************************************************************************/
void LCD_Clear(void) {
    
    LCD_Cmd(0x00); // Turn the display on
    LCD_Cmd(0x01); // Clear the display
    
}


/******************************************************************************
 * Function: LCD_PrintCharacter(char input)
 *
 * Returns: Nothing
 * 
 * Description: This routine...
 ******************************************************************************/
void LCD_PrintCharacter(char input) {
    
    char lowerNibble, upperNibble;
    lowerNibble = input & 0x0F;
    upperNibble = input & 0xF0;
    
    LCD_RS = 1; // Select data register
    LCD_RW = 0; // Select write mode
   
    while(!LCD_IsIdle()); // Check if LCD is busy
    LCD_SetDataBits(upperNibble >> 4); // Transfer upper nibble first
    LCD_Enable(); // Start writing data
    while(!LCD_IsIdle()); // Wait while LCD is still busy
    LCD_SetDataBits(lowerNibble);
    LCD_Enable(); // Start writing data

}


/******************************************************************************
 * Function: LCD_PrintString(const char *data)
 *
 * Returns: Nothing
 * 
 * Description: This routine writes a string to the LCD
 ******************************************************************************/
void LCD_PrintString(const char *input) {
    
    size_t i;
    
    /* Note: Unlike the "sizeof" function, the "strlen" function does not 
     * count the null terminating character "\0" */
    for (i = 0; i < strlen(input); i++)
        LCD_PrintCharacter(input[i]);
    
}

/******************************************************************************
 * Function: 
 *
 * Returns: Nothing
 * 
 * Description: This routine...
 ******************************************************************************/

/******************************************************************************
 * Function: 
 *
 * Returns: Nothing
 * 
 * Description: This routine...
 ******************************************************************************/

/******************************************************************************
 * Function: 
 *
 * Returns: Nothing
 * 
 * Description: This routine...
 ******************************************************************************/

/******************************************************************************
 * Function: 
 *
 * Returns: Nothing
 * 
 * Description: This routine...
 ******************************************************************************/

/******************************************************************************
 * Function: 
 *
 * Returns: Nothing
 * 
 * Description: This routine...
 ******************************************************************************/
