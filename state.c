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


// Function to initialise the state machine
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
    pStateHandlers[*pCurrentState](pCurrentState, pContext);
}


static void stateInit(DeviceState *pCurrentState, DeviceContext *pContext) {
    
    uint8_t i;
    uint8_t cursorPos;
   
    // Initialise Timer0
//    TMR0_StopTimer();
    
    // Scroll the welcome text from right to left 
    cursorPos = (uint8_t)(LCD_CHAR_LENGTH - strlen(getLcdText(LCD_TXT_WELCOME)));
    LCD_Clear();
    LCD_SetCursor(LCD_FIRST_LINE, cursorPos);
    LCD_PrintString(getLcdText(LCD_TXT_WELCOME));
    __delay_ms(1000);
   
    for (i = 0; i < LCD_CHAR_LENGTH ; i++) {
        LCD_ShiftDisplayLeft();
        __delay_ms(250);    
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
    
    uint8_t cursorPos;
    char strTemperature[LCD_TEMPERATURE_BUFFER_SIZE - 1];
    
    // Print the headline "Temperature" in the centre of the first line
    LCD_Clear();
    cursorPos = (uint8_t)(LCD_CHAR_LENGTH - strlen(getLcdText(LCD_TXT_TEMPERATURE))) / 2;
    LCD_SetCursor(LCD_FIRST_LINE, cursorPos);
    LCD_PrintString(getLcdText(LCD_TXT_TEMPERATURE));

    // Print the temperature value and its unit in the second line
    convertTemperatureToString(pContext->temperature, strTemperature);
    cursorPos = (uint8_t)(LCD_CHAR_LENGTH - strlen(strTemperature) - 3) / 2;    
    LCD_SetCursor(LCD_SECOND_LINE, cursorPos);
    LCD_PrintString(strTemperature);
    LCD_ShiftCursorRight();
    LCD_PrintCharacter(0xdf); // 0xdf = Celsius degree symbol
    LCD_PrintCharacter('C');        
    
    // Transition to the following state
    (*pCurrentState)++; 
}


static void stateDisplayPressure(DeviceState *pCurrentState,
        DeviceContext *pContext)
{
    
    uint8_t cursorPos;
    int16_t hpa; // Pressure in hPa (100 Pa = 1 hPa = 1 mbar)
   
    // Convert Pa to hPa
    hpa = pContext->pressure / 100; // convert Pa to hPa
    
    // Print the headline "Pressure" in the centre of the first line
    LCD_Clear();
    cursorPos = (uint8_t)(LCD_CHAR_LENGTH - strlen(getLcdText(LCD_TXT_PRESSURE))) / 2;
    LCD_SetCursor(LCD_FIRST_LINE, cursorPos);
    LCD_PrintString(getLcdText(LCD_TXT_PRESSURE));

    // Print the pressure value and its unit in the second line
    LCD_SetCursor(LCD_SECOND_LINE, 4);
    LCD_PrintInteger(hpa, INT_BASE_DECIMAL);
    LCD_ShiftCursorRight();
    LCD_PrintString(getLcdText(LCD_TXT_PRESSURE_UNIT));
    
    // Transition to the following state
    (*pCurrentState)++; 
}


static void stateDisplayAltitude(DeviceState *pCurrentState,
        DeviceContext *pContext) 
{
    uint8_t cursorPos;
   
    // Print the headline "Altitude" in the centre of the first line
    LCD_Clear();
    cursorPos = (uint8_t)(LCD_CHAR_LENGTH - strlen(getLcdText(LCD_TXT_ALTITUDE))) / 2;
    LCD_SetCursor(LCD_FIRST_LINE, cursorPos);
    LCD_PrintString(getLcdText(LCD_TXT_ALTITUDE));

    // Print the altitude value and its unit in the second line
    LCD_SetCursor(LCD_SECOND_LINE, 3);
    LCD_PrintInteger(pContext->altitude, INT_BASE_DECIMAL);
    LCD_ShiftCursorRight();
    LCD_PrintString(getLcdText(LCD_TXT_ALTITUDE_UNIT));
    
    // Transition to the following state
    (*pCurrentState)++; 
}


static void stateWait(DeviceState *pCurrentState, DeviceContext *pContext) {
    
//    __delay_ms(3000);
//    (*pCurrentState)++;

    TMR0_StartTimer();
    
    if (TMR0_HasOverflowOccured()){
        TMR0_StopTimer();
        PIR0bits.TMR0IF = 0;
        // Transition to the following state
        (*pCurrentState)++; 
    }    
}


static void stateFinal(DeviceState *pCurrentState, DeviceContext *pContext) {
    
    (*pCurrentState) = STATE_UPDATE_MEASUREMENT;    
}
