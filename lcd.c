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
 * xxx
 * 
 * Hardware Description:
 * xxx
 * 
*/

#include "lcd.h"

// Internal function prototypes
static void LCD_TransferNibbleBits(char input);
static void LCD_Write(char input, LCD_REG_TYPE regType);
static void LCD_ReverseString(char str[], uint16_t length);
static void LCD_ReadDataNibble(uint8_t *pReturnValue);
static uint8_t LCD_ReadDataByte(void);
static _Bool LCD_IsIdle(void);

static _Bool checkBusyBit;

/******************************************************************************
 * Function: LCD_SendNibbleBits(char input)
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
static void LCD_TransferNibbleBits(char input) {
    
    // Use the ternary operator to decode the nibble bits to be sent
    LCD_DB4 = (input & 0x01) ? 1 : 0;
    LCD_DB5 = (input & 0x02) ? 1 : 0;
    LCD_DB6 = (input & 0x04) ? 1 : 0;
    LCD_DB7 = (input & 0x08) ? 1 : 0;

    LCD_EN = 1; // Start writing data
    __delay_us(RW_TRIGGER_DELAY);
    LCD_EN = 0;
    __delay_us(RW_TRIGGER_DELAY);
    
}


/******************************************************************************
 * Function: LCD_Write(char input, LCD_REG_TYPE regType, _Bool checkBusyBit)
 *
 * Returns: Nothing
 * 
 * Description: This routine selects the provided register type (instruction OR 
 * data) and transmits the provided input instruction/data (8-bit) as two
 * consecutive nibble transfers.
 ******************************************************************************/
static void LCD_Write(char input, LCD_REG_TYPE regType) {
    
    // RS = 0 --> instruction register, RS = 1 --> data register
    LCD_RS = (regType == LCD_REG_CMD) ? 0 : 1; 
    __delay_us(100);
    
    while (!LCD_IsIdle()); // wait while LCD is still busy
    LCD_TransferNibbleBits((input & 0xF0) >> 4); // transmit upper nibble first
    LCD_TransferNibbleBits(input & 0x0F); // transmit lower nibble second
     
}

/******************************************************************************
 * Function: LCD_IsIdle(void)
 *
 * Returns: "true" if LCD is idle 
 * 
 * Description: This routine indicates whether the LCD is currently executing
 * an instruction. If so, the busy (DB7) flag is 1, and no other instruction 
 * will be accepted. 
 ******************************************************************************/
static _Bool LCD_IsIdle(void) {

    const uint16_t MAX_RETRY_COUNT = 500;
    uint16_t i;
    uint8_t receivedData;
    
    if (CHECK_BUSY_BIT) {
        /* To avoid a hangup in case of the LCD not responding, abort waiting
         * for the busy check bit after a certain number of retry cycles. 
         * In this case we'll still continue, just a bit slower than normally */
        for (i = MAX_RETRY_COUNT; i > 0; i--){
            receivedData = LCD_ReadDataByte();
            if ((receivedData & 0x80) == 0) // check busy bit (DB7)
                break;
        }
    } else {
        __delay_ms(1); // If busy bit isn't enabled then wait a little bit
    } 
    return true; 
}

/******************************************************************************
 * Function: LCD_Init(void)
 *
 * Returns: Nothing
 * 
 * Description: Initialises the LCD
 ******************************************************************************/
