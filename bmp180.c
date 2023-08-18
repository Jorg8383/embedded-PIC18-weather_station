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
 * This module contains functionality for the the Bosch BMP180 barometric
 * pressure sensor API
 * 
 * 
 * Hardware Description:
 * Bosch BMP180 barometric pressure sensor
*/

#include "mcc_generated_files/mcc.h"
#include "bmp180.h"

/* Internal global variables */
static BMP180_PARAM *pBMP180;

/* Internal function prototypes */
static int32_t calcB5(uint16_t rawTemperature);

/******************************************************************************* 
 * Function to initialise the communication with the BMP180 sensor
 ******************************************************************************/
/*
 * @brief For more details refer to the documentation in the corresponding
 * header file
 * 
 */
uint8_t BMP180_Init(BMP180_PARAM *bmp180) {

    /* Assign BMP180 structure to internal global pointer variable */
    if (bmp180 == 0)
        return 1; // Error 1; null pointer
    pBMP180 = bmp180;
    
    /* Assign the oversampling control register value and the max. conversion
     * time according to the selected hardware pressure sampling accuracy mode */
    switch (pBMP180->oversampling) {
        case BMP180_MODE_ULTRALOWPOWER:
            pBMP180->ossCtrlRegValue = BMP180_CTRL_MEAS_VAL_OSS_0;
            break;
        case BMP180_MODE_STANDARD:
            pBMP180->ossCtrlRegValue = BMP180_CTRL_MEAS_VAL_OSS_1;
            break;
        case BMP180_MODE_HIGHRESOLUTION:
            pBMP180->ossCtrlRegValue = BMP180_CTRL_MEAS_VAL_OSS_2;
            break;
        case BMP180_MODE_ULTRAHIGHRESOLUTION:
            pBMP180->ossCtrlRegValue = BMP180_CTRL_MEAS_VAL_OSS_3;
            break;
    }

    /* Read sensor chip-id to check whether communication is established */
    if (i2c_read1ByteRegister(BMP180_I2C_ADDR, BMP180_REG_CHIP_ID) 
            != BMP180_REG_CHIP_ID_VALUE)
        return 2; // Error 2; communication failed
    
    /* Read calibration data AC1 to AC6 from the EEPROM of the BMP180 */
    pBMP180->calibParam.ac1 = (int16_t) i2c_read2ByteRegister(BMP180_I2C_ADDR, 
            BMP180_CAL_COEFF_AC1_MSB);
    pBMP180->calibParam.ac2 = (int16_t) i2c_read2ByteRegister(BMP180_I2C_ADDR, 
            BMP180_CAL_COEFF_AC2_MSB);
    pBMP180->calibParam.ac3 = (int16_t) i2c_read2ByteRegister(BMP180_I2C_ADDR, 
            BMP180_CAL_COEFF_AC3_MSB);
    pBMP180->calibParam.ac4 = i2c_read2ByteRegister(BMP180_I2C_ADDR, 
            BMP180_CAL_COEFF_AC4_MSB);
    pBMP180->calibParam.ac5 = i2c_read2ByteRegister(BMP180_I2C_ADDR, 
            BMP180_CAL_COEFF_AC5_MSB);
    pBMP180->calibParam.ac6 = i2c_read2ByteRegister(BMP180_I2C_ADDR, 
            BMP180_CAL_COEFF_AC6_MSB);
    
    /* Read calibration data B1 and B2 from the EEPROM of the BMP180 */
    pBMP180->calibParam.b1 = (int16_t) i2c_read2ByteRegister(BMP180_I2C_ADDR, 
            BMP180_CAL_COEFF_B1_MSB);
    pBMP180->calibParam.b2 = (int16_t) i2c_read2ByteRegister(BMP180_I2C_ADDR, 
            BMP180_CAL_COEFF_B2_MSB);

    /* Read calibration data MB, MC, MD from the EEPROM of the BMP180 */
    pBMP180->calibParam.mb = (int16_t) i2c_read2ByteRegister(BMP180_I2C_ADDR, 
            BMP180_CAL_COEFF_MB_MSB);
    pBMP180->calibParam.mc = (int16_t) i2c_read2ByteRegister(BMP180_I2C_ADDR, 
            BMP180_CAL_COEFF_MC_MSB);
    pBMP180->calibParam.md = (int16_t) i2c_read2ByteRegister(BMP180_I2C_ADDR, 
            BMP180_CAL_COEFF_MD_MSB);

    /* Debugging only! Use default parameters as provided in the data sheet */
    #if BMP180_DEBUG_USE_DEFAULT_PARAM == 1
        pBMP180->calibParam.ac1 = 408;
        pBMP180->calibParam.ac2 = -72;
        pBMP180->calibParam.ac3 = -14383;
        pBMP180->calibParam.ac4 = 32741;
        pBMP180->calibParam.ac5 = 32757;
        pBMP180->calibParam.ac6 = 23153;
        pBMP180->calibParam.b1 = 6190;
        pBMP180->calibParam.b2 = 4;
        pBMP180->calibParam.mb = -32768;
        pBMP180->calibParam.mc = -8711;
        pBMP180->calibParam.md = 2868;
        pBMP180->oversampling = BMP180_MODE_ULTRALOWPOWER; // oss = 0
    #endif
    
    return 0; // return successfully
}

