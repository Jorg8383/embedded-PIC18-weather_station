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

#include "mcc_generated_files/mcc.h"
#include "bmp180.h"

/* Internal global variables */
static BMP180_PARAM *pBMP180;

/******************************************************************************
 * Function: uint8_t BMP180_Init(BMP180_PARAM *bmp180)
 *
 * Returns: 
 * 
 * Description: 
 ******************************************************************************/
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

    return 0; // return successfully
}

/******************************************************************************
 * Function: 
 *
 * Returns: 
 * 
 * Description: 
 ******************************************************************************/
uint16_t BMP180_ReadUncompensatedTemperature(void) {
    
    uint8_t dataBytes[BMP180_TEMPERATURE_DATA_BYTES] = {0};
    uint16_t ut = 0; // uncompensated temperature UT
    
    /* Write value to oversampling control register */
    i2c_write1ByteRegister(BMP180_I2C_ADDR, BMP180_REG_CTRL_MEAS, 
            BMP180_CTRL_MEAS_VAL_TEMP);

    /* Wait for the max. required conversion time */
    __delay_ms(BMP180_CONV_TIME_TEMP);

    /* Read raw temperature data (16-bit) */
    i2c_readNBytes(BMP180_I2C_ADDR, dataBytes, sizeof(dataBytes));

    /* Typecast data array to temperature variable */
    ut = (uint16_t)(dataBytes[BMP180_TEMPERATURE_DATA_MSB] << 8 
            | dataBytes[BMP180_TEMPERATURE_DATA_LSB]);
    
    return ut;
}


/******************************************************************************
 * Function: 
 *
 * Returns: 
 * 
 * Description: 
 ******************************************************************************/
uint32_t BMP180_ReadUncompensatedPressure(void) {
    
    uint8_t dataBytes[BMP180_PRESSURE_DATA_BYTES] = {0};
    uint32_t up = 0; // uncompensated pressure data UP
    
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
    up = (uint32_t)((((uint32_t)dataBytes[BMP180_PRESSURE_DATA_MSB] << 16) |
            ((uint32_t)dataBytes[BMP180_PRESSURE_DATA_LSB] << 8) |
            (uint32_t)dataBytes[BMP180_PRESSURE_DATA_XLSB]) >> 
            (8 - pBMP180->oversampling));

    return up;
}


/******************************************************************************
 * Function: 
 *
 * Returns: 
 * 
 * Description: According to the manual (BMP180 data sheet rev 1.2), the 
 * temperature is calculated as follows:
 * X1 = (UT - AC6) * AC5 / 2^15
 * X2 = MC * 2^11 / (X1 + MD)
 * B5 = X1 + X2
 * T = (B5 + 8) / 2^4
 ******************************************************************************/
int16_t BMP180_CalcTemperature(uint16_t uncompensatedTemperature) {
    
    int16_t temperature = 0;
    int32_t x1 = 0; 
    int32_t x2 = 0;
    
    x1 = (((int32_t) uncompensatedTemperature 
            - (int32_t) pBMP180->calibParam.ac6)
            * (int32_t) pBMP180->calibParam.ac5) >> 15;
    
    if (x1 == 0 && pBMP180->calibParam.md == 0)
        return 0; // return to avoid zero division
    
    x2 = ((int32_t) pBMP180->calibParam.mc << 11) / (x1 + pBMP180->calibParam.md);
    pBMP180->paramB5 = x1 + x2;
    temperature = (pBMP180->paramB5 + 8) >> 4;
    
    return temperature;
}


/******************************************************************************
 * Function: 
 *
 * Returns: 
 * 
 * Description: According to the manual (BMP180 data sheet rev 1.2), the 
 * pressure is calculated as follows:
 * B6 = B5 - 4000
 * X1 = (B2 * (B6 * B6 / 2^12)) / 2^11
 * X2 = AC2 * B6 / 2^11
 * X3 = X1 + X2
 * B3 = ((((long) AC1 * 4 + X3) << oss) + 2) / 4
 * X1 = AC3 * B6 / 2^13
 * X2 = (B1 * (B6 * B6 / 2^12)) / 2^16
 * X3 = ((X1 + X2) + 2) / 2^2
 * B4 = AC4 * (unsigned long)(X3 + 32768) / 2^15
 * B7 = ((unsigned long)UP - B3) * (50000 >> oss)
 * if (B7 < 0x80000000 ) then
 *      p = (B7 * 2) / B4
 * else
 *      p = (B7 / B4) *2
 * X1 = (p / 2^8) * (p / 2^8)
 * X1 = (X1 * 3038) / 2^16
 * X2 = (-7367 * p) / 2^16
 * p = p + (X1 + X2 + 3791) / 2^4
 * 
 ******************************************************************************/
int32_t BMP180_CalcPressure(uint32_t uncompensatedPressure) {
    
    int32_t pressure = 0;
    int32_t x1 = 0;
    int32_t x2 = 0;
    int32_t x3 = 0;
    int32_t b3 = 0;
    int32_t b6 = 0;
    uint32_t b4 = 0;
    uint32_t b7 = 0;
    
    /* Calculate B6 */
    b6 = pBMP180->paramB5 - 4000;
    
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
    b7 = ((uint32_t)(uncompensatedPressure - b3))
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

/******************************************************************************
 * Function: 
 *
 * Returns: 
 * 
 * Description: 
 ******************************************************************************/