void LCD_Init(void) {

    LATD = LATD & 0x0F; // Reset data bit interface pins
    LCD_RS = 0; // Select instruction register
    LCD_RW = 0; // Select write mode
    __delay_ms(500);            
    LCD_TransferNibbleBits(0x03); // Function set (8-bit)
    __delay_ms(10);
    LCD_TransferNibbleBits(0x03); // Function set (8-bit)
    __delay_ms(3);
    LCD_TransferNibbleBits(0x03); // Function set (8-bit)
    __delay_ms(5);
    
    LCD_TransferNibbleBits(0x02); // Set interface to 4-bit mode
    /* Function set:
     * 4-bit interface, (N = 1) two lines, (F = 0) 5 x 8 dots font size */
    LCD_Write((0b00100000 | 1 << FUNC_SET_N_POS | 0 << FUNC_SET_F_POS),
            LCD_REG_CMD);
    /* Display control: 
     * (D = 1) display on, (C = 0) cursor off, (B = 0) blinking off */
    LCD_Write((0b00001000 | 1 << DISPLAY_CTRL_D_POS | 0 << DISPLAY_CTRL_C_POS |
            0 << DISPLAY_CTRL_B_POS), LCD_REG_CMD);
    LCD_Clear();
    /* Entry mode set:
     * (I/D = 1) Increment DDRAM address by 1; moves the cursor to the right,
     * (S = 0) no display shift */
    LCD_Write((0b00000100 | 1 << ENTRY_MODE_ID_POS | 0 << ENTRY_MODE_S_POS),
            LCD_REG_CMD);
    
    /* Now at this point, the busy bit DB7 can be checked */ 
    checkBusyBit = CHECK_BUSY_BIT;
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
    
    while (!LCD_IsIdle()); // wait while LCD is still busy
    LCD_Write(0x01, LCD_REG_CMD);
}


/******************************************************************************
 * Function: LCD_PrintCharacter(uint8_t input)
 *
 * Returns: Nothing
 * 
 * Description: This routine...
 ******************************************************************************/
void LCD_PrintCharacter(char input) {
  
    while (!LCD_IsIdle()); // wait while LCD is still busy
    LCD_Write(input, LCD_REG_DATA);
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
    
    /* Note: Unlike "sizeof()", "strlen()" does not include the null terminating 
     * character "\0" */
    for (i = 0; i < strlen(input); i++)
        LCD_PrintCharacter(*(input + i));
}


/******************************************************************************
 * Function: LCD_SetCursor(LCD_CURSOR_LINE line, uint8_t offset)
 *
 * Returns: Nothing
 * 
 * Description: This routine routine sets the cursor to a position in either the 
 * first or second display line, where the offset determines the exact position
 * in that line, starting from the position at the very left. 
 ******************************************************************************/
void LCD_SetCursor(LCD_CURSOR_LINE line, uint8_t offset) {
    
    uint8_t temp;
    
    switch (line) {
        case LCD_FIRST_LINE:
            /* 0x80 (MSB = 1) sets the DDRAM address, which positions the cursor
             * in the first line to the very left if offset = 0 */
            temp = 0x80 + offset - 1;
            while (!LCD_IsIdle()); // wait while LCD is still busy
            LCD_Write(temp, LCD_REG_CMD);
            break;
        case LCD_SECOND_LINE:
            /* Adding 0x40 to 0x80 positions the cursor in the second line */
            temp = 0x80 + 0x40 + offset - 1;
            while (!LCD_IsIdle()); // wait while LCD is still busy
            LCD_Write(temp, LCD_REG_CMD);            
            break;
        default:
            ; // line not defined
    }
}

/******************************************************************************
 * Function: LCD_ShiftDisplayRight(void)
 *
 * Returns: Nothing
 * 
 * Description: This routine shifts the display to the right without writing or
 * reading data. When the displayed data is shifted repeatedly each line moves
 * only horizontally. The second line display does not shift into the first line
 * position.
 ******************************************************************************/
void LCD_ShiftDisplayRight(void) {
    
    while (!LCD_IsIdle()); // wait while LCD is still busy
    LCD_Write(0x1C, LCD_REG_CMD);
}


/******************************************************************************
 * Function: LCD_ShiftDisplayLeft(void) 
 *
 * Returns: Nothing
 * 
 * Description: This routine shifts the display to the left without writing or
 * reading data. When the displayed data is shifted repeatedly each line moves
 * only horizontally. The second line display does not shift into the first line
 * position.
 ******************************************************************************/
void LCD_ShiftDisplayLeft(void) {
    
    while (!LCD_IsIdle()); // wait while LCD is still busy
    LCD_Write(0x18, LCD_REG_CMD);    
}


