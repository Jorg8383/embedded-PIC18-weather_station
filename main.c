/**
  
  Author:
    Jorg Striebel

  File Name:
    main.c

  Summary:
    Main file of weather station project

  Description:
    Device:     PIC18F47Q10
*/

#include "mcc_generated_files/mcc.h"

/*
                         Main application
 */
void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
//    INTERRUPT_GlobalInterruptDisable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    // Select ADC channel, enable ADC and start conversion
    ADCC_StartConversion(RA0_POT);
    
    while (1)
    {
        // Add your application code
    }
}
/**
 End of File
*/