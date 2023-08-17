/* 
 * File:                bmp180.h
 * Author:              J. Striebel
 * Project:             Weather Station
 * Device:              PIC18F47Q10 @ 16 MHz internal OSC, 5V
 * Platform:            Curiosity HPC board (DM164136)
 * Compiler:            XC8 (v2.41)
 * IDE:                 MPLAB X (v6.10), MCC (5.3.7)
 * Program version:     1.0
 
 * Program Description:
 * --------------------
 * This header file represents an Application Programming Interface (API) for
 * the Bosch BMP180 barometric pressure sensor, allowing to communicate with
 * a MCU via I2C. To ensure application code portability across various 
 * Microchip MCU's, this sensor API utilises the Foundation Service framework
 * which provides fully hardware-abstracted interfaces. 
 * 
 * Hardware Description:
 * --------------------
 * Bosch barometric pressure sensor BMP180 with an I2C interface
 *    
 */

#ifndef BMP180_H
#define	BMP180_H

 /* I2C address definition */
#define BMP180_I2C_ADDR                     0x77

/* Hardware pressure sampling accuracy modes */
typedef enum {
    BMP180_MODE_ULTRALOWPOWER,           
    BMP180_MODE_STANDARD,               
    BMP180_MODE_HIGHRESOLUTION,          
    BMP180_MODE_ULTRAHIGHRESOLUTION    
} BMP180_OVERSAMPLING;

/* Data registers to operate the BMP180 */
#define BMP180_REG_CHIP_ID                  0xD0
#define BMP180_REG_CHIP_ID_VALUE            0x55 // fixed chip-id value 

#define BMP180_REG_SOFT_RESET               0xE0
#define BMP180_REG_CTRL_MEAS                0xF4
#define BMP180_REG_OUT_MSB                  0xF6
#define BMP180_REG_OUT_LSB                  0xF7
#define BMP180_REG_OUT_XLSB                 0xF8

/* Control register values for different oversampling settings (oss) */
#define BMP180_CTRL_MEAS_VAL_TEMP           0x2E // Temperature
#define BMP180_CTRL_MEAS_VAL_OSS_0          0x34 // Pressure (oss = 0)
#define BMP180_CTRL_MEAS_VAL_OSS_1          0x74 // Pressure (oss = 1)
#define BMP180_CTRL_MEAS_VAL_OSS_2          0xB4 // Pressure (oss = 2)
#define BMP180_CTRL_MEAS_VAL_OSS_3          0xF4 // Pressure (oss = 3)

/* Note: According to the BMP180 data sheet, the max. conversion time depends
 * on the oversampling setting (oss). For oss = 0 the max. conversion time is
 * 5 ms */
#define BMP180_CONV_TIME_TEMP               5 // Temperature conversion time [ms]
#define BMP180_CONV_TIME_OSS_0              5 // Pressure conversion time [ms] 
#define BMP180_CONV_TIME_OSS_1              8 
#define BMP180_CONV_TIME_OSS_2              14  
#define BMP180_CONV_TIME_OSS_3              26  

/* Register addresses for calibration coefficients */
#define BMP180_CAL_DATA_SIZE                22 // in bytes
#define BMP180_CAL_COEFF_AC1_MSB            0xAA    
#define BMP180_CAL_COEFF_AC1_LSB            0xAB
#define BMP180_CAL_COEFF_AC2_MSB            0xAC
#define BMP180_CAL_COEFF_AC2_LSB            0xAD
#define BMP180_CAL_COEFF_AC3_MSB            0xAE
#define BMP180_CAL_COEFF_AC3_LSB            0xAF
#define BMP180_CAL_COEFF_AC4_MSB            0xB0
#define BMP180_CAL_COEFF_AC4_LSB            0xB1
#define BMP180_CAL_COEFF_AC5_MSB            0xB2
#define BMP180_CAL_COEFF_AC5_LSB            0xB3
#define BMP180_CAL_COEFF_AC6_MSB            0xB4
#define BMP180_CAL_COEFF_AC6_LSB            0xB5
#define BMP180_CAL_COEFF_B1_MSB             0xB6
#define BMP180_CAL_COEFF_B1_LSB             0xB7
#define BMP180_CAL_COEFF_B2_MSB             0xB8
#define BMP180_CAL_COEFF_B2_LSB             0xB9
#define BMP180_CAL_COEFF_MB_MSB             0xBA
#define BMP180_CAL_COEFF_MB_LSB             0xBB
#define BMP180_CAL_COEFF_MC_MSB             0xBC
#define BMP180_CAL_COEFF_MC_LSB             0xBD
#define BMP180_CAL_COEFF_MD_MSB             0xBE
#define BMP180_CAL_COEFF_MD_LSB             0xBF