/******************************************************************************
 * Function: LCD_PrintInteger(int16_t number, uint8_t intBase)
 *
 * Returns: Nothing
 * 
 * Description: This function takes a integer number and its base (decimal,
 * hexadecimal, octal), converts it to a string and prints it on the
 * LCD display. The implementation of this function is based on the example
 * provided by: https://www.geeksforgeeks.org/implement-itoa/
 ******************************************************************************/
void LCD_PrintInteger(int16_t number, uint8_t intBase) {

    char str[10] = {0};
    uint8_t i = 0;
    uint8_t reminder;
    _Bool isNegative = false;
    
    // Handle zero as a number explicitly
    if (number == 0) {
        str[i++] = '0';
        str[i] = '\0'; // add null terminating character
        LCD_PrintString(str); // Print the string on the display
    }
    
    // Handle negative numbers with a base of 10
    if (number < 0 && intBase == INT_BASE_DECIMAL) {
        isNegative = true;
        number = number * -1;
    }
    
    // Extract digits individually
    while (number != 0) {
        // Perform Modulo to get the reminder
        reminder = number % intBase;
        /* Add the reminder with an offset either for hex or decimal/octal
         * to the string buffer */
        str[i++] = (reminder < 9) ? (reminder - 10) + 'a' : reminder + '0';
        number = number / intBase;
    }
    
    // Append sign if number is negative
    if (isNegative)
        str[i++] = '-';
    
    str[i] = '\0'; // Append a null terminating character
    LCD_ReverseString(str, i);
    LCD_PrintString(str); // Print the string on the display
    
}


/******************************************************************************
 * Function: reverseString(char str[], uint16_t length)
 *
 * Returns: Nothing
 * 
 * Description: This function reverses the provided string via pointer
 ******************************************************************************/
static void LCD_ReverseString(char str[], uint16_t length) {
    
    char temp;
    uint16_t startPos = 0;
    uint16_t endPos = length - 1; 
    // Keep swapping characters until half of the array length is reached 
    while (startPos < endPos) {
        temp = str[startPos];
        str[startPos] = str[endPos];
        str[endPos] = temp;
        endPos--;
        startPos++;
    }
}


/******************************************************************************
 * Function: LCD_ReadDataNibble(uint8_t *pReturnValue)
 *
 * Returns: Nothing
 * 
 * Description: This routine reads a data nibble from/to the bus-interface. The
 * read data value is returned via passing a function parameter by reference.
 ******************************************************************************/
static void LCD_ReadDataNibble(uint8_t *pReturnValue) {
    
    LCD_EN = 1; // Start reading/writing data
    __delay_us(RW_TRIGGER_DELAY);
    *pReturnValue = LCD_DATA_PORT & DATA_PORT_INPUT_PATTERN;
    LCD_EN = 0;
    __delay_us(RW_TRIGGER_DELAY);
}


/******************************************************************************
 * Function: LCD_ReadDataByte(void)
 *
 * Returns: Data byte as uint8_t
 * 
 * Description: This routine reads a data byte from/to the bus-interface.
 ******************************************************************************/
static uint8_t LCD_ReadDataByte(void) {
    
    _Bool rsStateHistory;
    uint8_t data, tempData;     

    rsStateHistory = LCD_RS; // Save RS state before changing it
    LCD_RS = 0; // Select instruction register
    LCD_RW = 1; // Select read mode
    LCD_DATA_TRIS |= ~DATA_TRIS_OUTPUT_PATTERN; // set data lines to input       
    __delay_us(10);

    /* Note that two cycles are needed for the busy flag check as well for
     * the data transfer */; 
    LCD_ReadDataNibble(&tempData); // Read higher nibble
    data = ((tempData << 4) & 0xF0);        
    LCD_ReadDataNibble(&tempData); // Read lower nibble
    data |= (tempData & 0x0F);

    LCD_DATA_TRIS &= DATA_TRIS_OUTPUT_PATTERN; // set data lines to output
    LCD_RW = 0; // Return to default mode of writing data to the LCD
    LCD_RS = rsStateHistory; // Restore RS state  

    return data;
}

