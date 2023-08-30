/* 
 * File:                trend.h
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

#ifndef TREND_H
#define	TREND_H

#include "mcc_generated_files/mcc.h"

#define MOVING_AVERAGE_WINDOW_SIZE 60

void initPressureReadings(void);
uint16_t calcPressureMovingAverage(void);
void updatePressureReadings(uint32_t pressure);
void timer0ISR(void);

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* TREND_H */

