/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC18F47Q10
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.36 and above
        MPLAB 	          :  MPLAB X 6.00	
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set RA0_POT aliases
#define RA0_POT_TRIS                 TRISAbits.TRISA0
#define RA0_POT_LAT                  LATAbits.LATA0
#define RA0_POT_PORT                 PORTAbits.RA0
#define RA0_POT_WPU                  WPUAbits.WPUA0
#define RA0_POT_OD                   ODCONAbits.ODCA0
#define RA0_POT_ANS                  ANSELAbits.ANSELA0
#define RA0_POT_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define RA0_POT_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define RA0_POT_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define RA0_POT_GetValue()           PORTAbits.RA0
#define RA0_POT_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define RA0_POT_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define RA0_POT_SetPullup()          do { WPUAbits.WPUA0 = 1; } while(0)
#define RA0_POT_ResetPullup()        do { WPUAbits.WPUA0 = 0; } while(0)
#define RA0_POT_SetPushPull()        do { ODCONAbits.ODCA0 = 0; } while(0)
#define RA0_POT_SetOpenDrain()       do { ODCONAbits.ODCA0 = 1; } while(0)
#define RA0_POT_SetAnalogMode()      do { ANSELAbits.ANSELA0 = 1; } while(0)
#define RA0_POT_SetDigitalMode()     do { ANSELAbits.ANSELA0 = 0; } while(0)

// get/set RC6 procedures
#define RC6_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define RC6_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define RC6_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define RC6_GetValue()              PORTCbits.RC6
#define RC6_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define RC6_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define RC6_SetPullup()             do { WPUCbits.WPUC6 = 1; } while(0)
#define RC6_ResetPullup()           do { WPUCbits.WPUC6 = 0; } while(0)
#define RC6_SetAnalogMode()         do { ANSELCbits.ANSELC6 = 1; } while(0)
#define RC6_SetDigitalMode()        do { ANSELCbits.ANSELC6 = 0; } while(0)

// get/set RC7 procedures
#define RC7_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define RC7_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define RC7_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define RC7_GetValue()              PORTCbits.RC7
#define RC7_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define RC7_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define RC7_SetPullup()             do { WPUCbits.WPUC7 = 1; } while(0)
#define RC7_ResetPullup()           do { WPUCbits.WPUC7 = 0; } while(0)
#define RC7_SetAnalogMode()         do { ANSELCbits.ANSELC7 = 1; } while(0)
#define RC7_SetDigitalMode()        do { ANSELCbits.ANSELC7 = 0; } while(0)

// get/set RD0 procedures
#define RD0_SetHigh()            do { LATDbits.LATD0 = 1; } while(0)
#define RD0_SetLow()             do { LATDbits.LATD0 = 0; } while(0)
#define RD0_Toggle()             do { LATDbits.LATD0 = ~LATDbits.LATD0; } while(0)
#define RD0_GetValue()              PORTDbits.RD0
#define RD0_SetDigitalInput()    do { TRISDbits.TRISD0 = 1; } while(0)
#define RD0_SetDigitalOutput()   do { TRISDbits.TRISD0 = 0; } while(0)
#define RD0_SetPullup()             do { WPUDbits.WPUD0 = 1; } while(0)
#define RD0_ResetPullup()           do { WPUDbits.WPUD0 = 0; } while(0)
#define RD0_SetAnalogMode()         do { ANSELDbits.ANSELD0 = 1; } while(0)
#define RD0_SetDigitalMode()        do { ANSELDbits.ANSELD0 = 0; } while(0)

