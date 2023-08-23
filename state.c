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


// Function prototypes for state handler functions
static void stateInit(DeviceState *pCurrentState, DeviceContext *pContext);
static void stateUpdateTemperature(DeviceState *pCurrentState, DeviceContext *pContext);
static void stateUpdatePressure(DeviceState *pCurrentState, DeviceContext *pContext);
static void stateUpdateAltitude(DeviceState *pCurrentState, DeviceContext *pContext);
static void stateWait(DeviceState *pCurrentState, DeviceContext *pContext);
static void stateFinal(DeviceState *pCurrentState, DeviceContext *pContext);

// Declare an array of function pointers for state handlers in flash memory
static void (*const pStateHandlers[])(DeviceState*, DeviceContext*) = {
    &stateInit,
    &stateUpdateTemperature,
    &stateWait,
    &stateUpdatePressure,
    &stateWait,
    &stateUpdateAltitude,
    &stateWait,
    &stateFinal
};


// Function that initialises the state machine
void initStateMachine(DeviceState *pCurrentState, DeviceContext *pContext) {
    
    // Check for null pointers
    if (pContext == 0 || pCurrentState == 0)
        return;
    
    // Initialise context if needed
    pContext->dummyInfo = 0;
    
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
    
    
    
    (*pCurrentState)++; // Go to the next state
    
}