/******************************************************************************* 
 * Function to read uncompensated temperature
 ******************************************************************************/
/*
 * @brief For more details refer to the documentation in the corresponding
 * header file
 * 
 */
uint16_t BMP180_ReadRawTemperature(void) {
    
    uint8_t dataBytes[BMP180_TEMPERATURE_DATA_BYTES] = {0};
    uint16_t rawTemperature = 0; // uncompensated temperature UT
    
    /* Write value to oversampling control register */
    i2c_write1ByteRegister(BMP180_I2C_ADDR, BMP180_REG_CTRL_MEAS, 
            BMP180_CTRL_MEAS_VAL_TEMP);

    /* Wait for the max. required conversion time */
    __delay_ms(BMP180_CONV_TIME_TEMP);

    /* Read raw temperature data (16-bit) */
    i2c_readNBytes(BMP180_I2C_ADDR, dataBytes, sizeof(dataBytes));

    /* Typecast data array to temperature variable */
    rawTemperature = (uint16_t)(dataBytes[BMP180_TEMPERATURE_DATA_MSB] << 8 
            | dataBytes[BMP180_TEMPERATURE_DATA_LSB]);
    
    /* Debugging only! Use default parameters as provided in the data sheet */
    #if BMP180_DEBUG_USE_DEFAULT_PARAM == 1
        rawTemperature = 27898;
    #endif

    return rawTemperature;
}


/******************************************************************************* 
 * Function to read uncompensated pressure
 ******************************************************************************/
/*
 * @brief For more details refer to the documentation in the corresponding
 * header file
 * 
 */
uint32_t BMP180_ReadRawPressure(void) {
    
    uint8_t dataBytes[BMP180_PRESSURE_DATA_BYTES] = {0};
    uint32_t rawPressure = 0; // uncompensated pressure data UP
    
    /* Write value to oversampling control register */
    i2c_write1ByteRegister(BMP180_I2C_ADDR, BMP180_REG_CTRL_MEAS, 
            pBMP180->ossCtrlRegValue);

    /* Wait for the max. required conversion time */
    switch (pBMP180->oversampling) {
        case BMP180_MODE_ULTRALOWPOWER:
            __delay_ms(BMP180_CONV_TIME_OSS_0);
            break;
        case BMP180_MODE_STANDARD:
            __delay_ms(BMP180_CONV_TIME_OSS_1);
            break;
        case BMP180_MODE_HIGHRESOLUTION:
            __delay_ms(BMP180_CONV_TIME_OSS_2);
            break;
        case BMP180_MODE_ULTRAHIGHRESOLUTION:
            __delay_ms(BMP180_CONV_TIME_OSS_3);
            break;
    }

    /* Read raw pressure data UP (16 to 19-bit) */
    i2c_readNBytes(BMP180_I2C_ADDR, dataBytes, sizeof(dataBytes));

    /* Typecast data array to pressure variable */
    rawPressure = 
            (uint32_t)((((uint32_t)dataBytes[BMP180_PRESSURE_DATA_MSB] << 16)
            | ((uint32_t)dataBytes[BMP180_PRESSURE_DATA_LSB] << 8)
            | (uint32_t)dataBytes[BMP180_PRESSURE_DATA_XLSB])
            >> (8 - pBMP180->oversampling));

    /* Debugging only! Use default parameters as provided in the data sheet */
    #if BMP180_DEBUG_USE_DEFAULT_PARAM == 1
        rawPressure = 23843;
    #endif

    return rawPressure;
}

