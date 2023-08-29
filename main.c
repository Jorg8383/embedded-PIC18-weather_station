/**
 * 
 * File Name:           main.c
 * Author:              J. Striebel
 * Project:             Weather Station
 * Device:              PIC18F47Q10 @ 16 MHz internal OSC, 5V 
 * Platform:            Curiosity HPC board (DM164136)
 * Compiler:            XC8 (v2.41)
 * IDE:                 MPLAB X (v6.10), MCC (5.3.7)
 
 * Description:
 * ------------
 * This weather station project demonstrates the implementation of two APIs to
 * communicate with the Hitachi HD44780 LCD via a 4-bit interface and the Bosch
 * BMP180 barometric pressure sensor via I2C. The raw temperature and pressure
 * values are read from the sensor, and based on this raw data, true temperature
 * and pressure values are calculated and then displayed on the LCD.
 * In addition, the altitude is calculated based on the pressure value. These 
 * three values are updated and displayed periodically using a finite state 
 * machine based on function pointers, representing the internal device state.
 * 
 * External peripherals:
 * ---------------------
 * - Hitachi HD44780 LCD (16 x 2 lines) via 4-bit interface
 * - Bosch BMP180 barometric pressure sensor via I2C interface
 * 
*/

/******************************************************************************
 * Includes and defines
 ******************************************************************************/
#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/adcc.h"
#include "lcd.h"
#include "lcd_app.h"
#include "bmp180.h"
#include "state.h"

// Global variables
BMP180_PARAM bmp180param;

/******************************************************************************
 * Main application
 ******************************************************************************/
void main(void)
{
    DeviceState currentState;
    DeviceContext deviceContext;
    _Bool sensorInitFailed = false;

    /***************************************************************************
     * Main initialisation routine 
     **************************************************************************/       
    SYSTEM_Initialize();

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    // Initialise the ADC conversion for the LCD backlight dimmer potentiometer
    ADCC_StartConversion(RA0_POT);
    TMR2_Start();
    
    // Initialise the internal state machine
    initStateMachine(&currentState, &deviceContext);
    
    // Initialise the LCD display
    LCD_Init();
    __delay_ms(500);
    
    // Initialise the BMP180 barometric pressure sensor
    if (BMP180_Init(&bmp180param) != 0) {
        sensorInitFailed = true;
        LCD_Clear();
        LCD_PrintString(getLcdText(LCD_TXT_ERROR_SENSOR));
    }
    while (sensorInitFailed); // Remain here if sensor communication has failed 
    
    /***************************************************************************
     * Main infinite loop 
     **************************************************************************/
    while (1)
    {
        runStateMachine(&currentState, &deviceContext);
    }
}
/**
 End of File
*/