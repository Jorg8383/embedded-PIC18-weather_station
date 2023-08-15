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
 * Function: 
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
            pBMP180->ossCtrlRegValue = BMP180_CTRL_REG_VAL_OSS_0;
            pBMP180->ossConversionTime = BMP180_CONV_TIME_OSS_0;
            break;
        case BMP180_MODE_STANDARD:
            pBMP180->ossCtrlRegValue = BMP180_CTRL_REG_VAL_OSS_1;
            pBMP180->ossConversionTime = BMP180_CONV_TIME_OSS_1;
            break;
        case BMP180_MODE_HIGHRESOLUTION:
            pBMP180->ossCtrlRegValue = BMP180_CTRL_REG_VAL_OSS_2;
            pBMP180->ossConversionTime = BMP180_CONV_TIME_OSS_2;
            break;
        case BMP180_MODE_ULTRAHIGHRESOLUTION:
            pBMP180->ossCtrlRegValue = BMP180_CTRL_REG_VAL_OSS_3;
            pBMP180->ossConversionTime = BMP180_CONV_TIME_OSS_3;
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
