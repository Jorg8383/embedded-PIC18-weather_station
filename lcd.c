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
 
 * Description:
 * ------------
 * This module represents an Application Programming Interface (API) for the LCD
 * of the Hitachi HD44780 (16x2 lines), allowing it to interoperate with a 
 * PIC18F47Q10 microcontroller. The communication between the display and 
 * controller is realised via a 4-bit interface, thus transmitting one data byte
 * as two sequential 4-bit transfers. For the 4-bit interface, only four bus 
 * lines (DB4 to DB7) are used for transfer.
*/

#include "mcc_generated_files/mcc.h"
#include "lcd.h"

// Internal function prototypes
static void LCD_TransferNibbleBits(char input);
static void LCD_Write(char input, LCD_REG_TYPE regType);
static void LCD_ReverseString(char str[], uint16_t length);
static void LCD_ReadDataNibble(uint8_t *pReturnValue);
static uint8_t LCD_ReadDataByte(void);
static _Bool LCD_IsIdle(void);

static _Bool checkBusyBit;


/******************************************************************************* 
 * Function to transfer one nibble to the LCD
 ******************************************************************************/
/*
 * @brief Sets the data ports (4-bit) according to the provided hex value.
 * For 4-bit interface data, only four bus lines (DB4 to DB7) are used for the 
 * transfer. Bus lines DB0 to DB3 are disabled. The data transfer between the 
 * HD44780U and the MPU is completed after the 4-bit data has been transferred 
 * twice. As for the order of data transfer, the four high order bits 
 * (for 8-bit operation, DB4 to DB7) are transferred before the four low order 
 * bits (for 8-bit operation, DB0 to DB3)
 * 
 * @param input (char)
 * 
 * @return void 
 * 
*/
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


/******************************************************************************* 
 * Function to write one byte to the LCD data/instruction register 
 ******************************************************************************/
/*
 * @brief This routine selects the provided register type (instruction OR 
 * data) and transmits the provided input instruction/data (8-bit) as two
 * consecutive nibble transfers..
 * 
 * @param None
 * 
 * @return void 
 * 
*/
static void LCD_Write(char input, LCD_REG_TYPE regType) {
    
    // RS = 0 --> instruction register, RS = 1 --> data register
    LCD_RS = (regType == LCD_REG_CMD) ? 0 : 1; 
    __delay_us(100);
    
    while (!LCD_IsIdle()); // wait while LCD is still busy
    LCD_TransferNibbleBits((input & 0xF0) >> 4); // transmit upper nibble first
    LCD_TransferNibbleBits(input & 0x0F); // transmit lower nibble second
     
}


/******************************************************************************* 
 * Function to check whether LCD is busy
 ******************************************************************************/
/*
 * @brief This routine indicates whether the LCD is currently executing
 * an instruction. If so, the busy (DB7) flag is 1, and no other instruction 
 * will be accepted.
 * 
 * @param None
 * 
 * @return True if LCD is idle 
 * 
*/
static _Bool LCD_IsIdle(void) {

    const uint16_t MAX_RETRY_COUNT = 500;
    uint16_t i;
    uint8_t receivedData;
    
    if (CHECK_BUSY_BIT) {
        /* To avoid a hangup in case of the LCD not responding, abort waiting
         * for the busy check bit after a certain number of retry cycles. 
         * In this case we'll still continue, just a bit slower than normally */
        for (i = 0; i < MAX_RETRY_COUNT; i++){
            receivedData = LCD_ReadDataByte();
            if ((receivedData & 0x80) == 0) // check busy bit (DB7)
                break;
        }
    } else {
        __delay_ms(1); // If busy bit isn't enabled then wait a little bit
    } 
    return true; 
}

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


/******************************************************************************* 
 * Function to clear the LCD display
 ******************************************************************************/
/*
 * @brief This routine clears the display and sets the cursor to the left
 * edge of the first line of the display.
 * 
 * @param None
 * 
 * @return void 
 * 
*/
void LCD_Clear(void) {
    
    while (!LCD_IsIdle()); // wait while LCD is still busy
    LCD_Write(0x01, LCD_REG_CMD);
    __delay_ms(5);
}


/******************************************************************************* 
 * Function to print a character on the LCD display
 ******************************************************************************/
/*
 * @brief This routine prints a character on the LCD display
 * 
 * @param input (char)
 * 
 * @return void 
 * 
*/
void LCD_PrintCharacter(char input) {
  
    while (!LCD_IsIdle()); // wait while LCD is still busy
    LCD_Write(input, LCD_REG_DATA);
}


