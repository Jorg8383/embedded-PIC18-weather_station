/* 
 * File:                lcd.h
 * Author:              J. Striebel
 * Project:             Weather Station
 * Device:              PIC18F47Q10 @ 16 MHz internal OSC, 5V
 * Platform:            Curiosity HPC board (DM164136)
 * Display:             LCD 16x2 - Hitachi HD44780
 * Compiler:            XC8 (v2.41)
 * IDE:                 MPLAB X (v6.10), MCC (5.3.7)
 * Program version:     1.0
 
 * Description:
 * ------------
 * This header file represents an Application Programming Interface (API) for
 * the Hitachi HD44780 LCD (16x2 lines), allowing it to interoperate with
 * a PIC18 microcontroller. The communication between the display and 
 * controller is realised via a 4-bit interface, thus transmitting one data byte
 * as two sequential 4-bit transfers. For this 4-bit interface, only four bus 
 * lines (DB4 to DB7) are used for transfer.
 * 
 * The Curiosity HPC with a PIC18F47Q10 MCU on board is wired to the HD44780 
 * display as follows:
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

#ifndef LCD_H
#define	LCD_H

#ifdef	__cplusplus
extern "C" {
#endif

// Include and defines
#include "mcc_generated_files/mcc.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#define LCD_DEBUG_COMPILE_TEST  0
    
// Project specific IO mapping
#define LCD_BL          LATDbits.LATD0  // LCD backlight LED
#define LCD_RS          LATDbits.LATD1  // Selects register: (0) = instruction, (1) = data
#define LCD_RW          LATDbits.LATD2  // Selects mode: (0) = write, (1) = read
#define LCD_EN          LATDbits.LATD3  // Starts data read/write
#define LCD_DB4         LATDbits.LATD4  
#define LCD_DB5         LATDbits.LATD5
#define LCD_DB6         LATDbits.LATD6
#define LCD_DB7         LATDbits.LATD7
#define LCD_DATA_TRIS   TRISD // TRIS register for data pins
#define LCD_DATA_PORT   PORTD // PORT register for data pins
    
// Definition of bit TRIS and PORT register pattern for data pins 
#define DATA_TRIS_OUTPUT_PATTERN 0x0F // TRIS data port pattern (0 = output)
#define DATA_PORT_INPUT_PATTERN 0xF0 // 1 = data pins used on data port    

/****************************************************************************** 
* LCD instruction register macros 
******************************************************************************/
#define ENTRY_MODE_ID_POS (1)
/* I/D: Increments (I/D = 1) or decrements (I/D = 0) the DDRAM address by 1 
 * when a character code is written into or read from DDRAM. The cursor or 
 * blinking moves to the right when incremented by 1 and to the left when 
 * decremented by 1. The same applies to writing and reading of CGRAM. */    
#define ENTRY_MODE_S_POS (0)
/* S: Shifts the entire display either to the right (I/D = 0) or to the left 
 * (I/D = 1) when S is 1. The display does not shift if S is 0 */
#define DISPLAY_CTRL_D_POS (2)
/* D: The display is on when D is 1 and off when D is 0 */
#define DISPLAY_CTRL_C_POS (1)
/* C: The cursor is displayed when C is 1 and not displayed when C is 0 */
#define DISPLAY_CTRL_B_POS (0)
/* B: The character indicated by the cursor blinks when B is 1 */
#define FUNC_SET_DL_POS (4)
/* DL: Sets the interface data length. Data is sent or received in 8-bit lengths 
 * (DB7 to DB0) when DL is 1, and in 4-bit lengths (DB7 to DB4) when DL is 0 */    
#define FUNC_SET_N_POS (3)
/* N: Sets the number of display lines. Two lines (N = 1), one line (N = 0) */    
#define FUNC_SET_F_POS (2)
/* F: Sets the character font. 5 x 10 dots (F = 1), 5 x 8 dots (F = 0)  */    

// Base definitions for integer to string conversion    
#define INT_BASE_DECIMAL 10 // Base 10 for decimal
#define INT_BASE_OCTAL 8 // Base 8 for octal
#define INT_BASE_HEX 16 // Base 16 for hexadecimal

