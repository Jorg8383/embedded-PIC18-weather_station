/**
 * 
 * File Name:           main.c
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

/******************************************************************************
 * Includes and defines
 ******************************************************************************/
#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/adcc.h"
#include "lcd.h"
#include "lcd_app.h"
#include "bmp180.h"
#include "state.h"

void mainInit(void);
void sysTickRoutine(void);

/******************************************************************************
 * Main application
 ******************************************************************************/
void main(void)
{
    DeviceState currentState;
    DeviceContext deviceContext;
    BMP180_PARAM bmp180;
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

    // Select ADC channel, enable ADC and start conversion
    ADCC_StartConversion(RA0_POT);
    TMR2_Start();
    
    // Initialise the internal state machine
    initStateMachine(&currentState, &deviceContext);
    
    // Initialise the LCD display
    LCD_Init();
    __delay_ms(500);
    
    // Initialise the BMP180 barometric pressure sensor
    if (BMP180_Init(&bmp180) != 0) {
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