// get/set RD1_LCD_RS aliases
#define RD1_LCD_RS_TRIS                 TRISDbits.TRISD1
#define RD1_LCD_RS_LAT                  LATDbits.LATD1
#define RD1_LCD_RS_PORT                 PORTDbits.RD1
#define RD1_LCD_RS_WPU                  WPUDbits.WPUD1
#define RD1_LCD_RS_OD                   ODCONDbits.ODCD1
#define RD1_LCD_RS_ANS                  ANSELDbits.ANSELD1
#define RD1_LCD_RS_SetHigh()            do { LATDbits.LATD1 = 1; } while(0)
#define RD1_LCD_RS_SetLow()             do { LATDbits.LATD1 = 0; } while(0)
#define RD1_LCD_RS_Toggle()             do { LATDbits.LATD1 = ~LATDbits.LATD1; } while(0)
#define RD1_LCD_RS_GetValue()           PORTDbits.RD1
#define RD1_LCD_RS_SetDigitalInput()    do { TRISDbits.TRISD1 = 1; } while(0)
#define RD1_LCD_RS_SetDigitalOutput()   do { TRISDbits.TRISD1 = 0; } while(0)
#define RD1_LCD_RS_SetPullup()          do { WPUDbits.WPUD1 = 1; } while(0)
#define RD1_LCD_RS_ResetPullup()        do { WPUDbits.WPUD1 = 0; } while(0)
#define RD1_LCD_RS_SetPushPull()        do { ODCONDbits.ODCD1 = 0; } while(0)
#define RD1_LCD_RS_SetOpenDrain()       do { ODCONDbits.ODCD1 = 1; } while(0)
#define RD1_LCD_RS_SetAnalogMode()      do { ANSELDbits.ANSELD1 = 1; } while(0)
#define RD1_LCD_RS_SetDigitalMode()     do { ANSELDbits.ANSELD1 = 0; } while(0)

// get/set RD2_LCD_RW aliases
#define RD2_LCD_RW_TRIS                 TRISDbits.TRISD2
#define RD2_LCD_RW_LAT                  LATDbits.LATD2
#define RD2_LCD_RW_PORT                 PORTDbits.RD2
#define RD2_LCD_RW_WPU                  WPUDbits.WPUD2
#define RD2_LCD_RW_OD                   ODCONDbits.ODCD2
#define RD2_LCD_RW_ANS                  ANSELDbits.ANSELD2
#define RD2_LCD_RW_SetHigh()            do { LATDbits.LATD2 = 1; } while(0)
#define RD2_LCD_RW_SetLow()             do { LATDbits.LATD2 = 0; } while(0)
#define RD2_LCD_RW_Toggle()             do { LATDbits.LATD2 = ~LATDbits.LATD2; } while(0)
#define RD2_LCD_RW_GetValue()           PORTDbits.RD2
#define RD2_LCD_RW_SetDigitalInput()    do { TRISDbits.TRISD2 = 1; } while(0)
#define RD2_LCD_RW_SetDigitalOutput()   do { TRISDbits.TRISD2 = 0; } while(0)
#define RD2_LCD_RW_SetPullup()          do { WPUDbits.WPUD2 = 1; } while(0)
#define RD2_LCD_RW_ResetPullup()        do { WPUDbits.WPUD2 = 0; } while(0)
#define RD2_LCD_RW_SetPushPull()        do { ODCONDbits.ODCD2 = 0; } while(0)
#define RD2_LCD_RW_SetOpenDrain()       do { ODCONDbits.ODCD2 = 1; } while(0)
#define RD2_LCD_RW_SetAnalogMode()      do { ANSELDbits.ANSELD2 = 1; } while(0)
#define RD2_LCD_RW_SetDigitalMode()     do { ANSELDbits.ANSELD2 = 0; } while(0)

// get/set RD3_LCD_E aliases
#define RD3_LCD_E_TRIS                 TRISDbits.TRISD3
#define RD3_LCD_E_LAT                  LATDbits.LATD3
#define RD3_LCD_E_PORT                 PORTDbits.RD3
#define RD3_LCD_E_WPU                  WPUDbits.WPUD3
#define RD3_LCD_E_OD                   ODCONDbits.ODCD3
#define RD3_LCD_E_ANS                  ANSELDbits.ANSELD3
#define RD3_LCD_E_SetHigh()            do { LATDbits.LATD3 = 1; } while(0)
#define RD3_LCD_E_SetLow()             do { LATDbits.LATD3 = 0; } while(0)
#define RD3_LCD_E_Toggle()             do { LATDbits.LATD3 = ~LATDbits.LATD3; } while(0)
#define RD3_LCD_E_GetValue()           PORTDbits.RD3
#define RD3_LCD_E_SetDigitalInput()    do { TRISDbits.TRISD3 = 1; } while(0)
#define RD3_LCD_E_SetDigitalOutput()   do { TRISDbits.TRISD3 = 0; } while(0)
#define RD3_LCD_E_SetPullup()          do { WPUDbits.WPUD3 = 1; } while(0)
#define RD3_LCD_E_ResetPullup()        do { WPUDbits.WPUD3 = 0; } while(0)
#define RD3_LCD_E_SetPushPull()        do { ODCONDbits.ODCD3 = 0; } while(0)
#define RD3_LCD_E_SetOpenDrain()       do { ODCONDbits.ODCD3 = 1; } while(0)
#define RD3_LCD_E_SetAnalogMode()      do { ANSELDbits.ANSELD3 = 1; } while(0)
#define RD3_LCD_E_SetDigitalMode()     do { ANSELDbits.ANSELD3 = 0; } while(0)