// Default time delay (us) for triggering read/write instructions
#define RW_TRIGGER_DELAY 20  
// If CHECK_BUSY_BIT is set to 1, the busy bit DB7 will be checked
#define CHECK_BUSY_BIT 1
    
// Type definitions    
typedef enum {
    LCD_FIRST_LINE,
    LCD_SECOND_LINE
} LCD_CURSOR_LINE;

typedef enum {
    LCD_REG_CMD,
    LCD_REG_DATA       
} LCD_REG_TYPE;
    
/******************************************************************************* 
 * Function to initialise the LCD display
 ******************************************************************************/
/*
 * @brief This function needs to be invoked once to initialise the LCD. It
 * initialises the interface, the function set, the display control, and the
 * entry mode set.
 * 
 * @param None
 * 
 * @return void 
 * 
*/
extern void LCD_Init(void);


/******************************************************************************* 
 * Function to clear the LCD display
 ******************************************************************************/
/*
 * @brief This function clears the both lines of the LCD display.
 * 
 * @param None
 * 
 * @return void 
 * 
*/
extern void LCD_Clear(void);


/******************************************************************************* 
 * Function to set the cursor position on the LCD display
 ******************************************************************************/
/*
 * @brief This function sets the cursor at the line and position defined by the 
 * user. 
 * 
 * @param cursor line, position offset
 * 
 * @return void 
 * 
*/
extern void LCD_SetCursor(LCD_CURSOR_LINE line, uint8_t offset);


/******************************************************************************* 
 * Function to shift the display to the right
 ******************************************************************************/
/*
 * @brief By invoking this function once, both lines of the display are shifted 
 * by one position to the right. This routine shifts the display to the right 
 * without writing or reading data. When the displayed data is shifted repeatedly
 * each line moves only horizontally. The second line display does not shift
 * into the first line position.
 * 
 * @param None
 * 
 * @return void 
 * 
*/
extern void LCD_ShiftDisplayRight(void);


/******************************************************************************* 
 * Function to the shift the display to the left
 ******************************************************************************/
/*
 * @brief By invoking this function once, both lines of the display are shifted 
 * by one position to the left. This routine shifts the display to the left 
 * without writing or reading data. When the displayed data is shifted repeatedly
 * each line moves only horizontally. The second line display does not shift
 * into the first line position.
 * 
 * @param 
 * 
 * @return void 
 * 
*/
extern void LCD_ShiftDisplayLeft(void);


/******************************************************************************* 
 * Function to shift the cursor to the right
 ******************************************************************************/
/*
 * @brief This routine shifts the cursor by one position to the right 
 * without changing DDRAM contents
 * 
 * @param None
 * 
 * @return void 
 * 
*/
extern void LCD_ShiftCursorRight(void);


/******************************************************************************* 
 * Function to shift the cursor to the left 
 ******************************************************************************/
/*
 * @brief This routine shifts the cursor by one position to the left 
 * without changing DDRAM contents
 * 
 * @param None
 * 
 * @return void 
 * 
*/
extern void LCD_ShiftCursorLeft(void);


/******************************************************************************* 
 * Function to print a character on the LCD display
 ******************************************************************************/
/*
 * @brief This function prints a character on the LCD display
 * 
 * @param char
 * 
 * @return void 
 * 
*/
extern void LCD_PrintCharacter(char input);


/******************************************************************************* 
 * Function to print a string on the LCD display
 ******************************************************************************/
/*
 * @brief This function prints a string on the LCD display
 * 
 * @param pointer to char
 * 
 * @return void 
 * 
*/
extern void LCD_PrintString(const char *input);


/******************************************************************************* 
 * Function to print an integer value on the LCD display
 ******************************************************************************/
/*
 * @brief This function prints a integer value on the LCD display
 * 
 * @param number, base (decimal, hexadecimal, octal) 
 * 
 * @return void 
 * 
*/
extern void LCD_PrintInteger(int16_t number, uint8_t intBase);

#ifdef	__cplusplus
}
#endif

#endif	/* LCD_HD447780_H */