/******************************************************************************* 
 * Function to calculate the internal parameter B5
 ******************************************************************************/
/*
 * @brief For more details refer to the documentation in the corresponding
 * header file and the BMP180 data sheet rev 1.2
 * 
 */
static int32_t calcB5(uint16_t rawTemperature) {

    int32_t x1 = 0; 
    int32_t x2 = 0;
    
    x1 = (((int32_t) rawTemperature 
            - (int32_t) pBMP180->calibParam.ac6)
            * (int32_t) pBMP180->calibParam.ac5) >> 15;
    
    if (x1 == 0 && pBMP180->calibParam.md == 0)
        return 0; // return to avoid zero division
    
    x2 = ((int32_t) pBMP180->calibParam.mc << 11) / (x1 + pBMP180->calibParam.md);
    return x1 + x2;    
}


/******************************************************************************* 
 * Function to calculate the true temperature
 ******************************************************************************/
/*
 * @brief For more details refer to the documentation in the corresponding
 * header file and the BMP180 data sheet rev 1.2
 * 
 */
int16_t BMP180_CalcTemperature(uint16_t rawTemperature) {
    
    int16_t temperature = 0;
    temperature = (int16_t)((calcB5(rawTemperature) + 8) >> 4);
    
    return temperature;
}


/******************************************************************************* 
 * Function to calculate the true pressure
 ******************************************************************************/
/*
 * @brief For more details refer to the documentation in the corresponding
 * header file and the BMP180 data sheet rev 1.2
 * 
 */
int32_t BMP180_CalcPressure(uint32_t rawPressure, uint32_t rawTemperature) {
    
    int32_t pressure = 0;
    int32_t x1 = 0;
    int32_t x2 = 0;
    int32_t x3 = 0;
    int32_t b3 = 0;
    int32_t b6 = 0;
    uint32_t b4 = 0;
    uint32_t b7 = 0;
    
    /* Calculate B6 */
    b6 = calcB5(rawTemperature) - 4000;
    
    /* Calculate B3 */
    x1 = (pBMP180->calibParam.b2 * ((b6 * b6) >> 12)) >> 11;
    x2 = (pBMP180->calibParam.ac2 * b6) >> 11;
    x3 = x1 + x2;
    b3 = ((((int32_t)pBMP180->calibParam.ac1 * 4 + x3) 
            << pBMP180->oversampling) + 2) >> 2;
    
    /* Calculate B4 */
    x1 = (pBMP180->calibParam.ac3 * b6) >> 13;
    x2 = (pBMP180->calibParam.b1 * ((b6 * b6) >> 12)) >> 16;
    x3 = ((x1 + x2) + 2) >> 2;
    b4 = (pBMP180->calibParam.ac4 * (uint32_t)(x3 + 32768)) >> 15;
    
    /* Calculate B7 */
    b7 = ((uint32_t)(rawPressure - b3))
            * (50000 >> pBMP180->oversampling);
   
    
    /* Calculate pressure in Pa */
    if (b4 == 0) 
        return 0;  // return to avoid zero division          

    if (b7 < 0x80000000) 
        pressure = (b7 << 1) / b4;
    else
        pressure = (b7 / b4) << 1;
    x1 = (pressure >> 8);
    x1 *= x1;
    x1 = (x1 * 3038) >> 16;
    x2 = (-7357 * pressure) >> 16;
    pressure += (x1 + x2 + 3791) >> 4;
    
    return pressure;
}
