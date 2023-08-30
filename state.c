/**
 * 
 * File:                state.c
 * Author:              J. Striebel
 * Project:             Weather Station
 * Device:              PIC18F47Q10 @ 16 MHz internal OSC, 5V
 * Platform:            Curiosity HPC board (DM164136)
 * Compiler:            XC8 (v2.41)
 * IDE:                 MPLAB X (v6.10), MCC (5.3.7)
 
 * Description:
 * ------------
 * This module comprises definitions and functionality required to realise a 
 * finite state machine, which defines various device states.
*/

#include "state.h"
#include "bmp180.h"
#include "lcd.h"
#include "lcd_app.h"
#include "trend.h"

// Global variables
static _Bool stateHasChanged;

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
static void stateDisplayWeatherTrend(DeviceState *pCurrentState, 
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
    &stateDisplayWeatherTrend,
    &stateWait,            
    &stateFinal
};


/******************************************************************************* 
 * Function to initialise the finite state machine
 ******************************************************************************/
/*
 * @brief This initialisation function needs to be invoked once 
 * 
 * @param pointer to the current state, pointer to the device context
 * 
 * @return void 
 * 
*/
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


/******************************************************************************* 
 * Function to run the finite state machine
 ******************************************************************************/
/*
 * @brief This drives the finite state machine and needs to be invoked
 * cyclically
 * 
 * @param pointer to the current state, pointer to the device context
 * 
 * @return void 
 * 
*/
void runStateMachine(DeviceState *pCurrentState, DeviceContext *pContext) {
    
    static DeviceState prevState;
    
    // Check for null pointers
    if (pContext == 0 || pCurrentState == 0)
        return;

    // Call the current state's handler function
    pStateHandlers[*pCurrentState](pCurrentState, pContext);

    // Set a flag if the state has changed
    stateHasChanged = (prevState != *pCurrentState) ? true : false;
    prevState = *pCurrentState;
    
}

/******************************************************************************* 
 * State: Initialise 
 ******************************************************************************/
/*
 * @brief This state is the initialisation routine of the state machine
 * 
 * @param pointer to the current state, pointer to the device context
 * 
 * @return void 
 * 
*/
static void stateInit(DeviceState *pCurrentState, DeviceContext *pContext) {
    
    uint8_t i;
       
    // Scroll the welcome text from right to left 
    LCD_Clear();
    LCD_SetCursor(LCD_FIRST_LINE, 0);
    LCD_PrintString(getLcdText(LCD_TXT_WELCOME));
    __delay_ms(1000);
   
    for (i = 0; i < LCD_CHAR_LENGTH ; i++) {
        LCD_ShiftDisplayLeft();
        __delay_ms(250);    
    }
    
    // Transition to the following state
    (*pCurrentState)++;     
}


/******************************************************************************* 
 * State: Update Measurement 
 ******************************************************************************/
/*
 * @brief This state reads the raw sensor data and calculates pressure,
 * temperature, and altitude values.
 * 
 * @param pointer to the current state, pointer to the device context
 * 
 * @return void 
 * 
*/
static void stateUpdateMeasurement(DeviceState *pCurrentState, 
        DeviceContext *pContext){
    
    uint16_t rawTemperature;
   
    // Get raw sensor data and calculate temperature, pressure and altitude
    rawTemperature = BMP180_ReadRawTemperature();
    pContext->temperature = BMP180_CalcTemperature(rawTemperature);
    pContext->pressure = BMP180_CalcPressure(BMP180_ReadRawPressure(), 
            rawTemperature);
    pContext->altitude = BMP180_CalcAltitude(pContext->pressure);
    
    if (updatePressureReading) {
        updatePressureReadings(pContext->pressure);
    }
    
    // Transition to the following state
    (*pCurrentState)++; 
}


/******************************************************************************* 
 * State: Display Temperature
 ******************************************************************************/