/* Constants */
#define BMP180_TEMPERATURE_DATA_BYTES       2
#define BMP180_PRESSURE_DATA_BYTES          3
#define BMP180_TEMPERATURE_DATA_LSB         1
#define BMP180_TEMPERATURE_DATA_MSB         0
#define BMP180_PRESSURE_DATA_MSB            0
#define BMP180_PRESSURE_DATA_LSB            1
#define BMP180_PRESSURE_DATA_XLSB           2 
#define BMP180_INVALID_DATA                 0

/* Data structure that holds device specific calibration parameters */
typedef struct bmp180_cal_coeff_t
{
    int16_t     ac1;
    int16_t     ac2;
    int16_t     ac3;
    uint16_t    ac4;
    uint16_t    ac5;
    uint16_t    ac6;
    int16_t     b1;
    int16_t     b2;
    int16_t     mb;
    int16_t     mc;
    int16_t     md;
} BMP180_CAL_COEFF;


/* Data structure that holds the BMP180 initialisation parameters */
typedef struct bmp180_param_t
{
    BMP180_CAL_COEFF        calibParam;
    BMP180_OVERSAMPLING     oversampling;
    uint8_t                 ossCtrlRegValue;
    uint8_t                 chipId;
    uint8_t                 deviceAddr;
} BMP180_PARAM; 


/******************************************************************************* 
 * Function to initialise the communication with the BMP180 sensor
 ******************************************************************************/
/*
 * @brief This function is used to initialise the communication with the BMP180
 * sensor by reading calibration coefficients from its internal EEPROM
 * 
 * @param bmp180 -> pointer to BMP180 data structure 
 * 
 * @return the result of invoking the initialisation routine
 * @retval 0 -> success
 * @retval 1 -> failed; null pointer to BMP180 data structure was provided
 * @retval 2 -> failed; couldn't establish communication with the sensor
 * 
 * @note
 * The following calibration parameters are read from the sensor's EEPROM:
 *  Parameter  |  MSB    |  LSB    
 * ------------|---------|---------
 *      AC1    |  0xAA   | 0xAB
 *      AC2    |  0xAC   | 0xAD
 *      AC3    |  0xAE   | 0xAF
 *      AC4    |  0xB0   | 0xB1
 *      AC5    |  0xB2   | 0xB3
 *      AC6    |  0xB4   | 0xB5
 *      B1     |  0xB6   | 0xB7
 *      B2     |  0xB8   | 0xB9
 *      MB     |  0xBA   | 0xBB
 *      MC     |  0xBC   | 0xBD
 *      MD     | 0xBE    | 0xBF
 * 
 */
uint8_t BMP180_Init(BMP180_PARAM *bmp180);


/******************************************************************************* 
 * Function to read uncompensated temperature
 ******************************************************************************/
/*
 * @brief This function is used to read the uncompensated temperature from the
 * registers 0xF6 and 0xF7
 * 
 * @param None
 * 
 * @return The uncompensated temperature
 * 
*/
uint16_t BMP180_ReadRawTemperature(void);


/******************************************************************************* 
 * Function to read uncompensated pressure
 ******************************************************************************/
/*
 * @brief This function is used to read the uncompensated temperature from the
 * registers 0xF6 (MSB), 0xF7 (LSB), and 0xF8 (XLSB) 
 * 
 * @param None
 * 
 * @return The uncompensated pressure
 * 
*/

uint32_t BMP180_ReadRawPressure(void);


/******************************************************************************* 
 * Function to calculate the true temperature
 ******************************************************************************/
/*
 * @brief This function calculates the true temperature in Celsius degree based 
 * on the provided raw temperature. Therefore, the raw temperature must be read
 * before invoking. 
 * 
 * @param The result from invoking BMP180_ReadRawPressure(void)
 * 
 * @return The true temperature as a 16-bit signed integer in 0.1 Celsius degree
 * steps
 * 
*/
int16_t BMP180_CalcTemperature(uint16_t rawTemperature);


/******************************************************************************* 
 * Function to calculate the true pressure
 ******************************************************************************/
/*
 * @brief This function calculates the true pressure based on the provided
 * raw pressure and raw temperature. Therefore, these parameters must be read 
 * before invoking this function. 
 * 
 * @param The result from invoking BMP180_ReadRawPressure(void)
 * 
 * @return The true pressure in Pa as a 32-bit signed integer
 * 
*/
int32_t BMP180_CalcPressure(uint32_t rawPressure, uint32_t rawTemperature);


#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* BMP180_H */

