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
 * 
*/


#include "trend.h"

// Global variables
volatile _Bool updatePressureReading;
uint32_t pressureReadings[MOVING_AVERAGE_WINDOW_SIZE];
uint8_t numberOfValidReadings = 0;


void initPressureReadings(void) {
    
    for (uint8_t i = 0 ; i < MOVING_AVERAGE_WINDOW_SIZE ; i++) {
        pressureReadings[i] = 0;
    }
}


uint32_t calcPressureMovingAverage(void) {
    
    uint32_t sum = 0;
    uint32_t result = 0;
    
    for (uint8_t i = 0 ; i < numberOfValidReadings ; i++) {
        sum += pressureReadings[i];
    } 
    if (numberOfValidReadings > 0) {
        result = sum / numberOfValidReadings;
    }
    
    return result;
}


void updatePressureReadings(uint32_t pressure) {

    uint8_t currentReadingIndex = 0;
    
    pressureReadings[currentReadingIndex++] = pressure;
    if (currentReadingIndex >= MOVING_AVERAGE_WINDOW_SIZE)
        currentReadingIndex = 0;
    if (numberOfValidReadings < MOVING_AVERAGE_WINDOW_SIZE)
        numberOfValidReadings++;
    
    updatePressureReading = false;
}


void timer0ISR(void) {
    
    updatePressureReading = true;
}