// get/set RD4_LCD_DB4 aliases
#define RD4_LCD_DB4_TRIS                 TRISDbits.TRISD4
#define RD4_LCD_DB4_LAT                  LATDbits.LATD4
#define RD4_LCD_DB4_PORT                 PORTDbits.RD4
#define RD4_LCD_DB4_WPU                  WPUDbits.WPUD4
#define RD4_LCD_DB4_OD                   ODCONDbits.ODCD4
#define RD4_LCD_DB4_ANS                  ANSELDbits.ANSELD4
#define RD4_LCD_DB4_SetHigh()            do { LATDbits.LATD4 = 1; } while(0)
#define RD4_LCD_DB4_SetLow()             do { LATDbits.LATD4 = 0; } while(0)
#define RD4_LCD_DB4_Toggle()             do { LATDbits.LATD4 = ~LATDbits.LATD4; } while(0)
#define RD4_LCD_DB4_GetValue()           PORTDbits.RD4
#define RD4_LCD_DB4_SetDigitalInput()    do { TRISDbits.TRISD4 = 1; } while(0)
#define RD4_LCD_DB4_SetDigitalOutput()   do { TRISDbits.TRISD4 = 0; } while(0)
#define RD4_LCD_DB4_SetPullup()          do { WPUDbits.WPUD4 = 1; } while(0)
#define RD4_LCD_DB4_ResetPullup()        do { WPUDbits.WPUD4 = 0; } while(0)
#define RD4_LCD_DB4_SetPushPull()        do { ODCONDbits.ODCD4 = 0; } while(0)
#define RD4_LCD_DB4_SetOpenDrain()       do { ODCONDbits.ODCD4 = 1; } while(0)
#define RD4_LCD_DB4_SetAnalogMode()      do { ANSELDbits.ANSELD4 = 1; } while(0)
#define RD4_LCD_DB4_SetDigitalMode()     do { ANSELDbits.ANSELD4 = 0; } while(0)

// get/set RD5_LCD_DB5 aliases
#define RD5_LCD_DB5_TRIS                 TRISDbits.TRISD5
#define RD5_LCD_DB5_LAT                  LATDbits.LATD5
#define RD5_LCD_DB5_PORT                 PORTDbits.RD5
#define RD5_LCD_DB5_WPU                  WPUDbits.WPUD5
#define RD5_LCD_DB5_OD                   ODCONDbits.ODCD5
#define RD5_LCD_DB5_ANS                  ANSELDbits.ANSELD5
#define RD5_LCD_DB5_SetHigh()            do { LATDbits.LATD5 = 1; } while(0)
#define RD5_LCD_DB5_SetLow()             do { LATDbits.LATD5 = 0; } while(0)
#define RD5_LCD_DB5_Toggle()             do { LATDbits.LATD5 = ~LATDbits.LATD5; } while(0)
#define RD5_LCD_DB5_GetValue()           PORTDbits.RD5
#define RD5_LCD_DB5_SetDigitalInput()    do { TRISDbits.TRISD5 = 1; } while(0)
#define RD5_LCD_DB5_SetDigitalOutput()   do { TRISDbits.TRISD5 = 0; } while(0)
#define RD5_LCD_DB5_SetPullup()          do { WPUDbits.WPUD5 = 1; } while(0)
#define RD5_LCD_DB5_ResetPullup()        do { WPUDbits.WPUD5 = 0; } while(0)
#define RD5_LCD_DB5_SetPushPull()        do { ODCONDbits.ODCD5 = 0; } while(0)
#define RD5_LCD_DB5_SetOpenDrain()       do { ODCONDbits.ODCD5 = 1; } while(0)
#define RD5_LCD_DB5_SetAnalogMode()      do { ANSELDbits.ANSELD5 = 1; } while(0)
#define RD5_LCD_DB5_SetDigitalMode()     do { ANSELDbits.ANSELD5 = 0; } while(0)

