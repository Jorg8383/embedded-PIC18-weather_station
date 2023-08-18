/**
 * 
 * File Name:           bmp180_test.c
 * Author:              J. Striebel
 * Project:             Weather Station
 * Device:              PIC18F47Q10 @ 16 MHz internal OSC, 5V 
 * Platform:            Curiosity HPC board (DM164136)
 * Compiler:            XC8 (v2.41)
 * IDE:                 MPLAB X (v6.10), MCC (5.3.7)
 * Program version:     1.0
 
 * Program Description:
 * xxx
 * 
 * Hardware Description:
 * xxx
 * 
*/

#include "mcc_generated_files/mcc.h"
#include "bmp180.h"

BMP180_PARAM bmp180;
_Bool initialiseSensor = true;
uint8_t initResult = 0xFF;

void BMP180_TestRoutine(void){

    uint16_t rawTemperature;
    uint32_t rawPressure;
    int16_t temperature;
    int32_t pressure;
    
    if (initialiseSensor) {
        initialiseSensor = false;
        initResult = BMP180_Init(&bmp180);
        printf("BMP180 initialisation result: %d\n", initResult);
    }
    
    __delay_ms(100);
    
    if (initResult == 0){
        
        rawTemperature = BMP180_ReadRawTemperature();
        printf("BMP180 - raw temperature: %d\n", rawTemperature);

        rawPressure = BMP180_ReadRawPressure();
        printf("BMP180 - raw pressure: %d\n", rawPressure);
        
        temperature = BMP180_CalcTemperature(rawTemperature);
        printf("BMP180 - temperature: %d\n", temperature);        
        
        pressure = BMP180_CalcPressure(rawPressure, rawTemperature);
        printf("BMP180 - pressure: %d\n", pressure);
        
        printf("----------------------------------\n");
    }

    __delay_ms(2000);
    
    
}
