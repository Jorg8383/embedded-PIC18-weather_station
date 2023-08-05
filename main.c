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
#include "lcd_test.h"

/******************************************************************************
 * Main application
 ******************************************************************************/
void main(void)
{
    mainInit();
    
    while (1)
    {
        LCD_TestRoutine();
        
    }
}

/******************************************************************************
 * Main init application
 ******************************************************************************/
void mainInit(void){

    // Initialize the device
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

    LCD_Init();
    __delay_ms(500);
       
}

/**
 End of File
*/