// get/set RD6_LCD_DB6 aliases
#define RD6_LCD_DB6_TRIS                 TRISDbits.TRISD6
#define RD6_LCD_DB6_LAT                  LATDbits.LATD6
#define RD6_LCD_DB6_PORT                 PORTDbits.RD6
#define RD6_LCD_DB6_WPU                  WPUDbits.WPUD6
#define RD6_LCD_DB6_OD                   ODCONDbits.ODCD6
#define RD6_LCD_DB6_ANS                  ANSELDbits.ANSELD6
#define RD6_LCD_DB6_SetHigh()            do { LATDbits.LATD6 = 1; } while(0)
#define RD6_LCD_DB6_SetLow()             do { LATDbits.LATD6 = 0; } while(0)
#define RD6_LCD_DB6_Toggle()             do { LATDbits.LATD6 = ~LATDbits.LATD6; } while(0)
#define RD6_LCD_DB6_GetValue()           PORTDbits.RD6
#define RD6_LCD_DB6_SetDigitalInput()    do { TRISDbits.TRISD6 = 1; } while(0)
#define RD6_LCD_DB6_SetDigitalOutput()   do { TRISDbits.TRISD6 = 0; } while(0)
#define RD6_LCD_DB6_SetPullup()          do { WPUDbits.WPUD6 = 1; } while(0)
#define RD6_LCD_DB6_ResetPullup()        do { WPUDbits.WPUD6 = 0; } while(0)
#define RD6_LCD_DB6_SetPushPull()        do { ODCONDbits.ODCD6 = 0; } while(0)
#define RD6_LCD_DB6_SetOpenDrain()       do { ODCONDbits.ODCD6 = 1; } while(0)
#define RD6_LCD_DB6_SetAnalogMode()      do { ANSELDbits.ANSELD6 = 1; } while(0)
#define RD6_LCD_DB6_SetDigitalMode()     do { ANSELDbits.ANSELD6 = 0; } while(0)

// get/set RD7_LCD_DB7 aliases
#define RD7_LCD_DB7_TRIS                 TRISDbits.TRISD7
#define RD7_LCD_DB7_LAT                  LATDbits.LATD7
#define RD7_LCD_DB7_PORT                 PORTDbits.RD7
#define RD7_LCD_DB7_WPU                  WPUDbits.WPUD7
#define RD7_LCD_DB7_OD                   ODCONDbits.ODCD7
#define RD7_LCD_DB7_ANS                  ANSELDbits.ANSELD7
#define RD7_LCD_DB7_SetHigh()            do { LATDbits.LATD7 = 1; } while(0)
#define RD7_LCD_DB7_SetLow()             do { LATDbits.LATD7 = 0; } while(0)
#define RD7_LCD_DB7_Toggle()             do { LATDbits.LATD7 = ~LATDbits.LATD7; } while(0)
#define RD7_LCD_DB7_GetValue()           PORTDbits.RD7
#define RD7_LCD_DB7_SetDigitalInput()    do { TRISDbits.TRISD7 = 1; } while(0)
#define RD7_LCD_DB7_SetDigitalOutput()   do { TRISDbits.TRISD7 = 0; } while(0)
#define RD7_LCD_DB7_SetPullup()          do { WPUDbits.WPUD7 = 1; } while(0)
#define RD7_LCD_DB7_ResetPullup()        do { WPUDbits.WPUD7 = 0; } while(0)
#define RD7_LCD_DB7_SetPushPull()        do { ODCONDbits.ODCD7 = 0; } while(0)
#define RD7_LCD_DB7_SetOpenDrain()       do { ODCONDbits.ODCD7 = 1; } while(0)
#define RD7_LCD_DB7_SetAnalogMode()      do { ANSELDbits.ANSELD7 = 1; } while(0)
#define RD7_LCD_DB7_SetDigitalMode()     do { ANSELDbits.ANSELD7 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/