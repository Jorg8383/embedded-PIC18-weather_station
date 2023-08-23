/* 
 * File:                state.h
 * Author:              J. Striebel
 * Project:             Weather Station
 * Device:              PIC18F47Q10 @ 16 MHz internal OSC, 5V
 * Platform:            Curiosity HPC board (DM164136)
 * Compiler:            XC8 (v2.41)
 * IDE:                 MPLAB X (v6.10), MCC (5.3.7)
 * Program version:     1.0
 
 * Program Description:
 * --------------------
 * 
 * Hardware Description:
 * --------------------
 *    
 */

#ifndef STATE_H
#define	STATE_H

#include "mcc_generated_files/mcc.h"

// Define the device states
typedef enum {
    STATE_INIT,
    STATE_UPDATE_MEASUREMENT,
    STATE_DISPLAY_TEMPERATURE,
    STATE_WAIT_1,        
    STATE_DISPLAY_PRESSURE,
    STATE_WAIT_2,        
    STATE_DISPLAY_ALTITUDE,
    STATE_WAIT_3,
    STATE_FINAL        
} DeviceState;

// Structure to hold state context information
typedef struct {
    int16_t temperature;
    int32_t pressure;
    int16_t altitude;
} DeviceContext;

// Function pointer type for state handler functions
typedef void (*stateHandler)(DeviceState*, DeviceContext*);

// Function prototypes for state machine functions
void initStateMachine(DeviceState *pCurrentState, DeviceContext *pContext);
void runStateMachine(DeviceState *pCurrentState, DeviceContext *pContext);


#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* STATE_H */