/******************************************************************************* 
 * Function to print a string on the LCD display
 ******************************************************************************/
/*
 * @brief This routine writes a string to the LCD
 * 
 * @param input (char)
 * 
 * @return void 
 * 
*/
void LCD_PrintString(const char *input) {
    
    size_t i;
    
    /* Note: Unlike "sizeof()", "strlen()" does not include the null terminating 
     * character "\0" */
    for (i = 0; i < strlen(input); i++)
        LCD_PrintCharacter(*(input + i));
}



/******************************************************************************* 
 * Function to set the cursor position
 ******************************************************************************/
/*
 * @brief This routine routine sets the cursor to a position in either the 
 * first or second display line, where the offset determines the exact position
 * in that line, starting from the position at the very left. 
 * 
 * @param cursor line, cursor position offset
 * 
 * @return void 
 * 
*/
void LCD_SetCursor(LCD_CURSOR_LINE line, uint8_t offset) {
    
    uint8_t value;
    
    switch (line) {
        case LCD_FIRST_LINE:
            /* 0x80 (MSB = 1) sets the DDRAM address, which positions the cursor
             * in the first line to the very left if offset = 0 */
            value = (offset & 0x7F) | 0x80;
            break;
        case LCD_SECOND_LINE:
            /* Adding 0x40 to 0x80 positions the cursor in the second line */
            value = (offset & 0x7F) | (0x80 + 0x40);
            break;
        default:
            ; // line not defined
        }
    while (!LCD_IsIdle()); // wait while LCD is still busy
    LCD_Write(value, LCD_REG_CMD);
    __delay_ms(5);
}

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
void LCD_ShiftDisplayRight(void) {
    
    while (!LCD_IsIdle()); // wait while LCD is still busy
    LCD_Write(0x1C, LCD_REG_CMD);
}


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
void LCD_ShiftDisplayLeft(void) {
    
    while (!LCD_IsIdle()); // wait while LCD is still busy
    LCD_Write(0x18, LCD_REG_CMD);    
}


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
void LCD_ShiftCursorRight(void) {
    
    while (!LCD_IsIdle()); // wait while LCD is still busy
    LCD_Write(0x14, LCD_REG_CMD);
}


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
void LCD_ShiftCursorLeft(void) {
    
    while (!LCD_IsIdle()); // wait while LCD is still busy
    LCD_Write(0x10, LCD_REG_CMD);
    return;
}


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
void LCD_PrintInteger(int16_t number, uint8_t intBase) {

    char buffer[20] = "";
    uint8_t i = 0;
    uint8_t reminder;
    int16_t num = number;
    _Bool isNegative = false;
    
    // Handle zero as a number explicitly
    if (number == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0'; // add null terminating character
        LCD_PrintString(buffer); // Print the string on the display
        return;
    }
    
    // Handle negative numbers with a base of 10
    if (num < 0 && intBase == INT_BASE_DECIMAL) {
        isNegative = true;
        num = num * -1;
    }
    
    // Extract digits individually
    while (num != 0) {
        // Perform Modulo to get the reminder
        reminder = num % intBase;
        /* Add the reminder with an offset either for hex or decimal/octal
         * to the string buffer */
        buffer[i++] = (reminder > 9) ? (reminder - 10) + 'a' : reminder + '0';
        num = num / intBase;
    }
    
    // Append sign if number is negative
    if (isNegative)
        buffer[i++] = '-';
    
    buffer[i] = '\0'; // Append a null terminating character
    LCD_ReverseString(buffer, i);
    LCD_PrintString(buffer); // Print the string on the display   
    return;
}


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
    return;
}


/******************************************************************************* 
 * Function to print an integer value on the LCD display
 ******************************************************************************/
/*
 * @brief This routine reads a data nibble from/to the bus-interface. The
 * read data value is returned via passing a function parameter by reference.
 * 
 * @param pointer to return value (uint8_t) 
 * 
 * @return void 
 * 
*/
static void LCD_ReadDataNibble(uint8_t *pReturnValue) {
    
    LCD_EN = 1; // Start reading/writing data
    __delay_us(RW_TRIGGER_DELAY);
    *pReturnValue = LCD_DATA_PORT & DATA_PORT_INPUT_PATTERN;
    LCD_EN = 0;
    __delay_us(RW_TRIGGER_DELAY);
    return;
}


/******************************************************************************* 
 * Function to print an integer value on the LCD display
 ******************************************************************************/
/*
 * @brief This routine reads a data byte from/to the bus-interface.
 * 
 * @param None
 * 
 * @return Data byte (uint8_t) 
 * 
*/
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

