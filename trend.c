/**
 * 
 * File:                trend.c
 * Author:              J. Striebel
 * Project:             Weather Station
 * Device:              PIC18F47Q10 @ 16 MHz internal OSC, 5V
 * Platform:            Curiosity HPC board (DM164136)
 * Compiler:            XC8 (v2.41)
 * IDE:                 MPLAB X (v6.10), MCC (5.3.7)
 
 * Description:
 * ------------
 * This module contains functions for recording pressure reading samples and for
 * calculating the moving average across these pressure values. 
*/


#include "trend.h"

// Global variables
volatile _Bool updatePressureReading;
int32_t pressureReadings[MOVING_AVERAGE_WINDOW_SIZE];
uint8_t numberOfValidReadings = 0;


/******************************************************************************* 
 * Function to initialise the pressure readings array
 ******************************************************************************/
/*
 * @brief This function initialises the pressure reading array
 * 
 * @param None
 * 
 * @return void 
 * 
*/
void initPressureReadings(void) {
    
    for (uint8_t i = 0 ; i < MOVING_AVERAGE_WINDOW_SIZE ; i++) {
        pressureReadings[i] = 0;
    }
}


/******************************************************************************* 
 * Function to calculate the moving pressure average
 ******************************************************************************/
/*
 * @brief This function calculates the moving pressure average bases on the
 * number of reading samples.
 * 
 * @param None
 * 
 * @return moving average (uint32_t)
 * 
*/
int32_t calcPressureMovingAverage(void) {
    
    int32_t sum = 0;
    int32_t result = 0;
    
    for (uint8_t i = 0 ; i < numberOfValidReadings ; i++) {
        sum += pressureReadings[i];
    } 
    if (numberOfValidReadings > 0) {
        result = sum / numberOfValidReadings;
    }
    
    return result;
}

/******************************************************************************* 
 * Function to update pressure readings
 ******************************************************************************/
/*
 * @brief This function updates the pressure readings by adding a new value to
 * the pressure readings array. In case the array is fully populated, the oldest
 * data entry will be overwritten.
 * 
 * @param None
 * 
 * @return moving average (uint32_t)
 * 
*/
void updatePressureReadings(int32_t pressure) {

    static uint8_t currentReadingIndex = 0;
    
    pressureReadings[currentReadingIndex++] = pressure;
    if (currentReadingIndex >= MOVING_AVERAGE_WINDOW_SIZE)
        currentReadingIndex = 0;
    if (numberOfValidReadings < MOVING_AVERAGE_WINDOW_SIZE)
        numberOfValidReadings++;
    
    updatePressureReading = false;
}

/******************************************************************************* 
 * Interrupt service routine for timer 0
 ******************************************************************************/
/*
 * @brief This ISR is invoked every minute by timer 0 to set a flag for updating
 * the pressure readings.
 * 
 * @param None
 * 
 * @return moving average (uint32_t)
 * 
*/
void timer0ISR(void) {
    
    updatePressureReading = true;
}