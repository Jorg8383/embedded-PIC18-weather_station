/**
 * 
 * File:                state.c
 * Author:              J. Striebel
 * Project:             Weather Station
 * Device:              PIC18F47Q10 @ 16 MHz internal OSC, 5V
 * Platform:            Curiosity HPC board (DM164136)
 * Compiler:            XC8 (v2.41)
 * IDE:                 MPLAB X (v6.10), MCC (5.3.7)
 
 * Program Description:
 * 
*/

#include "state.h"
#include "bmp180.h"
#include "lcd.h"
#include "lcd_app.h"


volatile uint32_t stateWaitCount;

// Function prototypes for state handler functions
static void stateInit(DeviceState *pCurrentState, 
        DeviceContext *pContext);
static void stateUpdateMeasurement(DeviceState *pCurrentState, 
        DeviceContext *pContext);
static void stateDisplayTemperature(DeviceState *pCurrentState, 
        DeviceContext *pContext);
static void stateDisplayPressure(DeviceState *pCurrentState, 
        DeviceContext *pContext);
static void stateDisplayAltitude(DeviceState *pCurrentState,
        DeviceContext *pContext);
static void stateWait(DeviceState *pCurrentState,
        DeviceContext *pContext);
static void stateFinal(DeviceState *pCurrentState,
        DeviceContext *pContext);

// Declare an array of function pointers for state handlers in flash memory
static void (*const pStateHandlers[])(DeviceState*, DeviceContext*) = {
    &stateInit,
    &stateUpdateMeasurement,
    &stateDisplayTemperature,
    &stateWait,
    &stateDisplayPressure,
    &stateWait,
    &stateDisplayAltitude,
    &stateWait,
    &stateFinal
};


// Function that initialises the state machine
void initStateMachine(DeviceState *pCurrentState, DeviceContext *pContext) {
    
    // Check for null pointers
    if (pContext == 0 || pCurrentState == 0)
        return;
    
    // Initialise context if needed
    pContext->altitude = 0;
    pContext->pressure = 0;
    pContext->temperature = 0;
    
    // Set the initial state
    *pCurrentState = STATE_INIT;
}


// Function to run the state machine and invoke the current state function
void runStateMachine(DeviceState *pCurrentState, DeviceContext *pContext) {
    
    // Check for null pointers
    if (pContext == 0 || pCurrentState == 0)
        return;

    // Call the current state's handler function
    (pStateHandlers[*pCurrentState])(pCurrentState, pContext);
}


static void stateInit(DeviceState *pCurrentState, DeviceContext *pContext) {
    
    const uint8_t charPos = 16;
    uint8_t i;
    
    // Print a welcome message on the LCD very right position
    LCD_Clear();
    LCD_SetCursor(LCD_FIRST_LINE, charPos);
    LCD_PrintString(getLcdText(LCD_TXT_WELCOME));
    __delay_ms(100);
    
    // Scroll the welcome text from right to left 
    for (i = 0; i <= charPos + strlen(getLcdText(LCD_TXT_WELCOME)); i++) {
        LCD_ShiftDisplayLeft();
        __delay_ms(100);    
    }
    
    // Transition to the following state
    (*pCurrentState)++;     
}


static void stateUpdateMeasurement(DeviceState *pCurrentState, 
        DeviceContext *pContext){
    
    uint32_t rawTemperature;
    
    // Get raw sensor data and calculate temperature, pressure and altitude
    rawTemperature = BMP180_ReadRawTemperature();
    pContext->temperature = BMP180_CalcTemperature(rawTemperature);
    pContext->pressure = BMP180_CalcPressure(BMP180_ReadRawPressure(), 
            rawTemperature);
    pContext->altitude = BMP180_CalcAltitude(pContext->pressure);
            
    // Transition to the following state
    (*pCurrentState)++; 
}



static void stateDisplayTemperature(DeviceState *pCurrentState, 
        DeviceContext *pContext) 
{   
    
    uint8_t charPos;
    
    LCD_Clear();
    charPos = (uint8_t)(LCD_CHAR_LENGTH - strlen(getLcdText(LCD_TXT_TEMPERATURE)));
    LCD_SetCursor(LCD_FIRST_LINE, charPos);
    LCD_PrintString(getLcdText(LCD_TXT_WELCOME));
    
    // Transition to the following state
    (*pCurrentState)++; 
}

static void stateDisplayPressure(DeviceState *pCurrentState,
        DeviceContext *pContext)
{
    
    int16_t hpa; // Pressure in hPa (100 Pa = 1 hPa = 1 mbar)
    
    // Convert Pa to hPa
    hpa = pContext->pressure / 100; // convert Pa to hPa
    
    
    // Transition to the following state
    (*pCurrentState)++; 
}

static void stateDisplayAltitude(DeviceState *pCurrentState,
        DeviceContext *pContext) 
{
    // TODO display value
    
            
    // Transition to the following state
    (*pCurrentState)++; 
}

static void stateWait(DeviceState *pCurrentState, DeviceContext *pContext) {
    
    TMR0_StartTimer();
    
    if (TMR0_HasOverflowOccured()){
        TMR0_StopTimer();
        // Transition to the following state
        (*pCurrentState)++; 
    }    
}

static void stateFinal(DeviceState *pCurrentState, DeviceContext *pContext) {
    
    (*pCurrentState) = STATE_UPDATE_MEASUREMENT;    
}
