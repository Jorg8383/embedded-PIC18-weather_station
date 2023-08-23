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

// Define the device states
typedef enum {
    STATE_UPDATE_TEMPERATURE,
    STATE_UPDATE_PRESSURE,
    STATE_UPDATE_ALTITUDE,
    STATE_WAIT
} DeviceState;

// Structure to hold state context information
typedef struct {
    uint8_t dummyInfo; // Not used yet
} DeviceContext;

// Function pointer type for state handler functions
typedef void (*stateHandler)(DeviceState*, DeviceContext*);

// Function prototypes for state machine functions
void stateMachineInit(DeviceState *currentState, DeviceContext *context);
void stateMachineRun(DeviceState *currentState, DeviceContext *context);


#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* STATE_H */

