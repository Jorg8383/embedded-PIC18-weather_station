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
 
 * Description:
 * ------------
 * Test routine of the Bosch BMP180 barometric pressure sensor
 * 
*/

#include "mcc_generated_files/mcc.h"
#include "bmp180.h"

#if BMP180_DEBUG_COMPILE_TEST
    BMP180_PARAM bmp180;
    _Bool initialiseSensor = true;
    uint8_t initResult = 0xFF;

    extern int32_t calculateAltitude(uint32_t pressure);

    void BMP180_TestRoutine(void){

        uint16_t rawTemperature;
        uint32_t rawPressure;
        int16_t temperature;
        int32_t pressure;
        int16_t altitude;

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
            printf("BMP180 - raw pressure: %ld\n", rawPressure);

            temperature = BMP180_CalcTemperature(rawTemperature);
            printf("BMP180 - temperature: %d\n", temperature);        

            pressure = BMP180_CalcPressure(rawPressure, rawTemperature);
            printf("BMP180 - pressure: %ld\n", pressure);

            altitude = BMP180_CalcAltitude(pressure);
            printf("BMP180 - altitude: %d\n", altitude);

    //        altitude = BMP180_CalculateAltitude(pressure);
    //        printf("BMP180 - altitude: %d\n", altitude);



            printf("----------------------------------\n");
        }

        __delay_ms(2000);


    }
#endif