/*
 * @brief This state displays the temperature on the LCD.
 * 
 * @param pointer to the current state, pointer to the device context
 * 
 * @return void 
 * 
*/
static void stateDisplayTemperature(DeviceState *pCurrentState, 
        DeviceContext *pContext) 
{   
    
    uint8_t cursorPos;
    char strTemperature[LCD_TEMPERATURE_BUFFER_SIZE - 1];
    
    // Print the headline "Temperature" in the centre of the first line
    LCD_Clear();
    cursorPos = (uint8_t)(LCD_CHAR_LENGTH 
            - strlen(getLcdText(LCD_TXT_TEMPERATURE))) / 2;
    LCD_SetCursor(LCD_FIRST_LINE, cursorPos);
    LCD_PrintString(getLcdText(LCD_TXT_TEMPERATURE));

    // Print the temperature value and its unit in the centre of the second line
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


/******************************************************************************* 
 * State: Display Pressure
 ******************************************************************************/
/*
 * @brief This state displays the pressure on the LCD.
 * 
 * @param pointer to the current state, pointer to the device context
 * 
 * @return void 
 * 
*/
static void stateDisplayPressure(DeviceState *pCurrentState,
        DeviceContext *pContext)
{
    
    int16_t hpa; // Pressure in hPa (100 Pa = 1 hPa = 1 mbar)
    uint8_t cursorPos;
    char itoaBuffer[12];
   
    // Convert Pa to hPa
    hpa = (int16_t)(pContext->pressure / 100); // convert Pa to hPa
    
    // Print the headline "Pressure" in the centre of the first line
    LCD_Clear();
    cursorPos = (uint8_t)(LCD_CHAR_LENGTH 
            - strlen(getLcdText(LCD_TXT_PRESSURE))) / 2;
    LCD_SetCursor(LCD_FIRST_LINE, cursorPos);
    LCD_PrintString(getLcdText(LCD_TXT_PRESSURE));

    // Print the pressure value and its unit in the centre of the second line
    cursorPos = (uint8_t)(LCD_CHAR_LENGTH 
            - strlen(itoa(hpa, itoaBuffer, sizeof(itoaBuffer)))
            - strlen(getLcdText(LCD_TXT_PRESSURE_UNIT))) / 2;    
    LCD_SetCursor(LCD_SECOND_LINE, cursorPos);
    LCD_PrintInteger(hpa, INT_BASE_DECIMAL);
    LCD_ShiftCursorRight();
    LCD_PrintString(getLcdText(LCD_TXT_PRESSURE_UNIT));
    
    // Transition to the following state
    (*pCurrentState)++; 
}

/******************************************************************************* 
 * State: Display Altitude
 ******************************************************************************/
/*
 * @brief This state displays the altitude on the LCD.
 * 
 * @param pointer to the current state, pointer to the device context
 * 
 * @return void 
 * 
*/
static void stateDisplayAltitude(DeviceState *pCurrentState,
        DeviceContext *pContext) 
{
    uint8_t cursorPos;
    char itoaBuffer[12];
   
    // Print the headline "Altitude" in the centre of the first line
    LCD_Clear();
    cursorPos = (uint8_t)(LCD_CHAR_LENGTH 
            - strlen(getLcdText(LCD_TXT_ALTITUDE))) / 2;
    LCD_SetCursor(LCD_FIRST_LINE, cursorPos);
    LCD_PrintString(getLcdText(LCD_TXT_ALTITUDE));

    // Print the altitude value and its unit in the centre of the second line
    cursorPos = (uint8_t)(LCD_CHAR_LENGTH 
            - strlen(itoa(pContext->altitude, itoaBuffer, sizeof(itoaBuffer)))
            - strlen(getLcdText(LCD_TXT_ALTITUDE_UNIT))) / 2;    
    LCD_SetCursor(LCD_SECOND_LINE, cursorPos);
    LCD_PrintInteger(pContext->altitude, INT_BASE_DECIMAL);
    LCD_ShiftCursorRight();
    LCD_PrintString(getLcdText(LCD_TXT_ALTITUDE_UNIT));
    
    // Transition to the following state
    (*pCurrentState)++; 
}


/******************************************************************************* 
 * State: Display Weather Trend
 ******************************************************************************/
/*
 * @brief This state displays the weather trend
 * 
 * @param pointer to the current state, pointer to the device context
 * 
 * @return void 
 * 
*/
static void stateDisplayWeatherTrend(DeviceState *pCurrentState,
        DeviceContext *pContext) 
{
    
    int32_t movingAveragePa, upwardThresholdPa, downwardThresholdPa;
    int16_t delta_hPa;
    LcdTextIndex trendTxt;
    uint8_t cursorPos;
    char itoaBuffer[5];
    
    // Calculate pressure moving average, thresholds and delta pressure   
    movingAveragePa = calcPressureMovingAverage();
    upwardThresholdPa = (movingAveragePa * 1) / 100; // 1% higher
    downwardThresholdPa = (movingAveragePa * 1) / 100; // 1% lower
    delta_hPa = (int16_t)((pContext->pressure - movingAveragePa) / 100);
    
    // Evaluate the weather trend and pick the appropriate text to be displayed
    if (numberOfValidReadings > 0 && 
            pContext->pressure > (movingAveragePa + upwardThresholdPa)) {
        trendTxt = LCD_TXT_TREND_UPWARD; // upward trend
    } else if (numberOfValidReadings > 0 &&
            pContext->pressure < (movingAveragePa - downwardThresholdPa)) {
        trendTxt = LCD_TXT_TREND_DOWNWARD; // downward trend
    } else {
        trendTxt = LCD_TXT_TREND_STABLE;
    }
           
    // Display the weather trend in the first line
    LCD_Clear();
    LCD_SetCursor(LCD_FIRST_LINE, 0);
    LCD_PrintString(getLcdText(LCD_TXT_WEATHER_TREND));
    LCD_ShiftCursorRight();
    LCD_PrintString(getLcdText(trendTxt));
    
    // Display delta hPa and number of reading samples in the second line
    LCD_SetCursor(LCD_SECOND_LINE, 0);
    LCD_PrintString(getLcdText(LCD_TXT_DELTA_PRESSURE));
    LCD_PrintInteger(delta_hPa, INT_BASE_DECIMAL);
    LCD_ShiftCursorRight();
    LCD_PrintString(getLcdText(LCD_TXT_PRESSURE_UNIT));    
    cursorPos = (uint8_t)(LCD_CHAR_LENGTH - strlen(itoa(numberOfValidReadings,
            itoaBuffer, sizeof(itoaBuffer))));
    LCD_SetCursor(LCD_SECOND_LINE, cursorPos);
    LCD_PrintInteger(numberOfValidReadings, INT_BASE_DECIMAL);
        
    // Transition to the following state
    (*pCurrentState)++;
    
    
}/******************************************************************************* 
 * State: Wait
 ******************************************************************************/
/*
 * @brief This state delays switching to the next state.
 * 
 * @param pointer to the current state, pointer to the device context
 * 
 * @return void 
 * 
*/
static void stateWait(DeviceState *pCurrentState, DeviceContext *pContext) {
   
    __delay_ms(4000);
    // Transition to the following state
    (*pCurrentState)++;
}


/******************************************************************************* 
 * State: Final
 ******************************************************************************/
/*
 * @brief This is the final state, pointing back to the origin state
 * 
 * @param pointer to the current state, pointer to the device context
 * 
 * @return void 
 * 
*/
static void stateFinal(DeviceState *pCurrentState, DeviceContext *pContext) {
    
    (*pCurrentState) = STATE_INIT;    
}
