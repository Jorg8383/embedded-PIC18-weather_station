/**
 * 
 * File:                bmp180.c
 * Author:              J. Striebel
 * Project:             Weather Station
 * Device:              PIC18F47Q10 @ 16 MHz internal OSC, 5V
 * Platform:            Curiosity HPC board (DM164136)
 * Sensor:              Bosch BMP180 Digital pressure sensor 
 * Compiler:            XC8 (v2.41)
 * IDE:                 MPLAB X (v6.10), MCC (5.3.7)
 * Program version:     1.0
 
 * Program Description:
 * This module represents an Application Programming Interface (API) for the 
 * Bosch BMP180 digital pressure sensor.
 * 
 * 
 * Hardware Description:
 * 
*/

#include <stdio.h>
#include "bmp180.h"

/* Internal global variables */
static BMP180_PARAM *pBMP180;

/******************************************************************************
 * Function: 
 *
 * Returns: 
 * 
 * Description: 
 ******************************************************************************/
uint8_t BMP180_Init(BMP180_PARAM *param) {
    
    // Assign BMP180 to internal global pointer variable
    if (param == 0)
        return 1 // Return error value 1 == null pointer
    pBMP180 = param;

    
}

/******************************************************************************
 * Function: 
 *
 * Returns: 
 * 
 * Description: 
 ******************************************************************************/


/******************************************************************************
 * Function: 
 *
 * Returns: 
 * 
 * Description: 
 ******************************************************************************/
/******************************************************************************
 * Function: 
 *
 * Returns: 
 * 
 * Description: 
 ******************************************************************************/
/******************************************************************************
 * Function: 
 *
 * Returns: 
 * 
 * Description: 
 ******************************************************************************/
/******************************************************************************
 * Function: 
 *
 * Returns: 
 * 
 * Description: 
 ******************************************************************************/
